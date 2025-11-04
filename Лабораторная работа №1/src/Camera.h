#pragma once
#include "CollisionWrapper.h"
#include <string>

// Теперь Camera наследуется от CollisionWrapper
class Camera : public CollisionWrapper {
protected:
    std::string model;
    double resolution;
    double zoomLevel;

public:
    Camera(int InitX, int InitY, const std::string& modelName, double res,
        int colWidth = 80, int colHeight = 60);
    virtual ~Camera();

    virtual void Show() override;
    virtual void Hide() override;
    virtual void TakePhoto();
    virtual void Zoom(double factor);
    virtual void Trigger() override { TakePhoto(); }

    void SetZoom(double zoom);
    double GetZoom() const;
    virtual void DrawCollisionBox() const override;
};

// Смартфон камера
class SmartphoneCamera : public Camera {
private:
    bool nightMode;
    bool hdrMode;

public:
    SmartphoneCamera(int InitX, int InitY, const std::string& modelName, double res);
    ~SmartphoneCamera();

    void Show() override;
    void Hide() override;
    void TakePhoto() override;
    void Zoom(double factor) override;

    void ToggleNightMode();
    void ToggleHDRMode();
};

// DSLR камера
class DSLRCamera : public Camera {
private:
    bool manualMode;
    double aperture;

public:
    DSLRCamera(int InitX, int InitY, const std::string& modelName, double res, double aperture);
    ~DSLRCamera();

    void Show() override;
    void Hide() override;
    void TakePhoto() override;

    void SetAperture(double aperture);
    void ToggleManualMode();
};

// Action камера
class ActionCamera : public Camera {
private:
    bool waterproof;
    bool wideAngle;

public:
    ActionCamera(int InitX, int InitY, const std::string& modelName, double res);
    ~ActionCamera();

    void Show() override;
    void Hide() override;
    void TakePhoto() override;

    void ToggleWideAngle();
};