#pragma once

#include <windows.h>
#include <string>

extern HDC hdc;

class Location {
protected:
    int X, Y;
public:
    Location(int InitX = 0, int InitY = 0);
    virtual ~Location();
    void SetX(int NewX);
    void SetY(int NewY);
    int GetX();
    int GetY();
};

class Point : public Location {
protected:
    bool visible;
public:
    Point(int InitX = 0, int InitY = 0);
    ~Point();
    virtual void Show();
    virtual void Hide();
    virtual void MoveTo(int NewX, int NewY);

    virtual void Drag(int Step);
};

class Circle : public Point {
protected:
    int radius;
public:
    Circle(int InitX = 0, int InitY = 0, int InitRad = 10);
    virtual ~Circle();
    virtual void Show() override;
    virtual void Hide() override;
    void EditRadius(int NewRadius);
    int GetRadius() { return radius; }
};