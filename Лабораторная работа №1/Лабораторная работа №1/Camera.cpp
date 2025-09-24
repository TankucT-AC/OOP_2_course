#include "Camera.h"
#include <windows.h>

extern HDC hdc;

// Конструкторы
Camera::Camera(int InitX, int InitY, int InitRad)
    : Circle(InitX, InitY, InitRad),
    focusPoint(InitX, InitY + InitRad * 2),
    isActive(false),
    isRecording(false) {
}

Camera::Camera(Point position, int InitRad)
    : Circle(position.GetX(), position.GetY(), InitRad),
    focusPoint(position.GetX(), position.GetY() + InitRad * 2),
    isActive(false),
    isRecording(false) {
}

// Деструктор
Camera::~Camera() {}

// Установить точку фокусировки
void Camera::SetFocus(int focusX, int focusY) {
    focusPoint.MoveTo(focusX, focusY);
}

void Camera::SetFocus(Point point) {
    focusPoint.MoveTo(point.GetX(), point.GetY());
}

// Управление записью
void Camera::StartRecording() {
    isRecording = true;
    if (visible) {
        Hide();
        Show();
    }
}

void Camera::StopRecording() {
    isRecording = false;
    if (visible) {
        Hide();
        Show();
    }
}

void Camera::ToggleActive() {
    isActive = !isActive;
    if (visible) {
        Hide();
        Show();
    }
}

// Переопределенные методы отрисовки
void Camera::Show() {
    visible = true;

    // Рисуем основное тело камеры (серый круг)
    HPEN cameraPen = CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
    HBRUSH cameraBrush = CreateSolidBrush(RGB(200, 200, 200));

    HGDIOBJ oldPen = SelectObject(hdc, cameraPen);
    HGDIOBJ oldBrush = SelectObject(hdc, cameraBrush);

    Ellipse(hdc, X - radius, Y - radius, X + radius, Y + radius);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(cameraPen);
    DeleteObject(cameraBrush);

    // Рисуем дополнительные элементы
    DrawLens();
    DrawStatus();

    // Рисуем линию фокусировки
    HPEN focusPen = CreatePen(PS_DOT, 1, RGB(0, 0, 255));
    SelectObject(hdc, focusPen);

    MoveToEx(hdc, X, Y, NULL);
    LineTo(hdc, focusPoint.GetX(), focusPoint.GetY());

    SelectObject(hdc, oldPen);
    DeleteObject(focusPen);

    // Показываем точку фокусировки
    focusPoint.Show();
}

void Camera::Hide() {
    visible = false;

    // Сначала скрываем точку фокусировки
    focusPoint.Hide();

    // Скрываем основную камеру (рисуем белым)
    HPEN clearPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HBRUSH clearBrush = CreateSolidBrush(RGB(255, 255, 255));

    HGDIOBJ oldPen = SelectObject(hdc, clearPen);
    HGDIOBJ oldBrush = SelectObject(hdc, clearBrush);

    Ellipse(hdc, X - radius, Y - radius, X + radius, Y + radius);

    // Стираем линию фокусировки
    MoveToEx(hdc, X, Y, NULL);
    LineTo(hdc, focusPoint.GetX(), focusPoint.GetY());

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(clearPen);
    DeleteObject(clearBrush);
}

void Camera::MoveTo(int NewX, int NewY) {
    Hide();
    X = NewX;
    Y = NewY;
    // Точка фокусировки перемещается относительно камеры
    focusPoint.MoveTo(NewX, NewY + radius * 2);
    Show();
}

// Вспомогательные методы отрисовки
void Camera::DrawLens() {
    // Рисуем объектив (черный круг поменьше)
    HPEN lensPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH lensBrush = CreateSolidBrush(RGB(50, 50, 50));

    HGDIOBJ oldPen = SelectObject(hdc, lensPen);
    HGDIOBJ oldBrush = SelectObject(hdc, lensBrush);

    int lensRadius = radius / 2;
    Ellipse(hdc, X - lensRadius, Y - lensRadius, X + lensRadius, Y + lensRadius);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(lensPen);
    DeleteObject(lensBrush);
}

void Camera::DrawStatus() {
    // Рисуем индикатор статуса
    COLORREF statusColor;
    if (isRecording) {
        statusColor = RGB(255, 0, 0);       // Красный - запись
    }
    else if (isActive) {
        statusColor = RGB(0, 255, 0);       // Зеленый - активна
    }
    else {
        statusColor = RGB(100, 100, 100);   // Серый - неактивна
    }

    HPEN statusPen = CreatePen(PS_SOLID, 1, statusColor);
    HBRUSH statusBrush = CreateSolidBrush(statusColor);

    HGDIOBJ oldPen = SelectObject(hdc, statusPen);
    HGDIOBJ oldBrush = SelectObject(hdc, statusBrush);

    int statusSize = radius / 3;
    Ellipse(hdc, X + radius - statusSize, Y - radius + statusSize,
        X + radius, Y - radius + statusSize * 2);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(statusPen);
    DeleteObject(statusBrush);
}

// Геттеры
Point Camera::GetFocusPoint() const {
    return focusPoint;
}

bool Camera::IsActive() const {
    return isActive;
}

bool Camera::IsRecording() const {
    return isRecording;
}