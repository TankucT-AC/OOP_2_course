#pragma once
#include "LinkedList.h"
#include <string>

// Класс для управления базой данных и файловыми операциями
class DatabaseManager 
{
private:
    LinkedList database;
    std::string filename;

public:
    DatabaseManager(const std::string& file);

    // Работа с файлом
    void loadFromFile();
    void saveToFile() const;

    // Операции с базой данных
    void addAirplane(const Airplane& plane);
    void removeAirplane(const Airplane& plane);
    void displayAll() const;

    // Поисковые операции
    void searchAndDisplay(const std::string& field, const std::string& value) const;

    // Сортировка
    void sortByField(int fieldChoice);

    // Вспомогательные функции
    bool isEmpty();

private:
    Airplane parseLine(const std::string& line) const;
    std::string airplaneToString(const Airplane& plane) const;
};
