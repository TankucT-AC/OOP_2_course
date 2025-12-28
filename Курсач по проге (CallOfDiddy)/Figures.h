#pragma once

#include "Camera.h"

#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

// Базовый класс для всех фигур, реализующий общую функциональность
class FigureBase : public Point
{
protected:
    int colWidth;
    int colHeight;

    // Индексы для фигур
    enum FigureType {
        FIGURE_BASE,
        FIGURE_CROSS,
        FIGURE_SQUARE,
        FIGURE_ACCUMULATOR,
        FIGURE_KEY
    };

    FigureType id;

public:
    FigureBase(int InitX, int InitY, int InitColWidth = 50, int InitColHeight = 50) :
        Point(InitX, InitY), colWidth(InitColWidth), colHeight(InitColHeight) 
    {
        id = FIGURE_BASE;
    }

    virtual ~FigureBase() {}

    // Реализация методов интерфейса IFigure
    int GetX() { return X; }
    int GetY() { return Y; }
    int GetColisionWidth() { return colWidth; }
    int GetColisionHeight() { return colHeight; }
    FigureType GetFigureType() { return id; }

    virtual void MoveTo() {}
    virtual void MoveTo(Camera* cam) {}
};

// Класс квадрата
class Oil : public FigureBase
{
protected:
    int len;

    double speed;

    void DrawImageOil(HDC targetDC, const wchar_t* filename, int x, int y, int width, int height)
    {
        // Создаем объект графики на основе твоего memDC
        Gdiplus::Graphics graphics(targetDC);

        // Загружаем изображение (поддерживает jpg, png, bmp, gif)
        Gdiplus::Image image(filename);

        // Рисуем в указанных координатах с нужным размером
        graphics.DrawImage(&image, x, y, width, height);
    }

public:
    Oil(int InitX, int InitY, int InitLen, double InitSpeed = 0.125)
        : FigureBase(InitX, InitY, InitLen, InitLen), len(InitLen), speed(InitSpeed)
    {
        id = FIGURE_SQUARE;
    }

    ~Oil() override { Hide(); }

    virtual void Show();
    virtual void Hide();
    virtual void MoveTo(Camera* cam);

    void ChangeLength(int newLen) {
        len = newLen;
        colWidth = newLen;
        colHeight = newLen;
    }

    int GetLength() { return len; }
};

// Класс креста
class Cross : public FigureBase
{
protected:
    int size;
    int ceil, floor;
    int d;
public:
    Cross(int InitX, int InitY, int InitSize)
        : FigureBase(InitX, InitY, InitSize, InitSize), size(InitSize) 
    {
        id = FIGURE_CROSS;
        ceil = GetY() + 100;
        floor = GetY() - 100;
        d = 1;
    }

    ~Cross() { Hide(); }

    virtual void Show();
    virtual void Hide();
    virtual void MoveTo();

    void ChangeSize(int newSize) {
        size = newSize;
        colWidth = newSize;
        colHeight = newSize;
    }

    int GetSize() { return size; }
};

// Класс аккумулятора
class Accumulator : public FigureBase
{
protected:
    float capacity;
    float batteryCharge;
    bool isCharging;

    int centerX, centerY;
    int radius;
    double angle;
public:
    Accumulator(int InitX, int InitY)
        : FigureBase(InitX, InitY, 60, 110), capacity(100), batteryCharge(100), 
        isCharging(true), centerX(InitX), centerY(InitY)
    {
        id = FIGURE_ACCUMULATOR;
        radius = 100;
        angle = 0.0;
    }

    ~Accumulator() override { Hide(); }

    virtual void Show();
    virtual void Hide();
    virtual void MoveTo();

    void Charge(float amount);
    void Discharge(float amount);
    void SetCharging(bool charging) { isCharging = charging; }
    int GetCurrentLevel() { return batteryCharge; }
};

class Key : public FigureBase
{
public:
    Key(int InitX, int InitY);
    ~Key() { Hide(); }
    virtual void Show();
    virtual void Hide();
};