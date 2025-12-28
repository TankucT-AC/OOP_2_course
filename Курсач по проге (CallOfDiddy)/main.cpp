/******************************************************************
* О Б Ъ Е К Т Н О - О Р И Е Н Т И Р О В А Н Н О Е ПРОГРАММИРОВАНИЕ*
*-----------------------------------------------------------------*
* Project Type  : QT Console Application                          *
* Project Name  : PRINCIPES_ООП\                                  *
* File Name     : ExOOP_ШАБЛОН.СPP                                *
* Language      : C/C++                                           *
* Programmer(s) : Чечиков Ю.Б.  &  Секретарев В.Е.                *
* Modified By   : Гаджиумаров М.Р                                 *
* Created       : 08/09/25                                        *
* Last Revision : 14/12/25                                        *
* Comment(s)    : Интерфейсы и абстрактные базовые классы         *
*                                                                 *
******************************************************************/


#define NOMINMAX
#include <windows.h>

// Либа, ответственная за звук
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// Либа, ответственная за вывод фоток
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

#include <iostream>
#include <vector>
#include <string>
#include <random>

using namespace std;

// Константы для работы с фото
Gdiplus::GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR           gdiplusToken;

#include "Figures.h"
#include "Camera.h"
#include "GameBoard.h"

//макрос для определения кода нажатой клавиши
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

HDC hdc;         // Реальный экран (консоль)
HDC memDC;       // Буфер в памяти
HBITMAP hMemBM;  // Поверхность буфера

// Массив камер с фотками P Diddy
const wchar_t* Pdidy[4] = { L"1.jpg", L"2.jpg", L"3.jpg", L"4.jpg" };

// Функция вывода фото
void DrawImage(HDC targetDC, const wchar_t* filename, int x, int y, int width, int height) 
{
    // Создаем объект графики на основе твоего memDC
    Gdiplus::Graphics graphics(targetDC);

    // Загружаем изображение (поддерживает jpg, png, bmp, gif)
    Gdiplus::Image image(filename);

    // Рисуем в указанных координатах с нужным размером
    graphics.DrawImage(&image, x, y, width, height);
}

// Функция инициализации двойной буферизации 
void InitDoubleBuffering(GameBoard& board,int padding) {
    hdc = GetWindowDC(GetConsoleWindow());
    memDC = CreateCompatibleDC(hdc);

    // Размеры поля + отступ
    int w = board.GetColWidth() + (2 * padding);
    int h = board.GetColHeight() + (2 * padding);

    // Создаем холст ровно под размер поля
    hMemBM = CreateCompatibleBitmap(hdc, w, h);
    SelectObject(memDC, hMemBM);
}

// Функция проверки коллизии с фигурами
bool CheckCollision(int testX, int testY, Camera& camera, FigureBase& trigger)
{
    int camCollisionWidth = camera.GetColisionWidth();
    int camCollisionHeight = camera.GetColisionHeight();

    int camLeft = testX - camCollisionWidth / 2;
    int camRight = testX + camCollisionWidth / 2;
    int camTop = testY - camCollisionHeight / 2;
    int camBottom = testY + camCollisionHeight / 2;

    int triggerX = trigger.GetX();
    int triggerY = trigger.GetY();
    int triggerCollisionWidth = trigger.GetColisionWidth();
    int triggerCollisionHeight = trigger.GetColisionHeight();

    int triggerLeft = triggerX - triggerCollisionWidth / 2;
    int triggerRight = triggerX + triggerCollisionWidth / 2;
    int triggerTop = triggerY - triggerCollisionHeight / 2;
    int triggerBottom = triggerY + triggerCollisionHeight / 2;

    return (camLeft <= triggerRight &&
        camRight >= triggerLeft &&
        camTop <= triggerBottom &&
        camBottom >= triggerTop);
}

// Функция проверки нахождения внутри поля
bool IsWithinBounds(int testX, int testY, Camera* camera, GameBoard* board, bool& isExit)
{
    // 1. Координаты и размеры камеры
    int camWidth = camera->GetColisionWidth();
    int camHeight = camera->GetColisionHeight();
    int camLeft = testX - camWidth / 2;
    int camRight = testX + camWidth / 2;
    int camTop = testY - camHeight / 2;
    int camBottom = testY + camHeight / 2;

    // 2. Координаты поля
    int bX = board->GetX();
    int bY = board->GetY();
    int bColWidth = board->GetColWidth();
    int bColHeight = board->GetColHeight();

    int fieldLeft = bX - bColWidth / 2;
    int fieldRight = bX + bColWidth / 2;
    int fieldTop = bY - bColHeight / 2;
    int fieldBottom = bY + bColHeight / 2;

    // 3. Проверка выхода за границы поля
    bool isOutside = (camLeft < fieldLeft || camRight > fieldRight ||
        camTop < fieldTop || camBottom > fieldBottom);

    if (isOutside)
    {
        // 4. Проверяем столкновение с дверью, если она открыта
        if (board->IsExitOpen())
        {
            // Получаем границы двери
            int dX = board->GetDoorX();
            int dY = board->GetDoorY();
            int dW = board->GetDoorWidth();
            int dH = board->GetDoorHeight();

            int doorLeft = dX - dW / 2;
            int doorRight = dX + dW / 2;
            int doorTop = dY - dH / 2;
            int doorBottom = dY + dH / 2;

            // Проверка пересечения камеры с дверью (AABB collision)
            bool hitDoor = (camLeft <= doorRight && camRight >= doorLeft &&
                camTop <= doorBottom && camBottom >= doorTop);

            if (hitDoor)
            {
                isExit = true;  // Меняем флаг
                return false;   // Возвращаем false, чтобы остановить движение в цикле
            }
        }
        return false; // Просто стена
    }

    return true; // Мы внутри поля, всё ок
}


int main()
{
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    //объявление и инициализация переменных

    //для классов LOCATION и POINT
    int x0 = 300;	//стартовые координаты ТОЧКИ
    int y0 = 300;

    //для класса CIRCLE
    int Radius0 = 50;		//начальное значение радиуса КРУГА
    int DeltaRad = 30;		//шаг изменения радиуса КРУГА

    //для класса RING
    int RingW0 = 20;		//начальное значение толщины КОЛЬЦА
    int DeltaWide0 = 10;	//шаг изменения толщины КОЛЬЦА


    //получим дескриптор консольного окна
    HWND hwnd = GetConsoleWindow();

    //если дескриптор НЕ существует
    if (hwnd == NULL)
    {
        std::cout << "Console Window Descriptor NOT FOUND !\n";
        return 1; //ERROR
    }

    //дескриптор существует

    //получим контекст устройства для консольного окна
    memDC = GetWindowDC(hwnd);

    //если контекст НЕ существует
    if (memDC == 0)
    {
        std::cout << "Handle Device Context NOT FOUND !\n";
        return 2;
    }

    // Создаем разные камеры
    Camera baseCamera(x0 + 250, y0 + 250, "Base Camera");
    SmartphoneCamera smartphoneCamera(x0 + 250, y0 + 250, "iPhone Camera");
    DSLRCamera dslrCamera(x0 + 250, y0 + 250, "Canon DSLR");
    ActionCamera actionCamera(x0 + 250, y0 + 250, "GoPro", 4.0);

    // Игровое поле
    int bX = x0 + 1200, bY = y0 + 500;
    GameBoard board(bX, bY, 2400, 1200, &baseCamera);

    // Массив камер
    Camera* cameras[4] = { &baseCamera, &smartphoneCamera, &dslrCamera, &actionCamera };

    int initX = x0 + 500, initY = y0 + 500;

    // ГПСЧ для случайного задавания координат
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> randX(bX - board.GetColWidth() / 2 + 10, bX + board.GetColWidth() / 2 - 10);
    uniform_int_distribution<int> randY(bY - board.GetColHeight() / 2 + 10, bY + board.GetColHeight() / 2 - 10);
    uniform_int_distribution<int> rand_Pdiddy(0, 50);

    // Триггеры
    Oil oil(initX, initY, 50);
    Cross cross(initX + 300, initY, 50);
    Accumulator acc(initX + 600, initY);
    Key key(randX(gen), randY(gen));

    // Массив объектов для проверки коллизий
    FigureBase* objects[4] = { &oil, &cross, &acc, &key };

    // Текущая камера (изначально базовая)
    int currentCameraIndex = 2;
    Camera* currentCamera = cameras[currentCameraIndex];


    int Step = 50; // шаг перемещения
    int FigX = currentCamera->GetX(), FigY = currentCamera->GetY();

    bool moveSquareLock = false; // флаг блокировки кубика
    bool isExit = false; // флаг выхода из комнаты
    bool GameIsWin = false; // флаг проверки на выигрыш

    int padding = 50; // отступ
    InitDoubleBuffering(board, padding);

    while (true)
    {
        // Очистка фона
        RECT r = {
            board.GetX() - board.GetColWidth() / 2 - padding,
            board.GetY() - board.GetColHeight() / 2 - padding,
            board.GetX() + board.GetColWidth() / 2 + padding,
            board.GetY() + board.GetColHeight() / 2 + padding
        };
        FillRect(memDC, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));

        // Отрисовка поля и
        // перерисовка всех фигур
        for (int i = 0; i < 4; ++i)
        {
            objects[i]->Show();
        }
        board.Show(currentCamera);
        key.Show();
        currentCamera->Show();

        // 2. Рассчитываем новые координаты для BitBlt
        int drawX = board.GetX() - board.GetColWidth() / 2 - padding;
        int drawY = board.GetY() - board.GetColHeight() / 2 - padding;

        // Ширина и высота + 2 * padding
        int fullWidth = board.GetColWidth() + (padding * 2);
        int fullHeight = board.GetColHeight() + (padding * 2);

        // Сдвигаем сетку
        SetWindowOrgEx(memDC, drawX, drawY, NULL);

        // Вероятность скримера - 1/75
        if (rand_Pdiddy(gen) == 25)
        {
            // Выбираем случайную фотку пидиди
            auto file = Pdidy[(rand_Pdiddy(gen) % 4)];
            // Звук скримера
            PlaySound(TEXT("scream.wav"), NULL, SND_FILENAME | SND_ASYNC);
            // Выводим на экран
            DrawImage(memDC, file, drawX, drawY, board.GetColWidth(), board.GetColHeight());
        }

        // Копируем расширенную область
        BitBlt(hdc, drawX, drawY, fullWidth, fullHeight,
            memDC, drawX, drawY, SRCCOPY);

        Sleep(50);

        if (board.IsDischarge())
        {
            break;
        }

        if (KEY_DOWN(VK_SPACE))
        {
            Sleep(500);
            break;
        };

        // 1. Перемещение триггеров
        if (!moveSquareLock) 
            objects[0]->MoveTo(currentCamera);

        objects[2]->MoveTo();

        // Перемещение камеры
        int newX = FigX;
        int newY = FigY;

        if (KEY_DOWN(VK_LEFT)) newX -= Step;
        if (KEY_DOWN(VK_RIGHT)) newX += Step;
        if (KEY_DOWN(VK_UP)) newY -= Step;
        if (KEY_DOWN(VK_DOWN)) newY += Step;


        bool collisionDetected = false;
        int camera_id = currentCamera->GetCameraType();
        int trigger_id = -1;

        // Если мы не внутри поля при новых координатах 
        // и при этом мы пытаемся выйти не через открытый выход, 
        // то пропускаем остальные действия.
        if (!IsWithinBounds(newX, newY, currentCamera, &board, isExit) && !isExit) continue;

        // Если вышли через открытый выход, то завершаем игру
        if (isExit)
        {
            GameIsWin = true;
            break;
        }

        // Проверяем коллизии с фигурами
        for (int i = 0; i < 4; i++)
        {
            if (CheckCollision(newX, newY, *currentCamera, *objects[i]))
            {
                collisionDetected = true;
                trigger_id = objects[i]->GetFigureType();
                if (trigger_id == oil.GetFigureType()) moveSquareLock = true;
                if (trigger_id == key.GetFigureType())
                {
                    board.AddKey();
                    objects[i]->Point::MoveTo(randX(gen), randY(gen));
                }
                break;
            }
        }

        // Если собрали нужное количество ключей, то выходим
        if (board.GetKeysCount() >= 5) board.OpenExit();


        if (!collisionDetected)
        {
            FigX = newX;
            FigY = newY;
            currentCamera->MoveTo(FigX, FigY);
            moveSquareLock = false;
        }
        else
        {
            // Логика столкновения (вспышка, смена камеры)
            if (trigger_id == acc.GetFigureType())
            {
                int amount = 10;
                acc.Discharge(amount);
                if (acc.GetCurrentLevel() > 0)
                    board.Crease(amount);
            }

            if (trigger_id == oil.GetFigureType())
            {
                board.Decrease(currentCamera->DisCharge());
            }

            currentCamera->TakePhoto();
            Sleep(200);
            currentCamera->ClearFlash();

            // Смена камеры при коллизии с крестом
            if (trigger_id == cross.GetFigureType()) 
            {
                int new_camera_id = (camera_id + trigger_id) % 4;
                currentCamera->Hide();
                currentCamera = cameras[new_camera_id];
            }
            currentCamera->MoveTo(FigX, FigY);
        }
    }

    // Вывод итогового текста
    if (GameIsWin) board.YouWin();
    else board.GameOver();

    std::cout << "\nExample ExOOP_0x DONE   \7\n";

    //	К О Н Е Ц     В С Т А В К И
    //===============================================================
    Gdiplus::GdiplusShutdown(gdiplusToken);

    return 0;

}//end main()

/****************  End Of ExOOP_ШАБЛОН.СPP File ****************/

