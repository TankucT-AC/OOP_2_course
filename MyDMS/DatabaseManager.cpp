#include "DatabaseManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

DatabaseManager::DatabaseManager(const std::string& file) : filename(file) 
{
    loadFromFile();
}

void DatabaseManager::loadFromFile() 
{
    std::ifstream file(filename);
    if (!file.is_open()) 
    {
        std::cout << "Файл не найден. Будет создан новый.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) 
    {
        if (!line.empty()) 
        {
            Airplane plane = parseLine(line);
            database.addAirplane(plane);
        }
    }
    file.close();
}

void DatabaseManager::saveToFile() const 
{
    std::ofstream file(filename);
    if (!file.is_open()) 
    {
        std::cerr << "Ошибка открытия файла для записи!\n";
        return;
    }

    Airplane* current = database.getHead();
    while (current != nullptr) 
    {
        file << airplaneToString(*current) << "\n";
        current = current->next;
    }
    file.close();
}

void DatabaseManager::addAirplane(const Airplane& plane) 
{
    database.addAirplane(plane);
    saveToFile();
}

void DatabaseManager::removeAirplane(const Airplane& plane) 
{
    if (database.removeAirplane(plane)) 
    {
        std::cout << "Запись удалена успешно.\n";
        saveToFile();
    }
    else 
    {
        std::cout << "Запись не найдена.\n";
    }
}

void DatabaseManager::displayAll() const 
{
    std::cout << "=== ВСЕ ЗАПИСИ В БАЗЕ ДАННЫХ ===\n";
    database.displayAll();
    std::cout << "Всего записей: " << database.getSize() << "\n";
}

void DatabaseManager::searchAndDisplay(const std::string& field, const std::string& value) const 
{
    LinkedList* result = nullptr;

    if (field == "airport") 
    {
        result = database.searchByAirport(value);
    }
    else if (field == "airline") 
    {
        result = database.searchByAirline(value);
    }
    else if (field == "model") 
    {
        result = database.searchByModel(value);
    }
    else if (field == "flight") 
    {
        result = database.searchByFlightNumber(value);
    }
    else if (field == "time") 
    {
        result = database.searchByDepartureTime(value);
    }
    else if (field == "destination") 
    {
        result = database.searchByDestination(value);
    }

    if (result != nullptr) 
    {
        if (result->isEmpty()) 
        {
            std::cout << "Записей не найдено.\n";
        }
        else 
        {
            std::cout << "\n=== РЕЗУЛЬТАТЫ ПОИСКА ===\n";
            result->displayAll();
            std::cout << "Найдено записей: " << result->getSize() << "\n";
        }
        delete result;
    }
}

void DatabaseManager::sortByField(int fieldChoice) 
{
    switch (fieldChoice) {
    case 1: database.sortByAirport(); break;
    case 2: database.sortByAirline(); break;
    case 3: database.sortByModel(); break;
    case 4: database.sortByFlightNumber(); break;
    case 5: database.sortByDepartureTime(); break;
    case 6: database.sortByDestination(); break;
    default: std::cout << "Неверный выбор поля.\n"; return;
    }
    saveToFile();
    std::cout << "Данные отсортированы и сохранены.\n";
}

bool DatabaseManager::isEmpty() 
{ 
    return database.isEmpty(); 
}

Airplane DatabaseManager::parseLine(const std::string& line) const 
{
    std::istringstream iss(line);
    std::string airport, airline, model, flight, time, dest;

    std::getline(iss, airport, '|');
    std::getline(iss, airline, '|');
    std::getline(iss, model, '|');
    std::getline(iss, flight, '|');
    std::getline(iss, time, '|');
    std::getline(iss, dest, '|');

    return Airplane(airport, airline, model, flight, time, dest);
}

std::string DatabaseManager::airplaneToString(const Airplane& plane) const 
{
    return plane.airportName + "|" + plane.airline + "|" +
        plane.planeModel + "|" + plane.flightNumber + "|" +
        plane.departureTime + "|" + plane.destination;
}