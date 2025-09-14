#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

#include "Graph.h"
#include <iostream>
#include <string>

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

int getIntInput(const std::string& prompt) {
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

std::string getStringInput(const std::string& prompt) 
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

std::wstring getStringInput(const std::wstring& prompt)
{
    std::wstring input;
    std::wcout << prompt;
    std::getline(std::wcin, input);
    return input;
}

void showMainMenu() {
    std::cout << "========== GRAPH MENU ==========\n";
    std::cout << "1. Create graph from file\n";
    std::cout << "2. Create empty graph\n";
    std::cout << "===============================\n\n";
}

void showOperationsMenu() {
    std::cout << "========== OPERATIONS ==========\n";
    std::cout << "1. Add vertex\n";
    std::cout << "2. Add edge\n";
    std::cout << "3. List all edges\n";
    std::cout << "4. Find distances (Ford-Bellman)\n";
    std::cout << "5. Find MST (Prim)\n";
    std::cout << "Any other key - Exit\n";
    std::cout << "===============================\n\n";
}

Graph createGraphFromFile() {
    std::wstring path = getStringInput(L"File path: ");
    std::string orient = getStringInput("Oriented? (y/n): ");
    bool isOrient = (orient == "y" || orient == "Y");
    return Graph(path, isOrient);
}

void handleAddVertex(Graph& graph) {
    int vertex = getIntInput("Vertex number: ");
    graph.add_vertex(vertex);
    std::cout << "Vertex " << vertex << " added\n";
}

void handleAddEdge(Graph& graph) {
    int from = getIntInput("From vertex: ");
    int to = getIntInput("To vertex: ");
    int weight = getIntInput("Weight: ");
    graph.add_edge(from, to, weight);
    std::cout << "Edge added: " << from << " -> " << to << " (weight " << weight << ")\n";
}

void handleListEdges(Graph& graph) {
    std::cout << "All edges:\n";
    graph.list_of_edges();
}

void handleFordBellman(Graph& graph) {
    int start = getIntInput("Start vertex: ");
    graph.FordBellman(start);
}

void handlePrimMST(Graph& graph) {
    int start = getIntInput("Start vertex: ");
    Graph mst = graph.MST_Prim(start);
    std::cout << "Minimum Spanning Tree:\n";
    mst.list_of_edges();
}

void handleOperation(const std::string& choice, Graph& graph) {
    if (choice == "1") handleAddVertex(graph);
    else if (choice == "2") handleAddEdge(graph);
    else if (choice == "3") handleListEdges(graph);
    else if (choice == "4") handleFordBellman(graph);
    else if (choice == "5") handlePrimMST(graph);
}

int main() {
    // Graph creation
    std::string choice;
    while (choice != "1" && choice != "2") {
        showMainMenu();
        choice = getStringInput("Choose option: ");
        clearConsole();
    }

    Graph graph;
    try {
        if (choice == "1") {
            graph = createGraphFromFile();
        }
        std::cout << "Graph created successfully!\n";
        waitForEnter();
        clearConsole();
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        waitForEnter();
        return 1;
    }

    // Main operations loop
    while (true) {
        showOperationsMenu();
        choice = getStringInput("Choose operation: ");

        if (choice != "1" && choice != "2" && choice != "3" && choice != "4" && choice != "5") {
            std::cout << "Goodbye!\n";
            break;
        }

        try {
            handleOperation(choice, graph);
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