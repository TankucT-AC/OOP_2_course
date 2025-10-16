#include "ParallelsGraph.h"
#include <iostream>
#include <sstream>
#include <vector>

int main(int argc, char* argv[])
{
    std::string inputFile;
    std::string outputFile = "ans.txt";
    int numThreads = 4;

    // Обработка аргументов командной строки
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-n" && i + 1 < argc) {
            // Обработка количества потоков
            try {
                numThreads = std::stoi(argv[++i]);
                if (numThreads <= 0) {
                    std::cerr << "ERROR: Number of threads must be positive" << std::endl;
                    return 1;
                }
            }
            catch (const std::exception& e) {
                std::cerr << "ERROR: Invalid number of threads" << std::endl;
                return 1;
            }
        }
        else if (arg == "-o" && i + 1 < argc) {
            // Обработка выходного файла
            outputFile = argv[++i];
        }
        else if (arg[0] == '-') {
            // Неизвестный флаг
            std::cerr << "ERROR: Unknown option " << arg << std::endl;
            return 1;
        }
        else {
            // Имя входного файла (обязательный параметр)
            if (inputFile.empty()) {
                inputFile = arg;
            }
            else {
                std::cerr << "ERROR: Multiple input files specified" << std::endl;
                return 1;
            }
        }
    }

    // Проверка обязательного параметра
    if (inputFile.empty()) {
        std::cerr << "ERROR: Input file name is required" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <input_file> [-n threads] [-o output_file]" << std::endl;
        return 1;
    }

    try {
        // Создание графа и обработка
        ParallelsGraph graph(inputFile, false, numThreads);

        auto MST = graph.MST_BoruvkaParallels();
        MST.write_graph(outputFile);

        std::cout << "Processing complete:" << std::endl;
        std::cout << "Input file: " << inputFile << std::endl;
        std::cout << "Output file: " << outputFile << std::endl;
        std::cout << "Threads: " << numThreads << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Error processing graph: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}