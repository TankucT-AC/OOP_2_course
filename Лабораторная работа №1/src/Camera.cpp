#include "Camera.h"
#include <cmath>

Camera::Camera(int InitX, int InitY, const std::string& modelName, double res,
    int colWidth, int colHeight)
    : CollisionWrapper(InitX, InitY, colWidth, colHeight),
    model(modelName), resolution(res), zoomLevel(1.0) {
}

Camera::~Camera() {}

void Camera::Show() {
    visible = true;

    // Корпус
    HPEN bodyPen = CreatePen(PS_SOLID, 3, RGB(60, 60, 60));
    HBRUSH bodyBrush = CreateSolidBrush(RGB(80, 80, 80));
    SelectObject(hdc, bodyPen);
    SelectObject(hdc, bodyBrush);
    Rectangle(hdc, X - 35, Y - 25, X + 35, Y + 25);

    // Объектив
    HBRUSH lensBrush = CreateSolidBrush(RGB(30, 30, 30));
    SelectObject(hdc, lensBrush);
    Ellipse(hdc, X + 15, Y - 15, X + 35, Y + 15);

    DeleteObject(bodyPen);
    DeleteObject(bodyBrush);
    DeleteObject(lensBrush);
}

void Camera::Hide() {
    if (!visible) return;

    visible = false;

    // Заливаем область камеры черным цветом
    HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, blackPen);
    SelectObject(hdc, blackBrush);

    // Заливаем прямоугольник, покрывающий всю камеру + вспышку
    Rectangle(hdc, X - 45, Y - 35, X + 85, Y + 35);

    DeleteObject(blackPen);
    DeleteObject(blackBrush);
}

void Camera::TakePhoto() {
    // Звездообразная вспышка
    HPEN flashPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 200));
    HBRUSH flashBrush = CreateSolidBrush(RGB(255, 255, 180));
    SelectObject(hdc, flashPen);
    SelectObject(hdc, flashBrush);

    // Большой круг вспышки
    Ellipse(hdc, X + 40, Y - 30, X + 80, Y + 30);

    // Лучи звезды
    POINT starPoints[10] = {
        {X + 80, Y}, {X + 100, Y - 10}, {X + 95, Y}, {X + 100, Y + 10}, {X + 80, Y},
        {X + 80, Y + 20}, {X + 75, Y}, {X + 80, Y - 20}, {X + 100, Y - 10}, {X + 80, Y}
    };
    Polygon(hdc, starPoints, 10);

    Sleep(200);

    // Убираем вспышку
    HPEN clearPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
    HBRUSH clearBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, clearPen);
    SelectObject(hdc, clearBrush);
    Ellipse(hdc, X + 30, Y - 40, X + 110, Y + 40);

    DeleteObject(flashPen);
    DeleteObject(flashBrush);
    DeleteObject(clearPen);
    DeleteObject(clearBrush);
}

void Camera::Zoom(double factor) {
    double newZoom = zoomLevel * factor;
    if (newZoom < 1.0) newZoom = 1.0;
    if (newZoom > 10.0) newZoom = 10.0;
    zoomLevel = newZoom;
}

void Camera::SetZoom(double zoom) {
    zoomLevel = zoom;
}

double Camera::GetZoom() const {
    return zoomLevel;
}

void Camera::DrawCollisionBox() const {
    // Зеленая рамка для камер
    HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    SelectObject(hdc, pen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Rectangle(hdc, GetCollisionLeft(), GetCollisionTop(),
        GetCollisionRight(), GetCollisionBottom());

    DeleteObject(pen);
}

// Реализация SmartphoneCamera
SmartphoneCamera::SmartphoneCamera(int InitX, int InitY, const std::string& modelName, double res)
    : Camera(InitX, InitY, modelName, res, 70, 90), nightMode(false), hdrMode(false) {
}

SmartphoneCamera::~SmartphoneCamera() {}

void SmartphoneCamera::Show() {
    visible = true;

    // Корпус смартфона
    HPEN bodyPen = CreatePen(PS_SOLID, 3, RGB(200, 200, 200));
    HBRUSH bodyBrush = CreateSolidBrush(RGB(220, 220, 220));
    SelectObject(hdc, bodyPen);
    SelectObject(hdc, bodyBrush);
    RoundRect(hdc, X - 25, Y - 35, X + 25, Y + 35, 10, 10);

    // Несколько камер как у реальных смартфонов
    HBRUSH cameraBrush = CreateSolidBrush(RGB(50, 50, 50));
    SelectObject(hdc, cameraBrush);
    Ellipse(hdc, X + 10, Y - 25, X + 20, Y - 15);
    Ellipse(hdc, X + 10, Y - 10, X + 18, Y - 2);
    Ellipse(hdc, X + 10, Y + 5, X + 18, Y + 13);

    DeleteObject(bodyPen);
    DeleteObject(bodyBrush);
    DeleteObject(cameraBrush);
}

void SmartphoneCamera::Hide() {
    if (!visible) return;

    visible = false;

    HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, blackPen);
    SelectObject(hdc, blackBrush);

    // Заливаем область смартфона + вспышку
    Rectangle(hdc, X - 35, Y - 45, X + 75, Y + 45);

    DeleteObject(blackPen);
    DeleteObject(blackBrush);
}

void SmartphoneCamera::TakePhoto() {
    // Квадратная вспышка с лучами
    HPEN flashPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 220));
    HBRUSH flashBrush = CreateSolidBrush(RGB(255, 255, 200));
    SelectObject(hdc, flashPen);
    SelectObject(hdc, flashBrush);

    Rectangle(hdc, X + 30, Y - 35, X + 70, Y + 35);

    // Добавляем лучи
    for (int i = 0; i < 4; i++) {
        Rectangle(hdc, X + 70 + i * 8, Y - 25 + i * 12, X + 90 + i * 8, Y - 15 + i * 12);
    }

    Sleep(200);

    // Очистка
    HPEN clearPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
    HBRUSH clearBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, clearPen);
    SelectObject(hdc, clearBrush);
    Rectangle(hdc, X + 25, Y - 40, X + 120, Y + 40);

    DeleteObject(flashPen);
    DeleteObject(flashBrush);
    DeleteObject(clearPen);
    DeleteObject(clearBrush);
}

void SmartphoneCamera::Zoom(double factor) {
    double newZoom = zoomLevel * factor;
    if (newZoom <= 5.0) {
        zoomLevel = newZoom;
    }
}

void SmartphoneCamera::ToggleNightMode() {
    nightMode = !nightMode;
}

void SmartphoneCamera::ToggleHDRMode() {
    hdrMode = !hdrMode;
}

// Реализация DSLRCamera
DSLRCamera::DSLRCamera(int InitX, int InitY, const std::string& modelName, double res, double apert)
    : Camera(InitX, InitY, modelName, res, 100, 80), manualMode(false), aperture(apert) {
}

DSLRCamera::~DSLRCamera() {}

void DSLRCamera::Show() {
    visible = true;

    // Корпус DSLR
    HPEN bodyPen = CreatePen(PS_SOLID, 3, RGB(40, 40, 40));
    HBRUSH bodyBrush = CreateSolidBrush(RGB(60, 60, 60));
    SelectObject(hdc, bodyPen);
    SelectObject(hdc, bodyBrush);
    Rectangle(hdc, X - 40, Y - 30, X + 40, Y + 30);

    // Добавил профессиональные детали
    HBRUSH gripBrush = CreateSolidBrush(RGB(30, 30, 30));
    SelectObject(hdc, gripBrush);
    Rectangle(hdc, X - 45, Y - 15, X - 35, Y + 25);

    // Большой объектив
    HBRUSH lensBrush = CreateSolidBrush(RGB(20, 20, 20));
    SelectObject(hdc, lensBrush);
    Ellipse(hdc, X + 25, Y - 25, X + 55, Y + 25);

    DeleteObject(bodyPen);
    DeleteObject(bodyBrush);
    DeleteObject(gripBrush);
    DeleteObject(lensBrush);
}

void DSLRCamera::Hide() {
    if (!visible) return;

    visible = false;

    HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, blackPen);
    SelectObject(hdc, blackBrush);

    // Заливаем большую область DSLR камеры
    Rectangle(hdc, X - 55, Y - 40, X + 95, Y + 40);

    DeleteObject(blackPen);
    DeleteObject(blackBrush);
}

void DSLRCamera::TakePhoto() {
    // Концентрические круги вспышки
    HPEN flashPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 180));
    SelectObject(hdc, flashPen);

    for (int i = 0; i < 4; i++) {
        int radius = 20 + i * 12;
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Ellipse(hdc, X + 60 - radius, Y - radius, X + 60 + radius, Y + radius);
    }

    HBRUSH flashBrush = CreateSolidBrush(RGB(255, 255, 150));
    SelectObject(hdc, flashBrush);
    Ellipse(hdc, X + 50, Y - 20, X + 70, Y + 20);

    Sleep(200);

    // Очистка
    HPEN clearPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HBRUSH clearBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, clearPen);
    SelectObject(hdc, clearBrush);
    Ellipse(hdc, X + 30, Y - 50, X + 90, Y + 50);

    DeleteObject(flashPen);
    DeleteObject(flashBrush);
    DeleteObject(clearPen);
    DeleteObject(clearBrush);
}

void DSLRCamera::SetAperture(double apert) {
    aperture = apert;
}

void DSLRCamera::ToggleManualMode() {
    manualMode = !manualMode;
}

// Реализация ActionCamera
ActionCamera::ActionCamera(int InitX, int InitY, const std::string& modelName, double res)
    : Camera(InitX, InitY, modelName, res, 80, 70), waterproof(true), wideAngle(true) {
}

ActionCamera::~ActionCamera() {}

void ActionCamera::Show() {
    visible = true;
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 165, 0));
    HBRUSH brush = CreateSolidBrush(RGB(60, 60, 60));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);

    Rectangle(hdc, X - 18, Y - 18, X + 18, Y + 18);

    HBRUSH lensBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, lensBrush);

    if (wideAngle) {
        // Более широкий объектив для широкоугольного режима
        Ellipse(hdc, X + 5, Y - 12, X + 16, Y + 12);
    }
    else {
        Ellipse(hdc, X + 8, Y - 10, X + 16, Y + 10);
    }

    DeleteObject(pen);
    DeleteObject(brush);
    DeleteObject(lensBrush);
}

void ActionCamera::Hide() {
    if (!visible) return;

    visible = false;

    HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, blackPen);
    SelectObject(hdc, blackBrush);

    // Заливаем область экшн-камеры
    Rectangle(hdc, X - 40, Y - 35, X + 80, Y + 35);

    DeleteObject(blackPen);
    DeleteObject(blackBrush);
}

void ActionCamera::TakePhoto() {
    // Динамическая вспышка для экшн-камеры
    HPEN flashPen = CreatePen(PS_SOLID, 3, RGB(200, 200, 255));
    SelectObject(hdc, flashPen);

    // Пульсирующий эффект
    for (int i = 0; i < 2; i++) {
        Ellipse(hdc, X + 12, Y - 8, X + 22, Y + 8);
        Sleep(80);
        HPEN clearPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
        SelectObject(hdc, clearPen);
        Ellipse(hdc, X + 10, Y - 10, X + 24, Y + 10);
        SelectObject(hdc, flashPen);
    }

    DeleteObject(flashPen);
}

void ActionCamera::ToggleWideAngle() {
    wideAngle = !wideAngle;
    Hide();
    Show(); // Перерисовываем с новыми параметрами
}