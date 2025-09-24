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
protected:
	bool visible;
public:
	Point(int InitX, int InitY);
	~Point();

	void Show();
	void Hide();
	void MoveTo(int NewX, int NewY);

	int GetX();
	int GetY();

	void SetX(int NewX);
	void SetY(int NewY);
};

class Circle : public Point
{
protected:
	int radius;
public:
	Circle(int InitX, int InitY, int InitRad);
	~Circle();

	void Show();
	void Hide();
	//void MoveTo(int NewX, int NewY);
	void EditRadius(int NewRadius);
};

// МОЯ ФИГУРА: ФОТОАППАРАТ (ПОЛЬЗОВАТЕЛЬСКИЙ И ШПИОНСКИЙ)