/******************************************************************
* О Б Ъ Е К Т Н О - О Р И Е Н Т И Р О В А Н Н О Е ПРОГРАММИРОВАНИЕ*
*-----------------------------------------------------------------*
* Project Type  : QT Console Application                          *
* Project Name  : PRINCIPES_ООП\                                  *
* File Name     : ExOOP_ШАБЛОН.СPP                                *
* Language      : C++ QT ver. 5.12.2 Qt Creator 4.8.2.            *
* Programmer(s) : Чечиков Ю.Б.  &  Секретарев В.Е.                *
* Modified By   : Гаджиумаров М.Р                                 *
* Created       : 08/09/25                                        *
* Last Revision : 08/09/25                                        *
* Comment(s)    : ЗАГОТОВКА ДЛЯ РАБОТЫ В КОНСОЛЬНОМ ОКНЕ          *
*                                                                 *
******************************************************************/

#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;           // Пространство имен std

#include "Point.h"
#include "Camera.h"

//макрос для определения кода нажатой клавиши
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

/****************************************************************/
/*Г Л О Б А Л Ь Н Ы Е  К О Н С Т А Н Т Ы  И  П Е Р Е М Е Н Н Ы Е*/
/****************************************************************/

HDC hdc; // Объявим контекст устройства
// Контекст устройства это структура, содержащая
// описание видеокарты на вашем компьютере
// и всех необходимых графических элементов



/****************************************************************/
/*             О С Н О В Н А Я    П Р О Г Р А М М А             */
/****************************************************************/
int main()
{
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
    }//if

    //дескриптор существует

    //получим контекст устройства для консольного окна
    hdc = GetWindowDC(hwnd);

    //если контекст НЕ существует
    if (hdc == 0)
    {
        std::cout << "Handle Device Context NOT FOUND !\n";
        return 2; //ERROR
    }//if 

   //контекст существует - можем работать
//===============================================================
//	М Е С Т О    В С Т А В К И

    int command;
    cin >> command;
    if (command == 1)
    {
        Point APoint(x0, y0);
        APoint.Show();

        for (int i = 0; i < 6; ++i)
        {
            APoint.MoveTo(x0 + 50, y0 + 35 * i);

            //cout << "Press ENTER";
            getchar();
        }

        while (true)
        {
            for (int i = 0; i < 100; ++i)
            {
                APoint.MoveTo(x0 + 100 + i, y0 + i);
                Sleep(50);
            }
        }
    }
    else if (command == 2) {
        std::cout << "\nExample ExOOP_0x START \n";

        std::vector<Camera*> cameras;

        // Создаем разнообразные камеры
        cameras.push_back(new DSLRCamera(50, 50, "Canon EOS R5", 45.0, 2.8));
        cameras.push_back(new SmartphoneCamera(250, 250, "iPhone 15 Pro", 48.0));
        cameras.push_back(new ActionCamera(450, 450, "GoPro Hero 12", 27.0));
        cameras.push_back(new Camera(650, 650, "Standard DSLR", 24.0));

        // Показываем все камеры
        for (Camera* cam : cameras) {
            cam->Show();
        }

        // Перетаскиваем каждую камеру по очереди
        for (int i = 0; i < cameras.size(); ++i) {
            cameras[i]->Drag(30); // Меньший шаг для более точного управления
        }

        // Можно также показать коллизионные боксы
        CollisionWrapper::DrawAllCollisionBoxes();

        // Очистка
        for (Camera* cam : cameras) {
            delete cam;
        }
        cameras.clear();
    }

    

    std::cout << "\nExample ExOOP_0x DONE   \7\n";

    //	К О Н Е Ц     В С Т А В К И
    //===============================================================

    return 0;

}//end main()

/****************  End Of ExOOP_ШАБЛОН.СPP File ****************/

