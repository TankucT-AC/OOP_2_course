#include <iostream>
#include <string>
#include <limits>
#include "DatabaseManager.h"

// Функции для ввода данных
static void displayMenu();
static void inputAirplane(Airplane& plane);
static void searchMenu(DatabaseManager& db);
static void deleteMenu(DatabaseManager& db);
static void sortMenu(DatabaseManager& db);
static void clearConsole();
static void waitAndClear();
static void protectInputChoice(int& choice, std::string&& err);

int main() 
{
    DatabaseManager db("airplanes.txt");
    int choice;

    // Первая очистка консоли при запуске
    clearConsole();

    do {
        displayMenu();
        std::cout << "Выберите действие: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Очищаем консоль после выбора пункта меню
        clearConsole();

        switch (choice) {
        case 1: { // Добавить запись
            Airplane plane;
            inputAirplane(plane);
            db.addAirplane(plane);
            std::cout << "\nЗапись добавлена успешно.\n";
            break;
        }
        case 2: // Показать все записи
            db.displayAll();
            break;
        case 3: // Поиск
            searchMenu(db);
            break;
        case 4: // Удаление
            deleteMenu(db);
            break;
        case 5: // Сортировка
            sortMenu(db);
            break;
        case 6: // Сохранить в файл
            db.saveToFile();
            std::cout << "Данные сохранены в файл.\n";
            break;
        case 0: // Выход
            std::cout << "Выход из программы.\n";
            break;
        default:
            std::cout << "Неверный выбор. Попробуйте снова.\n";
        }

        if (choice != 0) {
            waitAndClear();
        }

    } while (choice != 0);

    return 0;
}

static void displayMenu() 
{
    std::cout << "=== СИСТЕМА УЧЕТА ВОЗДУШНЫХ СУДОВ ===\n";
    std::cout << "1. Добавить новую запись\n";
    std::cout << "2. Показать все записи\n";
    std::cout << "3. Поиск записей\n";
    std::cout << "4. Удалить запись\n";
    std::cout << "5. Сортировать записи\n";
    std::cout << "6. Сохранить данные в файл\n";
    std::cout << "0. Выход\n";
}

static void inputAirplane(Airplane& plane) 
{
    std::cout << "=== ВВОД ДАННЫХ О РЕЙСЕ ===\n";

    std::cout << "Название аэропорта: ";
    std::getline(std::cin, plane.airportName);

    std::cout << "Авиакомпания: ";
    std::getline(std::cin, plane.airline);

    std::cout << "Марка самолета: ";
    std::getline(std::cin, plane.planeModel);

    std::cout << "Номер рейса: ";
    std::getline(std::cin, plane.flightNumber);

    std::cout << "Время вылета (ЧЧ:ММ): ";
    std::getline(std::cin, plane.departureTime);

    std::cout << "Пункт прибытия: ";
    std::getline(std::cin, plane.destination);
}

static void searchMenu(DatabaseManager& db) 
{
    int choice;
    std::string value;

    std::cout << "=== ПОИСК ЗАПИСЕЙ ===\n";
    std::cout << "1. По названию аэропорта\n";
    std::cout << "2. По авиакомпании\n";
    std::cout << "3. По марке самолета\n";
    std::cout << "4. По номеру рейса\n";
    std::cout << "5. По времени вылета\n";
    std::cout << "6. По пункту прибытия\n";
    std::cout << "Выберите поле для поиска: ";

    protectInputChoice(choice, "Ошибка! Введите число от 1 до 6: ");

    std::cout << "Введите значение для поиска: ";
    std::getline(std::cin, value);

    std::cout << "\n";
    switch (choice) {
    case 1: db.searchAndDisplay("airport", value); break;
    case 2: db.searchAndDisplay("airline", value); break;
    case 3: db.searchAndDisplay("model", value); break;
    case 4: db.searchAndDisplay("flight", value); break;
    case 5: db.searchAndDisplay("time", value); break;
    case 6: db.searchAndDisplay("destination", value); break;
    default: std::cout << "Неверный выбор.\n";
    }
}

static void deleteMenu(DatabaseManager& db) 
{
    std::cout << "=== УДАЛЕНИЕ ЗАПИСИ ===\n";

    // Сначала показываем все записи
    db.displayAll();

    if (db.isEmpty()) return;

    // Запрашиваем данные для удаления
    Airplane plane;
    std::cout << "\nВведите данные записи для удаления:\n";

    std::cout << "Название аэропорта: ";
    std::getline(std::cin, plane.airportName);

    std::cout << "Авиакомпания: ";
    std::getline(std::cin, plane.airline);

    std::cout << "Марка самолета: ";
    std::getline(std::cin, plane.planeModel);

    std::cout << "Номер рейса: ";
    std::getline(std::cin, plane.flightNumber);

    std::cout << "Время вылета: ";
    std::getline(std::cin, plane.departureTime);

    std::cout << "Пункт прибытия: ";
    std::getline(std::cin, plane.destination);

    // Подтверждение
    std::cout << "\nВы уверены, что хотите удалить эту запись? (y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore();

    if (confirm == 'y' || confirm == 'Y') {
        db.removeAirplane(plane);
    }
    else {
        std::cout << "Удаление отменено.\n";
    }
}

static void sortMenu(DatabaseManager& db) 
{
    std::cout << "=== СОРТИРОВКА ЗАПИСЕЙ ===\n";
    std::cout << "1. По названию аэропорта\n";
    std::cout << "2. По авиакомпании\n";
    std::cout << "3. По марке самолета\n";
    std::cout << "4. По номеру рейса\n";
    std::cout << "5. По времени вылета\n";
    std::cout << "6. По пункту прибытия\n";
    std::cout << "Выберите поле для сортировки: ";

    int choice;
    protectInputChoice(choice, "Ошибка! Введите число от 1 до 6: ");

    db.sortByField(choice);
}

// Функция для очистки консоли
static void clearConsole()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Функция для паузы с очисткой экрана
static void waitAndClear()
{
    std::cout << "\nНажмите Enter для продолжения...";
    std::cin.get();
    clearConsole();
}

// Функция для безопасного ввода выбора
static void protectInputChoice(int& choice, std::string&& err)
{
    while (!(std::cin >> choice)) {
        std::cout << err;
        std::cin.clear(); // Сбрасываем флаг ошибки
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем буфер
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}