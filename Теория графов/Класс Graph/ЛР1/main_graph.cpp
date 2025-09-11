#include <iostream>
#include "Graph.h"

//const std::string FName = "graphs/test1.txt"; // файл не существует
//const std::string FName = "graphs/test2.txt"; // файл пустой
//const std::string FName = "graphs/test3.txt"; // некорректные ребра (нехватка эл-ов)
//const std::string FName = "graphs/test4.txt"; // некорректные ребра (избыток эл-ов)

const std::string FName = "graphs/test1.1.txt"; 

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
