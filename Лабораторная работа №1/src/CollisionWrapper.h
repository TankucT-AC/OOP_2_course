#pragma once
#include "Point.h"
#include <vector>

class CollisionWrapper : public Point {
protected:
    int collisionWidth;
    int collisionHeight;
    static std::vector<CollisionWrapper*> allObjects;

public:
    CollisionWrapper(int InitX, int InitY, int colWidth = 40, int colHeight = 30);
    virtual ~CollisionWrapper();

    virtual void MoveTo(int NewX, int NewY) override;
    virtual void Drag(int Step) override;

    // Коллизия
    bool CheckCollision(int testX, int testY) const;
    bool CheckCollision(const CollisionWrapper& other) const;
    bool WouldCollide(int newX, int newY) const;
    virtual void DrawCollisionBox() const;
    virtual void Trigger() {};

    // Геттеры для размеров коллизии
    int GetCollisionLeft() const { return X - collisionWidth / 2; }
    int GetCollisionRight() const { return X + collisionWidth / 2; }
    int GetCollisionTop() const { return Y - collisionHeight / 2; }
    int GetCollisionBottom() const { return Y + collisionHeight / 2; }

    int GetCollisionWidth() const { return collisionWidth; }
    int GetCollisionHeight() const { return collisionHeight; }

    // Статические методы для управления всеми объектами
    static bool CheckGlobalCollision(const CollisionWrapper& obj, int newX, int newY);
    static void DrawAllCollisionBoxes();
};