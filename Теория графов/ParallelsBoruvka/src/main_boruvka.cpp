#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

#include "ParallelsGraph.h"
#include <iostream>
#include <string>

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

static int getIntInput(const std::string& prompt)
{
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

static std::string getStringInput(const std::string& prompt)
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

static std::wstring getStringInput(const std::wstring& prompt)
{
    std::wstring input;
    std::wcout << prompt;
    std::getline(std::wcin, input);
    return input;
}

static void showMainMenu()
{
    std::cout << "========== GRAPH MENU ==========\n";
    std::cout << "1. Create graph from file\n";
    std::cout << "2. Create empty graph\n";
    std::cout << "===============================\n\n";
}

static void showOperationsMenu()
{
    std::cout << "========== OPERATIONS ==========\n";
    std::cout << "1. Add vertex\n";
    std::cout << "2. Add edge\n";
    std::cout << "3. List all edges\n";
    std::cout << "4. Find distances (Ford-Bellman)\n";
    std::cout << "5. Find MST (Prim)\n";
    std::cout << "6. Find MST (Parallels Boruvka)\n";
    std::cout << "Any other key - Exit\n";
    std::cout << "===============================\n\n";
}

static ParallelsGraph createGraphFromFile()
{
    std::wstring path = getStringInput(L"File path: ");
    std::string orient = getStringInput("Oriented? (y/n): ");
    bool isOrient = (orient == "y" || orient == "Y");
    return ParallelsGraph(path, isOrient);
}

static void handleAddVertex(Graph& graph)
{
    int vertex = getIntInput("Vertex number: ");
    graph.add_vertex(vertex);
    std::cout << "Vertex " << vertex << " added\n";
}

static void handleAddEdge(Graph& graph)
{
    int from = getIntInput("From vertex: ");
    int to = getIntInput("To vertex: ");
    int weight = getIntInput("Weight: ");
    graph.add_edge(from, to, weight);
    std::cout << "Edge added: " << from << " -> " << to << " (weight " << weight << ")\n";
}

static void handleListEdges(Graph& graph)
{
    std::cout << "All edges:\n";
    graph.list_of_edges();
}

static void handleFordBellman(Graph& graph)
{
    int start = getIntInput("Start vertex: ");
    graph.FordBellman(start);
}

static void handlePrimMST(Graph& graph)
{
    int start = getIntInput("Start vertex: ");
    Graph mst = graph.MST_Prim(start);
    std::cout << "Minimum Spanning Tree:\n";
    mst.list_of_edges();
}

static void handleBoruvkaMST(ParallelsGraph& graph)
{
    int start = getIntInput("Start vertex: ");
    Graph mst = graph.MST_BoruvkaParallels(start);
    std::cout << "Minimum Spanning Tree:\n";
    mst.list_of_edges();
}

static void handleOperation(const std::string& choice, ParallelsGraph& graph)
{
    if (choice == "1") handleAddVertex(graph);
    else if (choice == "2") handleAddEdge(graph);
    else if (choice == "3") handleListEdges(graph);
    else if (choice == "4") handleFordBellman(graph);
    else if (choice == "5") handlePrimMST(graph);
    else if (choice == "6") handleBoruvkaMST(graph);
}

int main()
{
    // Graph creation
    std::string choice;
    while (choice != "1" && choice != "2")
    {
        showMainMenu();
        choice = getStringInput("Choose option: ");
        clearConsole();
    }

    ParallelsGraph graph;
    try
    {
        if (choice == "1")
        {
            graph = createGraphFromFile();
        }
        std::cout << "Graph created successfully!\n";
        waitForEnter();
        clearConsole();
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << "\n";
        waitForEnter();
        return 1;
    }

    // Main operations loop
    while (true)
    {
        showOperationsMenu();
        choice = getStringInput("Choose operation: ");

        if (choice != "1" && choice != "2" && choice != "3" && choice != "4" && choice != "5" && choice != "6")
        {
            std::cout << "Goodbye!\n";
            break;
        }

        try
        {
            handleOperation(choice, graph);
            waitForEnter();
            clearConsole();
        }
        catch (const std::exception& e)
        {
            std::cout << "Error: " << e.what() << "\n";
            waitForEnter();
            clearConsole();
        }
    }

    return 0;
}