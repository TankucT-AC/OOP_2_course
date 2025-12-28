#include "Point.h"

#include <iostream>
#include <vector>

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

extern HDC hdc;
extern HDC memDC;

// Реализация для Location
void Location::SetX(int NewX) { X = NewX; }
void Location::SetY(int NewY) { Y = NewY; }
Location::Location(int InitX, int InitY) { X = InitX; Y = InitY; }
Location::~Location() {}
int Location::GetX() { return X; }
int Location::GetY() { return Y; }

// Реализация для Point
Point::Point(int InitX, int InitY) : Location(InitX, InitY) { visible = false; }
Point::~Point() {}

void Point::Show()
{
    visible = true;
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(memDC, pen);
    Ellipse(memDC, X - 3, Y - 3, X + 3, Y + 3);
    DeleteObject(pen);
}

void Point::Hide()
{
    visible = false;
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(memDC, pen);
    Ellipse(memDC, X - 3, Y - 3, X + 3, Y + 3);
    DeleteObject(pen);
}

void Point::MoveTo(int NewX, int NewY)
{
    Hide();
    X = NewX;
    Y = NewY;
    Show();
}

// Функция перемещения точки по нажатию клавиш
void Point::Drag(int Step)
{
    int FigX = GetX(), FigY = GetY();

    while (true) {
        if (KEY_DOWN(VK_SPACE))
        {
            Sleep(500);
            break;
        }

        int newX = FigX;
        int newY = FigY;

        if (KEY_DOWN(VK_LEFT))
        {
            newX -= Step;
        }
        if (KEY_DOWN(VK_RIGHT))
        {
            newX += Step;
        }
        if (KEY_DOWN(VK_UP))
        {
            newY -= Step;
        }
        if (KEY_DOWN(VK_DOWN))
        {
            newY += Step;
        }

        Sleep(50);
    }
}

// Реализация для Circle
Circle::Circle(int InitX, int InitY, int InitRad) : Point(InitX, InitY) { radius = InitRad; }
Circle::~Circle() {}

// Показ круга
void Circle::Show()
{
    visible = true;
    HPEN redPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
    SelectObject(memDC, redPen);
    SelectObject(memDC, redBrush);
    Ellipse(memDC, X - radius, Y - radius, X + radius, Y + radius);
    DeleteObject(redPen);
    DeleteObject(redBrush);
}

// Скрытие круга
void Circle::Hide()
{
    visible = false;
    HPEN clearPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HBRUSH clearBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(memDC, clearPen);
    SelectObject(memDC, clearBrush);
    Ellipse(memDC, X - radius, Y - radius, X + radius, Y + radius);
    DeleteObject(clearPen);
    DeleteObject(clearBrush);
}

// Изменение радиуса
void Circle::EditRadius(int NewRadius) { radius -= NewRadius; }