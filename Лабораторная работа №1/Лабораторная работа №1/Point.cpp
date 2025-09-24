#include "Point.h"
#include <windows.h>

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

int Point::GetX() { return X; }
int Point::GetY() { return Y; }

void Point::SetX(int NewX) { X = NewX; }
void Point::SetY(int NewY) { Y = NewY; }

// Реализация для Circle
Circle::Circle(int InitX, int InitY, int InitRad) : Point(InitX, InitY)
{
    radius = InitRad;
}

Circle::~Circle() {}

void Circle::Show()
{
    visible = true;
    // Создаем красное перо и кисть
    HPEN redPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));

    // Просто выбираем новые объекты
    SelectObject(hdc, redPen);
    SelectObject(hdc, redBrush);

    // Рисуем
    Ellipse(hdc, X - radius, Y - radius, X + radius, Y + radius);

    // Удаляем объекты
    DeleteObject(redPen);
    DeleteObject(redBrush);
}

void Circle::Hide() 
{
    visible = false;
    // Стираем рисуя белым цветом (или цветом фона)
    HPEN clearPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HBRUSH clearBrush = CreateSolidBrush(RGB(255, 255, 255));

    // Просто выбираем новые объекты
    SelectObject(hdc, clearPen);
    SelectObject(hdc, clearBrush);

    // Рисуем
    Ellipse(hdc, X - radius, Y - radius, X + radius, Y + radius);

    // Удаляем объекты
    DeleteObject(clearPen);
    DeleteObject(clearBrush);
}

void Circle::EditRadius(int NewRadius)
{
    radius -= NewRadius;
}