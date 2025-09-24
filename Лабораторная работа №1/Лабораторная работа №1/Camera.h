#pragma once
#include "Point.h"

class Camera : public Circle {
private:
    Point focusPoint;       // ����� ����������� ������
    bool isActive;          // ������� �� ������
    bool isRecording;       // ���� �� ������

public:
    // ������������
    Camera(int InitX, int InitY, int InitRad = 20);
    Camera(Point position, int InitRad = 20);

    // ����������
    ~Camera();

    // ������ ���������� �������
    void SetFocus(int focusX, int focusY);      // ���������� ����� �����������
    void SetFocus(Point point);          // ���������� ����� �����������
    void StartRecording();                      // ������ "������"
    void StopRecording();                       // ���������� "������"
    void ToggleActive();                        // ����������� ����������

    // ���������������� ������
    void Show();                       // �������� ������
    void Hide();                       // ������ ������
    void MoveTo(int NewX, int NewY);   // ����������� ������

    // �������
    Point GetFocusPoint() const;
    bool IsActive() const;
    bool IsRecording() const;

private:
    void DrawLens();        // ���������� ��������
    void DrawStatus();      // ���������� ��������� �������
};