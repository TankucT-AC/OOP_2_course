#pragma once
#include <string>

// Класс для представления воздушного судна
class Airplane 
{
public:
    std::string airportName;     // Название аэропорта
    std::string airline;         // Авиакомпания
    std::string planeModel;      // Марка самолета
    std::string flightNumber;    // Номер рейса
    std::string departureTime;   // Время вылета
    std::string destination;     // Пункт прибытия

    Airplane* prev;              // Указатель на предыдущий элемент
    Airplane* next;              // Указатель на следующий элемент

    // Конструктор с параметрами
    Airplane(const std::string& airport, const std::string& airline,
        const std::string& model, const std::string& flightNum,
        const std::string& time, const std::string& dest);

    // Конструктор по умолчанию
    Airplane();

    // Конструктор копирования
    Airplane(const Airplane& other);

    // Оператор присваивания
    Airplane& operator=(const Airplane& other);

    // Метод для отображения информации о рейсе
    void display() const;

    // Метод для проверки совпадения по всем полям
    bool matches(const Airplane& other) const;
};
