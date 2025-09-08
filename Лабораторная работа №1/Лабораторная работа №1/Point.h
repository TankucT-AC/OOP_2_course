#pragma once

// В файле Point.h будут находится макеты классов для рисования

// Общий для всех объектов класс
class Location
{
protected:
	int X;
	int Y;
public:
	Location(int InitX, int InitY);
	~Location();

	int GetX();
	int GetY();

	void SetX(int NewX);
	void SetY(int NewY);
};

// Класс для точки
class Point : public Location
{
private:
	bool visible;
public:
	Point(int InitX, int InitY);
	~Point();

	void Show();
	void Hide();
	void MoveTo(int NewX, int NewY);
};
