#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <utility>
#include <sstream>

int ReadMatrix(const std::string& InputFile, 
    std::vector<std::vector<int>>& mtr, 
    int& n, int& m)
{
    std::ifstream fin(InputFile);

    if (!fin.is_open())
    {
        std::cerr << "ERROR: File is not found!" << std::endl;
        return 1;
    }

    if (fin.peek() == EOF)
    {
        std::cerr << "ERROR: File is empty!" << std::endl;
        return 1;
    }

    std::string line;
    n = -1;
    m = 0;  // начинаем с 0 строк

    while (std::getline(fin, line))
    {
        // Пропускаем пустые строки
        if (line.empty()) continue;

        std::istringstream iss(line);
        mtr.push_back({});  // добавляем новую строку

        int num;
        while (iss >> num)  // читаем пока успешно
        {
            if (num < 0)
            {
                std::cerr << "ERROR: Weights must be positive or 0!" << std::endl;
                return 1;
            }
            mtr[m].push_back(num);
        }

        // Проверяем, не осталось ли нечисловых данных
        if (!iss.eof())
        {
            std::cerr << "ERROR: Weights must be integer!" << std::endl;
            return 1;
        }

        // Устанавливаем/проверяем размерность
        if (n == -1)
        {
            n = static_cast<int>(mtr[m].size());
        }
        else if (n != static_cast<int>(mtr[m].size()))
        {
            std::cerr << "ERROR: Uncorrect matrix! Expected " << n
                << " columns, got " << mtr[m].size() << std::endl;
            return 1;
        }

        m++;  // увеличиваем счетчик строк ПОСЛЕ обработки
    }

    return 0;
}

bool isInMatrix(int n, int m, int x, int y)
{
	return (0 <= x && x < n) && (0 < y && y < m);
}

int dijkstra(int n, int m, 
	const std::vector<std::vector<int>>& mtr, 
	std::pair<int, int> start, 
	std::pair<int, int> end)
{
	const int INF = 1e9;
	std::vector<std::vector<int>> dist(n, std::vector<int>(m, INF));
	dist[start.first][start.second] = 0;
	
	std::set<std::pair<int, std::pair<int, int>>> min_vertex;
	min_vertex.insert({ 0, start });

	std::vector<int> dx = { -1, 0, 1 };
	std::vector<int> dy = { -1, 0, 1 };

	while (!min_vertex.empty())
	{
		auto pos = min_vertex.begin()->second;
		min_vertex.erase(min_vertex.begin());

		for (const int x : dx)
		{
			for (const int y : dy)
			{
				int curr_x = pos.first + x;
				int curr_y = pos.second + y;
				if ((!x && !y) || !isInMatrix(n, m, curr_x, curr_y) || !mtr[curr_x][curr_y]) continue;

				int curr_w = mtr[curr_x][curr_y];

				if (dist[curr_x][curr_y] > dist[pos.first][pos.second] + curr_w)
				{
					min_vertex.erase({ dist[curr_x][curr_y], {curr_x, curr_y} });
					dist[curr_x][curr_y] = dist[pos.first][pos.second] + curr_w;
					min_vertex.insert({ dist[curr_x][curr_y], {curr_x, curr_y} });
				}
			}
		}
	}

	if (dist[end.first][end.second] == INF)
		return -1;
	return dist[end.first][end.second];
}

int main(int argc, char* argv[])
{
    std::string inputFile;
    std::string outputFile = "ans.txt";

    std::pair<int, int> start;
    std::pair<int, int> end;

    // Обработка аргументов командной строки
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-a") {
            // Обработка начальной позиции
            try {
                int start_x = std::stoi(argv[++i]);
                int start_y = std::stoi(argv[++i]);
                if (start_x < 0 || start_y < 0) {
                    std::cerr << "ERROR: Coordinates must be positive" << std::endl;
                    return 1;
                }

                start = { start_x, start_y };
            }
            catch (const std::exception& e) {
                std::cerr << "ERROR: Invalid start position" << std::endl;
                return 1;
            }
        }
        else if (arg == "-b") {
            // Обработка конечной позиции
            try {
                int end_x = std::stoi(argv[++i]);
                int end_y = std::stoi(argv[++i]);
                if (end_x < 0 || end_y < 0) {
                    std::cerr << "ERROR: Coordinates must be positive" << std::endl;
                    return 1;
                }

                end = { end_x, end_y };
            }
            catch (const std::exception& e) {
                std::cerr << "ERROR: Invalid start position" << std::endl;
                return 1;
            }
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
        std::cerr << "Usage: " << argv[0] << " <input_file> [-a <start_x, start_y>] [-b <end_x, end_y>]" << std::endl;
        return 1;
    }

    try {
        // Считывание графа и обработка
        std::vector<std::vector<int>> mtr;
        int n, m;
        ReadMatrix(inputFile, mtr, n, m);

        auto dijkstra_dist = dijkstra(n, m, mtr, start, end);

        std::cout << dijkstra_dist << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error processing graph: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
