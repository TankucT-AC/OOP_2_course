#include "Point.h"
#include <Windows.h>

// В Point.cpp находится реализация методов классов из Point.h

extern HDC hdc;

// Реализация для Location
void Location::SetX(int NewX)
{
    X = NewX;
}

void Location::SetY(int NewY)
{
    Y = NewY;
}

Location::Location(int InitX, int InitY)
{
    X = InitX;
    Y = InitY;
}

Location::~Location() {}

int Location::GetX() { return X; }
int Location::GetY() { return Y; }

// Реализация для Point
Point::Point(int InitX, int InitY) : Location(InitX, InitY)
{
    visible = false;
}

Point::~Point() {}

// То, что закомментировано - старая реализация точки
// Использовать её нежелательно ввиду того, что точки видно не будет

//void Point::Show()
//{
//	visible = true;
//	SetPixel(hdc, X, Y, RGB(255, 0, 0));
//}
//
//void Point::Hide()
//{
//	visible = false;
//	SetPixel(hdc, X, Y, RGB(0, 0, 255));
//}

void Point::Show()
{
    visible = true;
    // Рисуем маленький круг вместо точки
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hdc, pen);
    Ellipse(hdc, X - 3, Y - 3, X + 3, Y + 3);
    DeleteObject(pen);
}

void Point::Hide()
{
    visible = false;
    // Стираем рисуя белым цветом (или цветом фона)
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    SelectObject(hdc, pen);
    Ellipse(hdc, X - 3, Y - 3, X + 3, Y + 3);
    DeleteObject(pen);
}

void Point::MoveTo(int NewX, int NewY)
{
    Hide();
    X = NewX;
    Y = NewY;
    Show();
}
