#pragma once
#include "Point.h"

class Camera : public Circle {
private:
    Point focusPoint;       // Точка фокусировки камеры
    bool isActive;          // Активна ли камера
    bool isRecording;       // Идет ли запись

public:
    // Конструкторы
    Camera(int InitX, int InitY, int InitRad = 20);
    Camera(Point position, int InitRad = 20);

    // Деструктор
    ~Camera();

    // Методы управления камерой
    void SetFocus(int focusX, int focusY);      // Установить точку фокусировки
    void SetFocus(Point point);          // Установить точку фокусировки
    void StartRecording();                      // Начать "запись"
    void StopRecording();                       // Остановить "запись"
    void ToggleActive();                        // Переключить активность

    // Переопределенные методы
    void Show();                       // Показать камеру
    void Hide();                       // Скрыть камеру
    void MoveTo(int NewX, int NewY);   // Переместить камеру

    // Геттеры
    Point GetFocusPoint() const;
    bool IsActive() const;
    bool IsRecording() const;

private:
    void DrawLens();        // Нарисовать объектив
    void DrawStatus();      // Нарисовать индикатор статуса
};