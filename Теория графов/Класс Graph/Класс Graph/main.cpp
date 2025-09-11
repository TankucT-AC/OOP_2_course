#include <iostream>
#include "Graph.h"

//const std::string FName = "Тесты/test1.txt"; // файл не существует
//const std::string FName = "Тесты/test2.txt"; // файл пустой
//const std::string FName = "Тесты/test3.txt"; // некорректные ребра (нехватка эл-ов)
const std::string FName = "Тесты/test4.txt"; // некорректные ребра (избыток эл-ов)
//const std::string FName = "Тесты/test1.1.txt"; 

int main()
{
	try {
		Graph graph(FName, true);
		std::cout << graph.size() << std::endl;
		graph.FordBellman(5);
	}
	catch (const std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
