#include <cmath>
#include "Figures.h"

// Реализация для класса Square
void Oil::Show()
{
    visible = true;

    // Получаем координаты через методы интерфейса
    int x = GetX();
    int y = GetY();

    //// Рисуем квадрат синим цветом
    //HPEN squarePen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    //HBRUSH squareBrush = CreateSolidBrush(RGB(100, 100, 255));
    //SelectObject(memDC, squarePen);
    //SelectObject(memDC, squareBrush);

    //// Рисуем квадрат с центром в (x, y) и заданной длиной стороны
    //Rectangle(memDC, x - len / 2, y - len / 2, x + len / 2, y + len / 2);

    //DeleteObject(squarePen);
    //DeleteObject(squareBrush);

    DrawImageOil(memDC, L"oil.jpg", x, y, len, len);
}

void Oil::Hide()
{
    if (!visible) return;

    visible = false;

    // Получаем координаты через методы интерфейса
    int x = GetX();
    int y = GetY();

    // Стираем квадрат, рисуя черный прямоугольник
    HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(memDC, blackPen);
    SelectObject(memDC, blackBrush);

    Rectangle(memDC, x - len / 2 - 1, y - len / 2 - 1, x + len / 2 + 1, y + len / 2 + 1);

    DeleteObject(blackPen);
    DeleteObject(blackBrush);
}

void Oil::MoveTo(Camera* cam)
{
    int camX = cam->GetX();
    int camY = cam->GetY();
    
    int vecX = camX - X;
    int vecY = camY - Y;
    
    int newX = X + speed * vecX;
    int newY = Y + speed * vecY;

    Hide();
    X = newX;
    Y = newY;
    Show();
}

// Реализация для класса Cross
void Cross::Show()
{
    visible = true;

    // Получаем координаты через методы интерфейса
    int x = GetX();
    int y = GetY();

    // Рисуем крест красным цветом
    HPEN crossPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
    SelectObject(memDC, crossPen);

    // Вертикальная линия креста
    MoveToEx(memDC, x, y - size / 2, NULL);
    LineTo(memDC, x, y + size / 2);

    // Горизонтальная линия креста
    MoveToEx(memDC, x - size / 2, y, NULL);
    LineTo(memDC, x + size / 2, y);

    DeleteObject(crossPen);
}

void Cross::Hide()
{
    if (!visible) return;

    visible = false;

    // Получаем координаты через методы интерфейса
    int x = GetX();
    int y = GetY();

    // Стираем крест, рисуя черные линии поверх
    HPEN blackPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
    SelectObject(memDC, blackPen);

    // Вертикальная линия
    MoveToEx(memDC, x, y - size / 2 - 1, NULL);
    LineTo(memDC, x, y + size / 2 + 1);

    // Горизонтальная линия
    MoveToEx(memDC, x - size / 2 - 1, y, NULL);
    LineTo(memDC, x + size / 2 + 1, y);

    DeleteObject(blackPen);
}

void Cross::MoveTo()
{
    Hide();
    int newY = GetY() + d;
    if (!(floor <= newY && newY <= ceil))
    {
        d *= -1;
       // return;
    }
    Y = newY;
    Show();
}

// Реализация для класса Accumulator
void Accumulator::Show()
{
    visible = true;

    // Получаем координаты через методы интерфейса
    int x = GetX();
    int y = GetY();

    // Корпус аккумулятора
    HPEN bodyPen = CreatePen(PS_SOLID, 2, RGB(50, 50, 50));
    HBRUSH bodyBrush = CreateSolidBrush(RGB(200, 200, 200));
    SelectObject(memDC, bodyPen);
    SelectObject(memDC, bodyBrush);

    // Основной прямоугольник аккумулятора
    Rectangle(memDC, x - 30, y - 50, x + 30, y + 50);

    // Верхний контакт
    HBRUSH contactBrush = CreateSolidBrush(RGB(100, 100, 100));
    SelectObject(memDC, contactBrush);
    Rectangle(memDC, x - 10, y - 60, x + 10, y - 50);

    // Уровень заряда
    COLORREF chargeColor;
    if (batteryCharge >= 80) {
        chargeColor = RGB(0, 255, 0); // Зеленый при зарядке
    }
    else if (batteryCharge > 25) {
        chargeColor = RGB(255, 255, 0); // Желтый
    }
    else {
        chargeColor = RGB(255, 0, 0); // Красный при низком заряде
    }

    HBRUSH chargeBrush = CreateSolidBrush(chargeColor);
    SelectObject(memDC, chargeBrush);

    // Вычисляем высоту уровня заряда
    int chargeHeight = (batteryCharge * 90) / 100;
    int chargeTop = y + 40 - chargeHeight;

    // Индикатор уровня заряда
    Rectangle(memDC, x - 20, chargeTop, x + 20, y + 40);

    DeleteObject(bodyPen);
    DeleteObject(bodyBrush);
    DeleteObject(contactBrush);
    DeleteObject(chargeBrush);
}

void Accumulator::Hide()
{
    if (!visible) return;

    visible = false;

    // Получаем координаты через методы интерфейса
    int x = GetX();
    int y = GetY();

    // Стираем аккумулятор черным прямоугольником
    HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(memDC, blackPen);
    SelectObject(memDC, blackBrush);

    Rectangle(memDC, x - 35, y - 65, x + 35, y + 55);

    DeleteObject(blackPen);
    DeleteObject(blackBrush);
}

void Accumulator::Charge(float amount)
{
    if (isCharging) {
        batteryCharge += amount;
        if (batteryCharge > capacity) {
            batteryCharge = capacity;
        }
        if (visible) {
            Show();
        }
    }
}

void Accumulator::Discharge(float amount)
{
    batteryCharge -= amount;
    if (batteryCharge < 0) {
        batteryCharge = 0;
    }
    if (visible) {
        Show();
    }
}

void Accumulator::MoveTo()
{
    int newX = centerX + static_cast<int>(radius * std::cos(angle));
    int newY = centerY + static_cast<int>(radius * std::sin(angle));

    Hide();
    X = newX;
    Y = newY;
    Show();
    angle += 0.05;
}

Key::Key(int InitX, int InitY) : FigureBase(InitX, InitY) { id = FIGURE_KEY; }

void Key::Show() 
{
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 0));
    SelectObject(memDC, hBrush);

    // 1. Прямоугольная головка со скруглением
    RoundRect(memDC, X, Y, X + 35, Y + 30, 10, 10);

    // 2. Узкий стержень
    Rectangle(memDC, X + 35, Y + 10, X + 110, Y + 20);

    // 3. Зазубрины (рисуем треугольниками или мелкими линиями)
    // Для простоты — серия маленьких выступов разной высоты
    for (int i = 0; i < 5; i++) 
    {
        Rectangle(memDC, X + 50 + (i * 10), Y + 20, X + 55 + (i * 10), Y + 23 + (i % 3 * 3));
    }

    DeleteObject(hBrush);
}

void Key::Hide()
{
    if (!visible) return;
    visible = false;

    // 1. Временно отключаем ограничение дыркой камеры
    SelectClipRgn(memDC, NULL);

    // 2. Стандартная затирка
    HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    HGDIOBJ oldPen = SelectObject(memDC, blackPen);
    HGDIOBJ oldBrush = SelectObject(memDC, blackBrush);

    Rectangle(memDC, X - 2, Y - 2, X + 115, Y + 35);

    // 3. Чистим ресурсы
    SelectObject(memDC, oldPen);
    SelectObject(memDC, oldBrush);
    DeleteObject(blackPen);
    DeleteObject(blackBrush);
}