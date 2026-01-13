#pragma once
#include "Airplane.h"

// Класс двусвязного списка для хранения воздушных судов
class LinkedList 
{
private:
    Airplane* head;    // Указатель на первый элемент
    Airplane* tail;    // Указатель на последний элемент

    int size;          // Количество элементов

public:
    LinkedList();
    ~LinkedList();

    // Основные операции с списком
    void addAirplane(const Airplane& plane);
    void displayAll() const;
    bool removeAirplane(const Airplane& plane);
    void clear();

    // Поиск по различным критериям
    LinkedList* searchByAirport(const std::string& airport) const;
    LinkedList* searchByAirline(const std::string& airline) const;
    LinkedList* searchByModel(const std::string& model) const;
    LinkedList* searchByFlightNumber(const std::string& flightNum) const;
    LinkedList* searchByDestination(const std::string& dest) const;
    LinkedList* searchByDepartureTime(const std::string& time) const;

    // Сортировка по различным полям
    void sortByAirport();
    void sortByAirline();
    void sortByModel();
    void sortByFlightNumber();
    void sortByDepartureTime();
    void sortByDestination();

    // Вспомогательные методы
    bool isEmpty() const;
    int getSize() const;
    Airplane* getHead() const;

private:
    // Вспомогательные методы для сортировки
    void shellSortByField(bool (*compare)(const Airplane*, const Airplane*));
    static void swapAirplaneData(Airplane* a, Airplane* b);
    Airplane* getNode(int index) const;
};
