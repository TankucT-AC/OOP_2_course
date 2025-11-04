#include "CollisionWrapper.h"
#include <algorithm>

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

// Инициализация статического вектора
std::vector<CollisionWrapper*> CollisionWrapper::allObjects;

CollisionWrapper::CollisionWrapper(int InitX, int InitY, int colWidth, int colHeight)
    : Point(InitX, InitY), collisionWidth(colWidth), collisionHeight(colHeight)
{
    allObjects.push_back(this);
}

CollisionWrapper::~CollisionWrapper() {
    auto it = std::find(allObjects.begin(), allObjects.end(), this);
    if (it != allObjects.end()) {
        allObjects.erase(it);
    }
}

void CollisionWrapper::MoveTo(int NewX, int NewY) {
    if (!WouldCollide(NewX, NewY)) {
        Hide();
        X = NewX;
        Y = NewY;
        Show();
    }
}

void CollisionWrapper::Drag(int Step) {
    int FigX = GetX(), FigY = GetY();

    while (true) {
        if (KEY_DOWN(VK_SPACE)) {
            Sleep(500);
            break;
        }

        int newX = FigX;
        int newY = FigY;

        if (KEY_DOWN(VK_LEFT)) {
            newX -= Step;
        }
        if (KEY_DOWN(VK_RIGHT)) {
            newX += Step;
        }
        if (KEY_DOWN(VK_UP)) {
            newY -= Step;
        }
        if (KEY_DOWN(VK_DOWN)) {
            newY += Step;
        }

        if (!WouldCollide(newX, newY)) {
            FigX = newX;
            FigY = newY;
            MoveTo(FigX, FigY);
        }
        else {
            Trigger();
            DrawCollisionBox();
        }

        Sleep(50);
    }
}

bool CollisionWrapper::CheckCollision(int testX, int testY) const {
    return (testX >= GetCollisionLeft() && testX <= GetCollisionRight() &&
        testY >= GetCollisionTop() && testY <= GetCollisionBottom());
}

bool CollisionWrapper::CheckCollision(const CollisionWrapper& other) const {
    return (GetCollisionLeft() <= other.GetCollisionRight() &&
        GetCollisionRight() >= other.GetCollisionLeft() &&
        GetCollisionTop() <= other.GetCollisionBottom() &&
        GetCollisionBottom() >= other.GetCollisionTop());
}

bool CollisionWrapper::WouldCollide(int newX, int newY) const {
    int tempLeft = newX - collisionWidth / 2;
    int tempRight = newX + collisionWidth / 2;
    int tempTop = newY - collisionHeight / 2;
    int tempBottom = newY + collisionHeight / 2;

    for (const auto& obj : allObjects) {
        if (obj != this) {
            if (tempLeft <= obj->GetCollisionRight() &&
                tempRight >= obj->GetCollisionLeft() &&
                tempTop <= obj->GetCollisionBottom() &&
                tempBottom >= obj->GetCollisionTop()) {
                return true;
            }
        }
    }
    return false;
}

void CollisionWrapper::DrawCollisionBox() const {
    HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    SelectObject(hdc, pen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Rectangle(hdc, GetCollisionLeft(), GetCollisionTop(),
        GetCollisionRight(), GetCollisionBottom());

    DeleteObject(pen);
}

bool CollisionWrapper::CheckGlobalCollision(const CollisionWrapper& obj, int newX, int newY) {
    return obj.WouldCollide(newX, newY);
}

void CollisionWrapper::DrawAllCollisionBoxes() {
    for (const auto& obj : allObjects) {
        obj->DrawCollisionBox();
    }
}