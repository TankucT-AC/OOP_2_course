#include "LinkedList.h"
#include <iostream>

// Функции сравнения для сортировки
static bool compareByAirport(const Airplane* a, const Airplane* b) {
    return a->airportName < b->airportName;
}

static bool compareByAirline(const Airplane* a, const Airplane* b) {
    return a->airline < b->airline;
}

static bool compareByModel(const Airplane* a, const Airplane* b) {
    return a->planeModel < b->planeModel;
}

static bool compareByFlightNumber(const Airplane* a, const Airplane* b) {
    return a->flightNumber < b->flightNumber;
}

static bool compareByDepartureTime(const Airplane* a, const Airplane* b) {
    return a->departureTime < b->departureTime;
}

static bool compareByDestination(const Airplane* a, const Airplane* b) {
    return a->destination < b->destination;
}

LinkedList::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

LinkedList::~LinkedList() 
{
    clear();
}

void LinkedList::addAirplane(const Airplane& plane) 
{
    Airplane* newPlane = new Airplane(plane);

    if (head == nullptr) 
    {
        head = tail = newPlane;
    }
    else 
    {
        tail->next = newPlane;
        newPlane->prev = tail;
        tail = newPlane;
    }
    size++;
}

void LinkedList::displayAll() const 
{
    if (isEmpty()) 
    {
        std::cout << "Список пуст.\n";
        return;
    }

    head->prev = tail;
    tail->next = head;

    Airplane* current = head;
    int counter = 1;
    while (current != nullptr) 
    {
        std::cout << "Запись #" << counter++ << ":\n";
        current->display();
        current = current->next;
    }
}

bool LinkedList::removeAirplane(const Airplane& plane) {
    if (isEmpty()) return false;

    Airplane* current = head;
    while (current != nullptr) 
    {
        if (current->matches(plane)) 
        {
            if (current->prev != nullptr) 
            {
                current->prev->next = current->next;
            }
            else 
            {
                head = current->next;
            }

            if (current->next != nullptr) 
            {
                current->next->prev = current->prev;
            }
            else 
            {
                tail = current->prev;
            }

            delete current;
            size--;
            return true;
        }
        current = current->next;
    }
    return false;
}

void LinkedList::clear() 
{
    Airplane* current = head;
    while (current != nullptr) 
    {
        Airplane* next = current->next;
        delete current;
        current = next;
    }
    head = tail = nullptr;
    size = 0;
}

LinkedList* LinkedList::searchByAirport(const std::string& airport) const 
{
    LinkedList* result = new LinkedList();
    Airplane* current = head;

    while (current != nullptr) 
    {
        if (current->airportName == airport) 
        {
            result->addAirplane(*current);
        }
        current = current->next;
    }

    return result;
}

LinkedList* LinkedList::searchByAirline(const std::string& airline) const 
{
    LinkedList* result = new LinkedList();
    Airplane* current = head;

    while (current != nullptr) 
    {
        if (current->airline == airline) 
        {
            result->addAirplane(*current);
        }
        current = current->next;
    }

    return result;
}

LinkedList* LinkedList::searchByModel(const std::string& model) const 
{
    LinkedList* result = new LinkedList();
    Airplane* current = head;

    while (current != nullptr) 
    {
        if (current->planeModel == model) 
        {
            result->addAirplane(*current);
        }
        current = current->next;
    }

    return result;
}

LinkedList* LinkedList::searchByFlightNumber(const std::string& flightNum) const 
{
    LinkedList* result = new LinkedList();
    Airplane* current = head;

    while (current != nullptr) 
    {
        if (current->flightNumber == flightNum) 
        {
            result->addAirplane(*current);
        }
        current = current->next;
    }

    return result;
}

LinkedList* LinkedList::searchByDestination(const std::string& dest) const 
{
    LinkedList* result = new LinkedList();
    Airplane* current = head;

    while (current != nullptr) 
    {
        if (current->destination == dest) 
        {
            result->addAirplane(*current);
        }
        current = current->next;
    }

    return result;
}

LinkedList* LinkedList::searchByDepartureTime(const std::string& time) const 
{
    LinkedList* result = new LinkedList();
    Airplane* current = head;

    while (current != nullptr) 
    {
        if (current->departureTime == time) 
        {
            result->addAirplane(*current);
        }
        current = current->next;
    }

    return result;
}

// Вспомогательная функция для обмена данными между узлами
void LinkedList::swapAirplaneData(Airplane* a, Airplane* b) 
{
    if (a == b) return;

    std::swap(a->airportName, b->airportName);
    std::swap(a->airline, b->airline);
    std::swap(a->planeModel, b->planeModel);
    std::swap(a->flightNumber, b->flightNumber);
    std::swap(a->departureTime, b->departureTime);
    std::swap(a->destination, b->destination);
}

// Получение узла по индексу
Airplane* LinkedList::getNode(int index) const 
{
    if (index < 0 || index >= size) return nullptr;

    Airplane* current = head;
    for (int i = 0; i < index && current != nullptr; i++) 
    {
        current = current->next;
    }
    return current;
}

// Сортировка Шелла
void LinkedList::shellSortByField(bool (*compare)(const Airplane*, const Airplane*)) 
{
    if (size < 2) return;

    int n = size;

    // Последовательность шагов Кнута: h = 3*h + 1
    int h = 1;
    while (h < n / 3) 
    {
        h = 3 * h + 1;
    }

    // Сортировка Шелла
    while (h >= 1) 
    {
        for (int i = h; i < n; i++) 
        {
            int j = i;
            while (j - h >= 0) 
            {
                Airplane* node_j = getNode(j);
                Airplane* node_jh = getNode(j - h);

                if (!compare(node_j, node_jh)) break;
                
                swapAirplaneData(node_j, node_jh);
                j -= h;
            }
        }
        h = h / 3;
    }
}

void LinkedList::sortByAirport() 
{
    shellSortByField(compareByAirport);
}

void LinkedList::sortByAirline() 
{
    shellSortByField(compareByAirline);
}

void LinkedList::sortByModel() 
{
    shellSortByField(compareByModel);
}

void LinkedList::sortByFlightNumber() 
{
    shellSortByField(compareByFlightNumber);
}

void LinkedList::sortByDepartureTime() 
{
    shellSortByField(compareByDepartureTime);
}

void LinkedList::sortByDestination() 
{
    shellSortByField(compareByDestination);
}

bool LinkedList::isEmpty() const 
{
    return head == nullptr;
}

int LinkedList::getSize() const 
{
    return size;
}

Airplane* LinkedList::getHead() const 
{
    return head;
}