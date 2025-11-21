#include <chrono>
#include <random>
#include <vector>
#include <algorithm>
#include <iostream>

int MIN_NUM = -1000;
int MAX_NUM = 1000;
int MAX_SIZE = 100'000'000;
const std::vector<int> SIZES = { 10'000'000, 25'000'000, 30'000'000, 50'000'000, 75'000'000, 100'000'000 };

// Функция вывода заголовка таблицы
void print_table_header() {
    std::cout << std::left;
    std::cout << std::setw(10) << "Алгоритм" << "   | "
        << std::right << std::setw(8) << "Размер" << "   | "
        << std::left << std::setw(18) << "Случай поиска" << "      | "
        << std::right << std::setw(12) << "Время(мкс)" << "   | "
        << std::setw(12) << "Counter1" << " | "
        << std::setw(12) << "Counter2" << "\n";

    std::cout << std::setw(10) << std::string(10, '-') << "-|-"
        << std::setw(8) << std::string(8, '-') << "-|-"
        << std::setw(18) << std::string(18, '-') << "-|-"
        << std::setw(12) << std::string(12, '-') << "-|-"
        << std::setw(12) << std::string(12, '-') << "-|-"
        << std::setw(12) << std::string(12, '-') << "\n";
}

// Функция вывода строки таблицы
void print_table_row(const std::string& algorithm,
    int size,
    int search_case,
    long long time,
    const std::pair<int, int>& counter)
{
    std::cout << std::left;
    std::cout << std::setw(10) << algorithm << " | "
        << std::right << std::setw(8) << size << " | "
        << std::left << std::setw(18) << search_case << " | "
        << std::right << std::setw(12) << time << " | "
        << std::setw(12) << counter.first << " | "
        << std::setw(12) << counter.second << "\n";
}

// Функция замера времени выполнения программы
template <typename Func, typename... Args>
auto TimeDuration(Func f, int& result, Args&&... args)
{
    auto start = std::chrono::high_resolution_clock::now();
    result = f(args...);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    return duration;
}

// Better Linear Search (поиск элемента в массиве)
int BLS(int key, 
    int size, 
    const std::vector<int>& vec, 
    int& counter1, int& counter2)
{
    int result = -1;
    for (int i = 0; i < size; ++i)
    {
        ++counter1;
        ++counter2;
        if (vec[i] == key)
        {
            result = i;
            break;
        }
    }

    return result;
}

// Sentinel Linear Search (поиск элемента в массиве)
int SLS(int key, 
    int size, 
    std::vector<int>& vec,
    int& counter1, int& counter2)
{
    int last = vec[size - 1];
    vec[size - 1] = key;

    int i = 0;
    while (vec[i] != key) 
    {
        ++i;
        ++counter1;
    }
    vec[size - 1] = last;

    if (vec[i] == key) return i;
    else return -1;
}

// Ordered Array Search (поиск элемента в ОТСОРТИРОВАННОМ массиве)
int OAS(int key, 
    int size, 
    const std::vector<int>& vec,
    int& counter1, int& counter2)
{
    int result = -1;
    for (int i = 0; i < size; ++i)
    {
        ++counter1;
        ++counter2;
        if (vec[i] >= key)
        {
            ++counter1;
            if (vec[i] == key) result = i;
            break;
        }
    }

    return result;
}

// Binary Search (поиск элемента в ОТСОРТИРОВАННОМ массиве)
int BS(int key, 
    int size,
    const std::vector<int>& vec,
    int& counter1, int& counter2)
{
    int left = 0, right = size - 1;

    while (left <= right)
    {
        ++counter2;
        int mid = left + (right - left) / 2;
        if (vec[mid] == key)
        {
            ++counter1;
            return mid;
        }
        else if (vec[mid] < key)
        {
            counter1 += 2;
            left = mid + 1;
        }
        else
        {
            counter1 += 2;
            right = mid - 1;
        }
    }

    return -1;
}

int main()
{
    // Инициализация ГПСЧ 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> randint(MIN_NUM, MAX_NUM);

    // Неотсортированный массив
    std::vector<int> vec(MAX_SIZE, 0);

    for (int i = 0; i < MAX_SIZE; ++i)
    {
        vec[i] = randint(gen);
    }

    // Отсортированный массив
    std::vector<int> sort_vec(vec.begin(), vec.end());
    std::sort(sort_vec.begin(), sort_vec.end());

    // Результаты поиска
    std::vector<int> results(4, -1);

    // Счётчики (counter1 и counter2)
    std::vector<std::pair<int, int>> counters(4, { 0, 0 });

    for (const int size : SIZES)
    {

        std::vector<int> keys = {
            // ключи для поиска в неотсортированном массиве
            vec[0],
            vec[size / 2],
            vec[size - 1],
            // ключи для поиска в ОТСОРТИРОВАННОМ массиве
            sort_vec[0],
            sort_vec[size / 2],
            vec[size - 1],
        };

        for (int i = 0; i < 3; ++i)
        {
            int key = keys[i];
            int sort_key = keys[i + 3];

            // Замер времени работы алгоритмов поиска
            auto time_BLS = TimeDuration(BLS, results[0], 
                key, size, vec, counters[0].first, counters[0].second);
            auto time_SLS = TimeDuration(SLS, results[1],
                key, size, vec, counters[1].first, counters[1].second);
            auto time_OAS = TimeDuration(OAS, results[2],
                sort_key, size, sort_vec, counters[2].first, counters[2].second);
            auto time_BS = TimeDuration(BS, results[3],
                sort_key, size, sort_vec, counters[3].first, counters[3].second);

            // Вывод результатов в консоль
            print_table_header();
            print_table_row("BLS", size, i+1, time_BLS.count(), counters[0]);
            print_table_row("SLS", size, i+1, time_SLS.count(), counters[1]);
            print_table_row("OAS", size, i+1, time_OAS.count(), counters[2]);
            print_table_row("BS", size, i+1, time_BS.count(), counters[3]);
            std::cout << std::endl;

            // сброс счётчиков 
            for (int i = 0; i < 4; ++i)
            {
                counters[i] = { 0, 0 };
            }
        }
    }
}