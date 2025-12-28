#pragma once

#include "Point.h"
#include <string>

// Индексы для камер
enum CameraType {
    CAMERA_BASE = 0,
    CAMERA_SMARTPHONE,
    CAMERA_DSLR,
    CAMERA_ACTION,
    CAMERA_NEW
};

// Интерфейс камеры
class ICamera
{
public:
    virtual void Frame() = 0;             // Корпус
    virtual void Lens() = 0;              // Линзы и объективы
    virtual void AuxiliaryElements() = 0; // Дополнительные детали
    virtual float DisCharge() = 0;
};

// Класс камеры
class Camera : public Point, public ICamera
{
protected:
    std::string model;
    int radius;
    float amount;
    double zoomLevel;
    int colWidth;
    int colHeight;
    bool isFlashing; // Добавляем флаг вспышки
    CameraType id;

public:
    Camera(int InitX, int InitY, const std::string& modelName, int InitColWidth = 80, int InitColHeight = 60, int InitRad = 100);
    virtual ~Camera();

    virtual void Frame();
    virtual void Lens();
    virtual void AuxiliaryElements();

    virtual float DisCharge();
     void Show();
    virtual void Hide();
    virtual void TakePhoto();
    virtual void ClearFlash();

    // Геттеры
    int GetColisionWidth() { return colWidth; }
    int GetColisionHeight() { return colHeight; }
    int GetRadius() { return radius; }
    std::string GetModel() { return model; }
    bool GetIsFlashing() { return isFlashing; }

    CameraType GetCameraType() { return id; }
};

// Смартфон камера
class SmartphoneCamera : public Camera
{
private:
    bool nightMode;
    bool hdrMode;

public:
    SmartphoneCamera(int InitX, int InitY, const std::string& modelName);
    ~SmartphoneCamera();

    virtual void Frame();
    virtual void Lens();
    virtual void AuxiliaryElements();

    void Hide();
    void TakePhoto();
    void ClearFlash();
};

// DSLR камера
class DSLRCamera : public Camera
{
private:
    bool manualMode;

public:
    DSLRCamera(int InitX, int InitY, const std::string& modelName);
    ~DSLRCamera();

    virtual void Frame();
    virtual void Lens();
    virtual void AuxiliaryElements();

    void Hide();
    void TakePhoto();
    void ClearFlash();
};

// Action камера
class ActionCamera : public Camera
{
private:
    bool waterproof;
    bool wideAngle;

public:
    ActionCamera(int InitX, int InitY, const std::string& modelName, double res);
    ~ActionCamera();

    virtual void Frame();
    virtual void Lens();
    virtual void AuxiliaryElements();

    void Hide();
    void TakePhoto();
    void ClearFlash();
};
