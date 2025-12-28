#pragma once

#include "Camera.h"

class GameBoard : public Point 
{
private:
    Camera* currentCamera;
    int colWidth;
    int colHeight;
    int keysCollected;

    // Вложенный класс двери
    class Door {
    public:
        int dx, dy, width, height;
        bool isOpen;

        Door() : dx(0), dy(0), width(60), height(100), isOpen(false) {}

        void SetPosition(int x, int y) { dx = x; dy = y; }
        void Open() { isOpen = true; }
        void Close() { isOpen = false; }
        bool IsOpen() { return isOpen; }
    };

    // Вложенный класс шкалы заряда заряда
    class Charge {
    private:
        float level; // Текущий заряд в % (0.0 - 100.0)
    public:
        Charge() : level(100.0f) {}

        void Decrease(float amount) {
            level -= amount;
            if (level < 0) level = 0;
        }
        void Crease(float amount)
        {
            level += amount;
            if (level > 100) level = 100;
        }

        bool IsEmpty() const { return level <= 0; }
        float GetLevel() const { return level; }

        COLORREF GetColor() const {
            if (level >= 80) return RGB(0, 255, 0);   // Зеленый
            if (level >= 25) return RGB(255, 255, 0); // Желтый
            return RGB(255, 0, 0);                    // Красный
        }
    };

    Door exitDoor;  // Экземпляр двери
    Charge battery; // Шкала заряда

    // Отрисовка ключей для шкалы
    void DrawKeyIcon(int x, int y, float scale);
public:
    GameBoard(int InitX, int InitY, int InitColWidth, int InitColHeight, Camera* cam);
    ~GameBoard();
    void Show(Camera* cam);
    void OpenExit() { exitDoor.Open(); }
    void CloseExit() { exitDoor.Close(); }
    bool IsExitOpen() { return exitDoor.IsOpen(); }
    bool IsDischarge() { return battery.IsEmpty(); }
    void Decrease(float amount) { battery.Decrease(amount); }
    void Crease(float amount) { battery.Crease(amount); }
    void AddKey() { keysCollected++; };
    void DrawCameraRim(Camera* cam);

    // Геттеры
    int GetKeysCount() { return keysCollected; }
    int GetColWidth() { return colWidth; }
    int GetColHeight() { return colHeight; }
    int GetDoorX() { return exitDoor.dx; }
    int GetDoorY() { return exitDoor.dy; }
    int GetDoorWidth() { return exitDoor.width; }
    int GetDoorHeight() { return exitDoor.height; }

    // Финальный вывод
    void GameOver();
    void YouWin();
};
