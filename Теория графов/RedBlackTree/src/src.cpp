#include "RedBlackTree.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <limits>
#include <cstdlib>

using namespace std::chrono;

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif


// Функция для загрузки массива из файла
std::vector<int> loadArrayFromFile(const std::string & filename) {
    std::vector<int> array;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Error: Cannot open file " << filename << std::endl;
        return array;
    }

    int value;
    while (file >> value) {
        array.push_back(value);
    }

    file.close();
    std::cout << "Loaded " << array.size() << " elements from file.\n";
    return array;
}

// Вспомогательные функции для интерфейса
static void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

static int getIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore();
            break;
        }
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Invalid input. Please enter a number.\n";
    }
    return value;
}

static std::string getStringInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

// Меню
static void showMainMenu() {
    std::cout << "========== RED-BLACK TREE ==========\n";
    std::cout << "1. Create tree from file\n";
    std::cout << "2. Create empty tree\n";
    std::cout << "====================================\n\n";
}   

static void showOperationsMenu() {
    std::cout << "========== OPERATIONS ==========\n";
    std::cout << "1. Insert element\n";
    std::cout << "2. Delete element\n";
    std::cout << "3. Search element\n";
    std::cout << "4. Find minimum element\n";
    std::cout << "5. Find maximum element\n";
    std::cout << "6. Find 10 minimum elements\n";
    std::cout << "7. Find 10 maximum elements\n";
    std::cout << "8. Print all elements\n";
    std::cout << "9. Clear tree\n";
    std::cout << "0. Exit\n";
    std::cout << "================================\n\n";
}

static RedBlackTree createTreeFromFile() {
    RedBlackTree tree;
    std::string filename = getStringInput("File path: ");

    std::vector<int> array = loadArrayFromFile(filename);
    if (array.empty()) {
        std::cout << "Warning: No elements loaded. Creating empty tree.\n";
        return tree;
    }

    // Вставка элементов с измерением времени
    auto start = high_resolution_clock::now();
    for (int val : array) {
        tree.insert(val);
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    std::cout << "Tree created with " << array.size() << " elements.\n";
    std::cout << "Time: " << duration.count() << " microseconds\n";

    return tree;
}

// Обработчики операций
static void handleInsert(RedBlackTree& tree) {
    int value = getIntInput("Enter element to insert: ");

    auto start = high_resolution_clock::now();
    tree.insert(value);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    std::cout << "Element " << value << " inserted.\n";
    std::cout << "Time: " << duration.count() << " microseconds\n";
}

static void handleDelete(RedBlackTree& tree) {
    if (tree.isEmpty()) {
        std::cout << "Tree is empty.\n";
        return;
    }

    int value = getIntInput("Enter element to delete: ");

    auto start = high_resolution_clock::now();
    tree.remove(value);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    std::cout << "Element " << value << " deleted.\n";
    std::cout << "Time: " << duration.count() << " microseconds\n";
}

static void handleSearch(RedBlackTree& tree) {
    if (tree.isEmpty()) {
        std::cout << "Tree is empty.\n";
        return;
    }

    int value = getIntInput("Enter element to search: ");

    auto start = high_resolution_clock::now();
    bool found = tree.search(value);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    if (found) {
        std::cout << "Element " << value << " found.\n";
    }
    else {
        std::cout << "Element " << value << " not found.\n";
    }
    std::cout << "Time: " << duration.count() << " microseconds\n";
}

static void handleFindMin(RedBlackTree& tree) {
    if (tree.isEmpty()) {
        std::cout << "Tree is empty.\n";
        return;
    }

    auto start = high_resolution_clock::now();
    int minVal = tree.findMin();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    std::cout << "Minimum element: " << minVal << std::endl;
    std::cout << "Time: " << duration.count() << " microseconds\n";
}

static void handleFindMax(RedBlackTree& tree) {
    if (tree.isEmpty()) {
        std::cout << "Tree is empty.\n";
        return;
    }

    auto start = high_resolution_clock::now();
    int maxVal = tree.findMax();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    std::cout << "Maximum element: " << maxVal << std::endl;
    std::cout << "Time: " << duration.count() << " microseconds\n";
}

static void handleFindMinK(RedBlackTree& tree) {
    if (tree.isEmpty()) {
        std::cout << "Tree is empty.\n";
        return;
    }

    auto start = high_resolution_clock::now();
    std::vector<int> minElements = tree.findMinK(10);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    if (minElements.empty()) {
        std::cout << "No elements in tree.\n";
    }
    else {
        std::cout << "10 minimum elements: ";
        for (size_t i = 0; i < minElements.size(); ++i) {
            std::cout << minElements[i];
            if (i != minElements.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Time: " << duration.count() << " microseconds\n";
}

static void handleFindMaxK(RedBlackTree& tree) {
    if (tree.isEmpty()) {
        std::cout << "Tree is empty.\n";
        return;
    }

    auto start = high_resolution_clock::now();
    std::vector<int> maxElements = tree.findMaxK(10);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    if (maxElements.empty()) {
        std::cout << "No elements in tree.\n";
    }
    else {
        std::cout << "10 maximum elements: ";
        for (size_t i = 0; i < maxElements.size(); ++i) {
            std::cout << maxElements[i];
            if (i != maxElements.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Time: " << duration.count() << " microseconds\n";
}

static void handlePrint(RedBlackTree& tree) {
    auto start = high_resolution_clock::now();
    tree.printInOrder();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    std::cout << "Time: " << duration.count() << " microseconds\n";
}

static void handleClear(RedBlackTree& tree) {
    auto start = high_resolution_clock::now();
    tree.clear();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    std::cout << "Tree cleared.\n";
    std::cout << "Time: " << duration.count() << " microseconds\n";
}

static void handleOperation(const std::string& choice, RedBlackTree& tree) {
    if (choice == "1") handleInsert(tree);
    else if (choice == "2") handleDelete(tree);
    else if (choice == "3") handleSearch(tree);
    else if (choice == "4") handleFindMin(tree);
    else if (choice == "5") handleFindMax(tree);
    else if (choice == "6") handleFindMinK(tree);
    else if (choice == "7") handleFindMaxK(tree);
    else if (choice == "8") handlePrint(tree);
    else if (choice == "9") handleClear(tree);
}

int main() {
    // Создание дерева
    std::string choice;
    while (choice != "1" && choice != "2") {
        showMainMenu();
        choice = getStringInput("Choose option: ");
        clearConsole();
    }

    RedBlackTree tree;
    try {
        if (choice == "1") {
            tree = createTreeFromFile();
        }
        else {
            std::cout << "Empty tree created.\n";
        }
        waitForEnter();
        clearConsole();
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        waitForEnter();
        return 1;
    }

    // Основной цикл операций
    while (true) {
        showOperationsMenu();
        choice = getStringInput("Choose operation: ");

        if (choice == "0") {
            std::cout << "Goodbye!\n";
            break;
        }

        try {
            if (choice >= "1" && choice <= "9") {
                handleOperation(choice, tree);
            }
            else {
                std::cout << "Invalid choice. Please try again.\n";
            }
            waitForEnter();
            clearConsole();
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
            waitForEnter();
            clearConsole();
        }
    }

    return 0;
}