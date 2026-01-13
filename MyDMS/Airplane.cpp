#include "Airplane.h"
#include <iostream>

Airplane::Airplane(const std::string& airport, const std::string& airline,
    const std::string& model, const std::string& flightNum,
    const std::string& time, const std::string& dest)
    : airportName(airport), airline(airline), planeModel(model),
    flightNumber(flightNum), departureTime(time), destination(dest),
    prev(nullptr), next(nullptr) {}

Airplane::Airplane()
    : prev(nullptr), next(nullptr) {}

Airplane::Airplane(const Airplane& other)
    : airportName(other.airportName),
    airline(other.airline),
    planeModel(other.planeModel),
    flightNumber(other.flightNumber),
    departureTime(other.departureTime),
    destination(other.destination),
    prev(nullptr), 
    next(nullptr)   
{}

Airplane& Airplane::operator=(const Airplane& other) {
    if (this != &other) {
        airportName = other.airportName;
        airline = other.airline;
        planeModel = other.planeModel;
        flightNumber = other.flightNumber;
        departureTime = other.departureTime;
        destination = other.destination;
    }
    return *this;
}

void Airplane::display() const 
{
    std::cout << "Аэропорт: " << airportName << "\n";
    std::cout << "Авиакомпания: " << airline << "\n";
    std::cout << "Марка самолета: " << planeModel << "\n";
    std::cout << "Номер рейса: " << flightNumber << "\n";
    std::cout << "Время вылета: " << departureTime << "\n";
    std::cout << "Пункт прибытия: " << destination << "\n";
    std::cout << "---------------------------\n";
}

bool Airplane::matches(const Airplane& other) const 
{
    return airportName == other.airportName &&
        airline == other.airline &&
        planeModel == other.planeModel &&
        flightNumber == other.flightNumber &&
        departureTime == other.departureTime &&
        destination == other.destination;
}