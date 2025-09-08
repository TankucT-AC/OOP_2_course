#include "Point.h"
#include <Windows.h>

// � Point.cpp ��������� ���������� ������� ������� �� Point.h

extern HDC hdc;

// ���������� ��� Location
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

// ���������� ��� Point
Point::Point(int InitX, int InitY) : Location(InitX, InitY)
{
    visible = false;
}

Point::~Point() {}

// ��, ��� ���������������� - ������ ���������� �����
// ������������ � ������������ ����� ����, ��� ����� ����� �� �����

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
    // ������ ��������� ���� ������ �����
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hdc, pen);
    Ellipse(hdc, X - 3, Y - 3, X + 3, Y + 3);
    DeleteObject(pen);
}

void Point::Hide()
{
    visible = false;
    // ������� ����� ����� ������ (��� ������ ����)
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