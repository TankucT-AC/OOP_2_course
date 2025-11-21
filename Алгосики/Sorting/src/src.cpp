#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <chrono>

using ll = long long;

int MIN_NUM = -1000;
int MAX_NUM = 1000;
int MAX_SIZE = 100'000;
const std::vector<int> SIZES = { 500, 1'000, 10'000, 50'000, 100'000 };

// Функция вывода заголовка таблицы
void print_table_header() {
	std::cout << std::left;
	std::cout << std::setw(10) << "Алгоритм" << "   | "
		<< std::right << std::setw(8) << "Размер" << "   | "
		<< std::right << std::setw(12) << "Время(мкс)" << "   | "
		<< std::setw(12) << "Counter1" << " | "
		<< std::setw(12) << "Counter2" << "\n";

	std::cout << std::setw(10) << std::string(10, '-') << "-|-"
		<< std::setw(8) << std::string(8, '-') << "-|-"
		<< std::setw(12) << std::string(12, '-') << "-|-"
		<< std::setw(12) << std::string(12, '-') << "-|-"
		<< std::setw(12) << std::string(12, '-') << "\n";
}

// Функция вывода строки таблицы
void print_table_row(const std::string& algorithm,
	int size,
	long long time,
	const std::pair<ll, ll>& counter)
{
	std::cout << std::left;
	std::cout << std::setw(10) << algorithm << " | "
		<< std::right << std::setw(8) << size << " | "
		<< std::right << std::setw(12) << time << " | "
		<< std::setw(12) << counter.first << " | "
		<< std::setw(12) << counter.second << "\n";
}

// Функция замера времени выполнения программы
template <typename Func, typename... Args>
auto TimeDuration(Func f, Args&&... args)
{
	auto start = std::chrono::high_resolution_clock::now();
	f(args...);
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	return duration;
}


void InsertionSort(std::vector<int>& vec, const int n, ll& counter1, ll& counter2)
{
	for (int i = 1; i < n; ++i)
	{
		++counter1;
		int curr = i;
		int prev = i - 1;
		while (curr > 0 && vec[prev] > vec[curr])
		{
			++counter1;
			++counter2;
			std::swap(vec[prev], vec[curr]);
			prev--;
			curr--;
		}
		++counter1;
		++counter2;
	}
}

int partition(std::vector<int>& vec, int left, int right, ll& counter1, ll& counter2)
{
	int pivot = vec[right];
	
	int i = left;
	int j = right;
	while (i <= j)
	{
		while (vec[i] < pivot) 
		{
			++i;
			++counter1;
		}

		while (vec[j] > pivot) 
		{
			--j;
			++counter1;
		}

		++counter1;
		if (i > j) break;

		std::swap(vec[i], vec[j]);
		++counter2;

		++i;
		--j;
	}
	
	return j;
}

void QuickSort(std::vector<int>& vec, int left, int right, ll& counter1, ll& counter2)
{
	if (left >= right) return;

	int index = partition(vec, left, right, counter1, counter2);

	QuickSort(vec, left, index, counter1, counter2);
	QuickSort(vec, index + 1, right, counter1, counter2);
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

    // Счётчики (counter1 и counter2)
    std::vector<std::pair<ll, ll>> counters(2, { 0, 0 });

    for (const int size : SIZES)
    {
		std::vector<int> InsertionVec(vec.begin(), vec.begin() + size);
		std::vector<int> QuickVec(vec.begin(), vec.begin() + size);

		// Замер времени работы алгоритмов поиска
		auto time_Insertion = TimeDuration(InsertionSort, InsertionVec, size, counters[0].first, counters[0].second);
		auto time_Quick = TimeDuration(QuickSort, QuickVec, 0, size - 1, // потому что [left, right]
			counters[1].first, counters[1].second);

		// Вывод результатов в консоль
		print_table_header();
		print_table_row("InserSort", size, time_Insertion.count(), counters[0]);
		print_table_row("QuickSort", size, time_Quick.count(), counters[1]);
		std::cout << std::endl;

		// сброс счётчиков 
		for (int i = 0; i < 2; ++i)
		{
			counters[i] = { 0, 0 };
		}
    }
}
