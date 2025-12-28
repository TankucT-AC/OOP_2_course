#include "Camera.h"
#include <cmath>

Camera::Camera(int InitX, int InitY, const std::string& modelName,
    int InitColWidth, int InitColHeight, int InitRad) : Point(InitX, InitY), model(modelName),
    zoomLevel(1.0), colHeight(InitColHeight), colWidth(InitColWidth), radius(InitRad), isFlashing(false)
{
    id = CAMERA_BASE;
    amount = 15.0;
}

Camera::~Camera() {}

void Camera::Frame()
{
    HPEN bodyPen = CreatePen(PS_SOLID, 3, RGB(70, 70, 70));
    HBRUSH bodyBrush = CreateSolidBrush(RGB(90, 90, 90));
    SelectObject(memDC, bodyPen);
    SelectObject(memDC, bodyBrush);
    RoundRect(memDC, X - 40, Y - 30, X + 40, Y + 30, 10, 10);

    DeleteObject(bodyPen);
    DeleteObject(bodyBrush);
}

void Camera::Lens()
{
    // Объектив с градиентом
    HBRUSH lensBrush = CreateSolidBrush(RGB(20, 20, 20));
    SelectObject(memDC, lensBrush);
    Ellipse(memDC, X + 20, Y - 18, X + 38, Y + 18);

    // Внутренняя линза
    HBRUSH innerLensBrush = CreateSolidBrush(RGB(80, 80, 120));
    SelectObject(memDC, innerLensBrush);
    Ellipse(memDC, X + 24, Y - 14, X + 34, Y + 14);

    DeleteObject(lensBrush);
    DeleteObject(innerLensBrush);
}

void Camera::AuxiliaryElements()
{
    // Вспомогательные элементы
    HPEN detailPen = CreatePen(PS_SOLID, 1, RGB(150, 150, 150));
    SelectObject(memDC, detailPen);
    MoveToEx(memDC, X - 35, Y - 20, NULL);
    LineTo(memDC, X - 25, Y - 20);
    MoveToEx(memDC, X - 35, Y, NULL);
    LineTo(memDC, X - 25, Y);
    MoveToEx(memDC, X - 35, Y + 20, NULL);
    LineTo(memDC, X - 25, Y + 20);

    DeleteObject(detailPen);
}

void Camera::Show()
{
    visible = true;

    Frame();
    Lens();
    AuxiliaryElements();
}

void Camera::Hide()
{
    if (!visible) return;
    visible = false;

    HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(memDC, blackPen);
    SelectObject(memDC, blackBrush);
    Rectangle(memDC, X - 70, Y - 40, X + 50, Y + 40);
    DeleteObject(blackPen);
    DeleteObject(blackBrush);
}

void Camera::TakePhoto()
{
    if (isFlashing) return;
    isFlashing = true;


    HPEN flashPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 200));
    HBRUSH flashBrush = CreateSolidBrush(RGB(255, 255, 180));
    SelectObject(hdc, flashPen);
    SelectObject(hdc, flashBrush);

    // Основная вспышка - начинается от края объектива
    Ellipse(hdc, X + 25, Y - 25, X + 65, Y + 25);

    // Расходящиеся лучи
    for (int i = 0; i < 8; i++)
    {
        double angle = i * 3.14159 / 4;
        int x1 = X + 55 + (int)(15 * cos(angle));
        int y1 = Y + (int)(15 * sin(angle));
        int x2 = X + 75 + (int)(25 * cos(angle));
        int y2 = Y + (int)(25 * sin(angle));

        HPEN rayPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 150));
        SelectObject(hdc, rayPen);
        MoveToEx(hdc, x1, y1, NULL);
        LineTo(hdc, x2, y2);
        DeleteObject(rayPen);
    }

    DeleteObject(flashPen);
    DeleteObject(flashBrush);
}

void Camera::ClearFlash()
{
    if (isFlashing)
    {
        HPEN clearPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HBRUSH clearBrush = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(hdc, clearPen);
        SelectObject(hdc, clearBrush);

        // Область очистки тоже смещаем
        Rectangle(hdc, X + 20, Y - 40, X + 110, Y + 40);

        DeleteObject(clearPen);
        DeleteObject(clearBrush);
        isFlashing = false;

        Show();
    }
}

float Camera::DisCharge() { return amount; }

// Методы смарткамеры
SmartphoneCamera::SmartphoneCamera(int InitX, int InitY, const std::string& modelName)
    : Camera(InitX, InitY, modelName, 70, 90), nightMode(false), hdrMode(false)
{
    id = CAMERA_SMARTPHONE;
    radius = 80;
}

SmartphoneCamera::~SmartphoneCamera() {}

void SmartphoneCamera::Frame()
{
    // Стильный корпус смартфона
    HPEN bodyPen = CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
    HBRUSH bodyBrush = CreateSolidBrush(RGB(30, 30, 30));
    SelectObject(memDC, bodyPen);
    SelectObject(memDC, bodyBrush);
    RoundRect(memDC, X - 20, Y - 35, X + 20, Y + 35, 15, 15);

    // Экран
    HBRUSH screenBrush = CreateSolidBrush(RGB(10, 10, 40));
    SelectObject(memDC, screenBrush);
    RoundRect(memDC, X - 18, Y - 32, X + 18, Y + 28, 8, 8);

    DeleteObject(bodyPen);
    DeleteObject(bodyBrush);
    DeleteObject(screenBrush);
}

void SmartphoneCamera::Lens()
{
    // Камеры как у современного смартфона
    HBRUSH cameraBrush = CreateSolidBrush(RGB(80, 80, 80));
    SelectObject(memDC, cameraBrush);

    // Основная камера
    Ellipse(memDC, X + 8, Y - 28, X + 16, Y - 20);
    // Дополнительные камеры
    Ellipse(memDC, X + 2, Y - 28, X + 7, Y - 23);
    Ellipse(memDC, X + 17, Y - 28, X + 22, Y - 23);

    DeleteObject(cameraBrush);
}

void SmartphoneCamera::AuxiliaryElements()
{
    // Вспышка - небольшой кружок в правом верхнем углу
    HBRUSH flashBrush = CreateSolidBrush(RGB(100, 100, 100));
    SelectObject(memDC, flashBrush);
    Ellipse(memDC, X - 8, Y - 30, X - 3, Y - 25);
    DeleteObject(flashBrush);
}

void SmartphoneCamera::Hide()
{
    if (!visible) return;

    visible = false;

    HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(memDC, blackPen);
    SelectObject(memDC, blackBrush);

    // Заливаем область смартфона
    Rectangle(memDC, X - 30, Y - 45, X + 30, Y + 45);

    DeleteObject(blackPen);
    DeleteObject(blackBrush);
}

void SmartphoneCamera::TakePhoto()
{
    if (isFlashing) return;
    isFlashing = true;

    // Вспышка в виде небольшого кружочка в правом верхнем углу
    HPEN flashPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 220));
    HBRUSH flashBrush = CreateSolidBrush(RGB(255, 255, 200));
    SelectObject(hdc, flashPen);
    SelectObject(hdc, flashBrush);

    // Небольшой кружок вспышки в правом верхнем углу камеры
    Ellipse(hdc, X - 15, Y - 37, X + 4, Y - 18);

    DeleteObject(flashPen);
    DeleteObject(flashBrush);
}

void SmartphoneCamera::ClearFlash()
{
    if (isFlashing)
    {
        HPEN clearPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HBRUSH clearBrush = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(hdc, clearPen);
        SelectObject(hdc, clearBrush);

        // Очищаем небольшую область вокруг вспышки
        Rectangle(hdc, X - 12, Y - 34, X + 2, Y - 21);

        DeleteObject(clearPen);
        DeleteObject(clearBrush);
        isFlashing = false;
        Show();
    }
}

// Методы DSLR-камеры
DSLRCamera::DSLRCamera(int InitX, int InitY, const std::string& modelName)
    : Camera(InitX, InitY, modelName, 100, 80), manualMode(false) 
{
    id = CAMERA_DSLR;
    radius = 150;
}

DSLRCamera::~DSLRCamera() {}

void DSLRCamera::Frame()
{
    // Корпус DSLR
    HPEN bodyPen = CreatePen(PS_SOLID, 3, RGB(40, 40, 40));
    HBRUSH bodyBrush = CreateSolidBrush(RGB(60, 60, 60));
    SelectObject(memDC, bodyPen);
    SelectObject(memDC, bodyBrush);
    Rectangle(memDC, X - 40, Y - 30, X + 40, Y + 30);

    DeleteObject(bodyPen);
    DeleteObject(bodyBrush);
}

void DSLRCamera::Lens()
{
    // Большой объектив
    HBRUSH lensBrush = CreateSolidBrush(RGB(20, 20, 20));
    SelectObject(memDC, lensBrush);
    Ellipse(memDC, X + 25, Y - 25, X + 55, Y + 25);

    DeleteObject(lensBrush);
}

void DSLRCamera::AuxiliaryElements()
{
    // Добавил профессиональные детали
    HBRUSH gripBrush = CreateSolidBrush(RGB(30, 30, 30));
    SelectObject(memDC, gripBrush);
    Rectangle(memDC, X - 45, Y - 15, X - 35, Y + 25);

    DeleteObject(gripBrush);
}

void DSLRCamera::Hide()
{
    if (!visible) return;

    visible = false;

    HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(memDC, blackPen);
    SelectObject(memDC, blackBrush);

    // Заливаем большую область DSLR камеры
    Rectangle(memDC, X - 50, Y - 35, X + 58, Y + 35);

    DeleteObject(blackPen);
    DeleteObject(blackBrush);
}

void DSLRCamera::TakePhoto()
{
    if (isFlashing) return;
    isFlashing = true;

    // Нормальная мощная вспышка для DSLR
    HPEN flashPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 190));
    HBRUSH flashBrush = CreateSolidBrush(RGB(255, 255, 170));
    SelectObject(hdc, flashPen);
    SelectObject(hdc, flashBrush);

    // Большая прямоугольная вспышка (как у настоящих DSLR)
    Rectangle(hdc, X + 50, Y - 40, X + 80, Y + 40);

    // Мощный световой поток
    POINT lightBeam[4] =
    {
        {X + 80, Y - 30},
        {X + 120, Y - 50},
        {X + 120, Y + 50},
        {X + 80, Y + 30}
    };

    HBRUSH beamBrush = CreateSolidBrush(RGB(255, 255, 140));
    SelectObject(hdc, beamBrush);
    Polygon(hdc, lightBeam, 4);

    DeleteObject(flashPen);
    DeleteObject(flashBrush);
    DeleteObject(beamBrush);
}


void DSLRCamera::ClearFlash()
{
    if (isFlashing)
    {
        HPEN clearPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HBRUSH clearBrush = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(hdc, clearPen);
        SelectObject(hdc, clearBrush);

        // Большая область очистки для DSLR вспышки
        Rectangle(hdc, X + 45, Y - 55, X + 125, Y + 55);

        DeleteObject(clearPen);
        DeleteObject(clearBrush);
        isFlashing = false;
        Show();
    }
}


ActionCamera::ActionCamera(int InitX, int InitY, const std::string& modelName, double res)
    : Camera(InitX, InitY, modelName, 80, 70), waterproof(true), wideAngle(true) 
{
    id = CAMERA_ACTION;
    radius = 60;
}

ActionCamera::~ActionCamera() {}

void ActionCamera::Frame()
{
    // Компактный прочный корпус экшн-камеры - камера слева
    HPEN bodyPen = CreatePen(PS_SOLID, 2, RGB(0, 100, 0));
    HBRUSH bodyBrush = CreateSolidBrush(RGB(0, 150, 0));
    SelectObject(memDC, bodyPen);
    SelectObject(memDC, bodyBrush);
    Rectangle(memDC, X - 25, Y - 20, X + 25, Y + 20);

    DeleteObject(bodyPen);
    DeleteObject(bodyBrush);
}

void ActionCamera::Lens()
{
    // Защищенный объектив - СЛЕВА
    HBRUSH lensBrush = CreateSolidBrush(RGB(40, 40, 40));
    SelectObject(memDC, lensBrush);
    Ellipse(memDC, X - 22, Y - 12, X - 10, Y + 12); // Объектив слева

    // Внутренняя линза
    HBRUSH innerLensBrush = CreateSolidBrush(RGB(100, 100, 150));
    SelectObject(memDC, innerLensBrush);
    Ellipse(memDC, X - 19, Y - 9, X - 13, Y + 9);

    DeleteObject(lensBrush);
    DeleteObject(innerLensBrush);
}

void ActionCamera::AuxiliaryElements()
{
    // Кнопки управления - справа
    HBRUSH buttonBrush = CreateSolidBrush(RGB(60, 60, 60));
    SelectObject(memDC, buttonBrush);
    Ellipse(memDC, X + 10, Y - 15, X + 15, Y - 10);
    Ellipse(memDC, X + 10, Y, X + 15, Y + 5);
    Ellipse(memDC, X + 10, Y + 10, X + 15, Y + 15);

    // Индикатор записи (красный)
    HBRUSH recBrush = CreateSolidBrush(RGB(255, 0, 0));
    SelectObject(memDC, recBrush);
    Ellipse(memDC, X + 5, Y - 18, X + 8, Y - 15);

    DeleteObject(buttonBrush);
    DeleteObject(recBrush);
}


void ActionCamera::Hide()
{
    if (!visible) return;

    visible = false;

    HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(memDC, blackPen);
    SelectObject(memDC, blackBrush);

    // Заливаем область экшн-камеры
    Rectangle(memDC, X - 30, Y - 25, X + 30, Y + 25);

    DeleteObject(blackPen);
    DeleteObject(blackBrush);
}

void ActionCamera::TakePhoto()
{
    if (isFlashing) return;
    isFlashing = true;

    // Вспышка для экшн-камеры - начинается слева (от объектива) и больше по размеру
    HPEN flashPen = CreatePen(PS_SOLID, 2, RGB(200, 220, 255));
    HBRUSH flashBrush = CreateSolidBrush(RGB(180, 200, 255));
    SelectObject(hdc, flashPen);
    SelectObject(hdc, flashBrush);

    // Круглая вспышка - начинается от левого объектива и больше по размеру
    Ellipse(hdc, X - 35, Y - 20, X - 15, Y + 20); // Увеличена и смещена влево

    // Рассеянный свет - тоже больше
    for (int i = 0; i < 3; i++)
    {
        int radius = 15 + i * 8; // Увеличены радиусы
        HPEN haloPen = CreatePen(PS_SOLID, 1, RGB(150, 170, 255));
        SelectObject(hdc, haloPen);
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        int centerX = X - 25; // Центр смещен влево
        Ellipse(hdc, centerX - radius, Y - radius, centerX + radius, Y + radius);
        DeleteObject(haloPen);
    }

    DeleteObject(flashPen);
    DeleteObject(flashBrush);
}

void ActionCamera::ClearFlash()
{
    if (isFlashing)
    {
        HPEN clearPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HBRUSH clearBrush = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(hdc, clearPen);
        SelectObject(hdc, clearBrush);

        Rectangle(hdc, X - 65, Y - 35, X + 10, Y + 35);

        DeleteObject(clearPen);
        DeleteObject(clearBrush);
        isFlashing = false;
        Show();
    }
}