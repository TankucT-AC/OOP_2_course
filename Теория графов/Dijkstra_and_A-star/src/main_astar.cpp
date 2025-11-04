#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <utility>
#include <sstream>
#include <chrono>

using Position = std::pair<int, int>;
using Element = std::pair<int, Position>;
using Parent = std::vector<std::vector<Position>>;

// Функция чтения матрицы из файла
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
    m = 0;

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

        m++;  // увеличиваем счетчик строк после обработки
    }

    return 0;
}

// Вспомогательная функция проверки принадлежности точки сетке
bool isInMatrix(int n, int m, int x, int y)
{
	return (0 <= x && x < n) && (0 <= y && y < m);
}

// Функция вывода найденного пути
void PathShow(std::ofstream& fout, 
    const Parent& parent, 
    Position start, 
    Position end,
    std::string&& name)
{
    fout << name << " path:\n";
    if (start == end)
    {
        fout << "(" << start.first << ", " << start.second << ")" << " ";
        return;
    }

    std::vector<Position> path;
    Position curr = end;
    while (curr != start)
    {
        path.push_back(curr);
        curr = parent[curr.first][curr.second];
    }
    path.push_back(curr);

    std::reverse(path.begin(), path.end());

    for (const auto& [x, y] : path)
    {
        fout << "(" << x << ", " << y << ")" << " ";
    }
    fout << std::endl;
}

// Алгоритм Дейкстры
int dijkstra(int n, int m,
    const std::vector<std::vector<int>>& mtr,
    Position start,
    Position end,
    Parent& parent)
{
    const int INF = 1e9;
    std::vector<std::vector<int>> dist(n, std::vector<int>(m, INF));
    dist[start.first][start.second] = 0;

    std::set<Element> min_set;
    min_set.insert({ 0, start });

    // Смещения из текущей позиции
    std::vector<int> dx = { -1, 0, 1, 1, 1, 0, -1, -1 };
    std::vector<int> dy = { 1, 1, 1, 0, -1, -1, -1, 0 };

    while (!min_set.empty())
    {
        auto pos = min_set.begin()->second;
        min_set.erase(min_set.begin());

        for (int i = 0; i < 8; i++)
        {
            int curr_x = pos.first + dx[i];
            int curr_y = pos.second + dy[i];

            if (!isInMatrix(n, m, curr_x, curr_y)) continue;

            // По условию задачи в 0 нет пути, поэтому пропускаем
            if (mtr[curr_x][curr_y] == 0) continue;

            int new_dist = dist[pos.first][pos.second] + mtr[curr_x][curr_y];

            if (new_dist < dist[curr_x][curr_y])
            {
                parent[curr_x][curr_y] = pos;
                min_set.erase({ dist[curr_x][curr_y], {curr_x, curr_y} });
                dist[curr_x][curr_y] = new_dist;
                min_set.insert({ new_dist, {curr_x, curr_y} });
            }
        }
    }

    if (dist[end.first][end.second] == INF) return -1;

    return dist[end.first][end.second];
}

// Алгоритм А*
template <typename Func>
int A_star(int n, int m,
    const std::vector<std::vector<int>>& mtr,
    Position start,
    Position end,
    Parent& parent,
    Func h)
{
    const int INF = 1e9;
    std::vector<std::vector<int>> g(n, std::vector<int>(m, INF));
    std::vector<std::vector<int>> f(n, std::vector<int>(m, INF));

    std::set<Element> min_set;

    g[start.first][start.second] = 0;
    f[start.first][start.second] = g[start.first][start.second] + h(start, end);
    min_set.insert({ f[start.first][start.second], start });

    // Смещения из текущей позиции
    std::vector<int> dx = { -1, 0, 1, 1, 1, 0, -1, -1 };
    std::vector<int> dy = { 1, 1, 1, 0, -1, -1, -1, 0 };

    while (!min_set.empty())
    {
        auto it = min_set.begin();
        auto pos = it->second;
        min_set.erase(it);

        if (pos == end)
        {
            return g[pos.first][pos.second];
        }

        for (int i = 0; i < 8; i++)
        {
            int curr_x = pos.first + dx[i];
            int curr_y = pos.second + dy[i];

            // Проверка выхода за границы и препятствий
            if (!isInMatrix(n, m, curr_x, curr_y) || mtr[curr_x][curr_y] == 0)
                continue;

            Position curr_pos = { curr_x, curr_y };

            // Вычисление новой стоимости g
            int tentative_g = g[pos.first][pos.second] + mtr[curr_x][curr_y];

            if (tentative_g < g[curr_x][curr_y])
            {
                parent[curr_x][curr_y] = pos;
                min_set.erase({ f[curr_x][curr_y], curr_pos });
                g[curr_x][curr_y] = tentative_g;
                f[curr_x][curr_y] = g[curr_x][curr_y] + h(curr_pos, end);
                min_set.insert({ f[curr_x][curr_y], curr_pos });
            }
        }
    }

    return -1;
}

template <typename Func, typename... Args>
decltype(auto) TimeDuration(Func f, int& dist, Args&&... args)
{
    auto start = std::chrono::high_resolution_clock::now();
    dist = f(args...);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    return duration;
}

int main(int argc, char* argv[])
{
    std::string inputFile;
    std::string outputFile = "ans.txt";

    Position start;
    Position end;

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
        auto err = ReadMatrix(inputFile, mtr, n, m);
        if (err != 0) return err;

        auto ChebyshevsHeuristic = [](const Position& current, const Position& goal) -> int
            {
                return std::max(std::abs(current.first - goal.first), std::abs(current.second - goal.second));
            };

        Parent dijkstra_parent(n, std::vector<Position>(m, { -1, -1 }));
        Parent a_star_parent(n, std::vector<Position>(m, { -1, -1 }));

        int dijkstra_dist = -1, a_star_dist = -1;

        auto dijkstra_duration = TimeDuration(dijkstra, dijkstra_dist, n, m, mtr, start, end, dijkstra_parent);
        auto a_star_duration = TimeDuration(A_star<decltype(ChebyshevsHeuristic)>, a_star_dist, 
            n, m, mtr, start, end, a_star_parent, ChebyshevsHeuristic);

        std::ofstream fout(outputFile);

        if (dijkstra_dist == -1)
            fout << "Dijkstra's algorithm: Path is not found!\n";
        else
        {
            fout << "Dijkstra's algorithm: Path's size: " << dijkstra_dist << "\n";
            PathShow(fout, dijkstra_parent, start, end, "Dijkstra's algorithm");
        }
        if (a_star_dist == -1) 
            fout << "A* algorithm: Path is not found!\n";
        else
        {
            fout << "A* algorithm: Path's size: " << a_star_dist << "\n";
            PathShow(fout, a_star_parent, start, end, "A* algorithm");
        }

        fout << "Running time of Dijkstra's algorithm: " << dijkstra_duration.count() << " .ms\n";
        fout << "Running time of A* algorithm: " << a_star_duration.count() << " .ms\n";

        fout.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Error processing graph: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
