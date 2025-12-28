#include <iostream>
#include <fstream>

static void clearConsole()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void waitForEnter()
{
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

class Element {
public:
    int data;       // Значение
    Element* next;  // Указатель на следующий элемент

    Element(int value) : data(value), next(nullptr) {}

    void SetData(int newData) { data = newData; }
    int GetData() { return data; }
    void SetNext(Element* nextElem) { next = nextElem; }
    Element* GetNext() { return next; }
};

class List {
private:
    Element* head; // Указатель на первый элемент
    int size;      // Размер линейного списка

public:
    List() : head(nullptr), size(0) {}

    ~List() {
        DeleteList();
    }

    // 1. Создание списка
    void CreateList() {
        DeleteList();

        std::cout << "Введите количество элементов: ";
        std::cin >> size;

        if (size <= 0) return;

        std::cout << "Элемент 0: ";
        int value;
        std::cin >> value;

        head = new Element(value);
        Element* current = head;

        for (int i = 1; i < size; i++) {
            std::cout << "Элемент " << i << ": ";
            std::cin >> value;
            current->next = new Element(value);
            current = current->next;
        }

        std::cout << std::endl;
        std::cout << "Список создан!" << std::endl;
    }

    // 2. Вывод списка
    void DisplayList() {
        if (head == nullptr) {
            std::cout << "Список пуст!" << std::endl;
            return;
        }

        Element* current = head;
        std::cout << "Список: ";
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    // 3. Удаление списка
    void DeleteList() {
        int flag = 0;

        Element* current = head;
        if (current == nullptr) {
            flag = 1;
        }
        while (current != nullptr) {
            Element* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        size = 0;

        if (flag == 0) {
            std::cout << "Список удалён!" << std::endl;
        }
    }

    // Методы для варианта 7:

    // 2. Поиск элемента с максимальным значением (повторы запрещены)
    void FindMax() {
        if (head == nullptr) {
            std::cout << "Список пуст!" << std::endl;
            return;
        }

        Element* current = head;
        int maxValue = current->data;
        int maxPosition = 0;
        int currentPosition = 0;

        while (current != nullptr) {
            if (current->data > maxValue) {
                maxValue = current->data;
                maxPosition = currentPosition;
            }
            current = current->next;
            currentPosition++;
        }

        std::cout << "Максимальный элемент: " << maxValue
            << " на позиции: " << maxPosition << std::endl;
    }

    // 4. Включение нового элемента в конец списка
    void AddToEnd() {
        int value;
        std::cout << "Введите значение для добавления: ";
        std::cin >> value;

        Element* newElement = new Element(value);

        if (head == nullptr) {
            head = newElement;
        }
        else {
            Element* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newElement;
        }
        size++;
        std::cout << "Элемент добавлен в конец!" << std::endl;
    }

    // 8. Удаление элемента из позиции с заданным номером
    void DeleteAtPosition() {
        if (head == nullptr) {
            std::cout << "Список пуст!" << std::endl;
            return;
        }

        int position;
        std::cout << "Введите позицию для удаления (0-" << size - 1 << "): ";
        std::cin >> position;

        if (position < 0 || position >= size) {
            std::cout << "Неверная позиция!" << std::endl;
            return;
        }

        if (position == 0) {
            // Удаление первого элемента
            Element* temp = head;
            head = head->next;
            delete temp;
        }
        else {
            // Удаление прочих
            Element* current = head;
            for (int i = 0; i < position - 1; i++) {
                current = current->next;
            }
            Element* temp = current->next;
            current->next = temp->next;
            delete temp;
        }
        size--;
        std::cout << "Элемент удален!" << std::endl;
    }
};

// Интерфейс
void ShowMenu() {
    std::cout << "=== Меню ===" << std::endl;
    std::cout << "1. Создать список" << std::endl;
    std::cout << "2. Вывести список" << std::endl;
    std::cout << "3. Удалить список" << std::endl;
    std::cout << "4. Найти максимальный элемент" << std::endl;
    std::cout << "5. Добавить в конец" << std::endl;
    std::cout << "6. Удалить по позиции" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << "Выберите операцию: ";
}

int main() {
    List myList;
    int choice;

    do {
        ShowMenu();
        std::cin >> choice;

        switch (choice) {
        case 1:
            myList.CreateList();
            break;
        case 2:
            myList.DisplayList();
            break;
        case 3:
            myList.DeleteList();
            break;
        case 4:
            myList.FindMax();
            break;
        case 5:
            myList.AddToEnd();
            break;
        case 6:
            myList.DeleteAtPosition();
            break;
        case 0:
            std::cout << "Выход..." << std::endl;
            break;
        default:
            std::cout << "Неверный выбор!" << std::endl;
        }
        waitForEnter();
        clearConsole();
    } while (choice != 0);

    return 0;
}