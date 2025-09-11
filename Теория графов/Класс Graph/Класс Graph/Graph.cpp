// Реализация методов класса Graph

#include "Graph.h"
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>

Graph::Graph() : count_edges(0), count_vertex(0), is_orient(false)
{
	graph = {};
}

Graph::Graph(bool is_orient) : count_edges(0), count_vertex(0)
{
	graph = {};
	this->is_orient = is_orient;
}

Graph::Graph(const std::string& path, bool is_orient)
{
	this->is_orient = is_orient;
	count_vertex = 0;
	count_edges = 0;
	graph = {};

	std::ifstream fin(path);
	if (!fin.is_open())
	{
		std::cout << "WARNING: File is not found!\n";
		return;
	}

	if (fin.peek() == EOF)
	{
		std::cout << "WARNING: File is empty!\n";
		fin.close();
		return;
	}

	std::string line;
	while (std::getline(fin, line))
	{
		std::istringstream iss(line);
		int u, v, w;
		iss >> u >> v >> w;

		// Проверка на то, что данных хватает в файле
		if (iss.fail()) 
		{
			graph.clear();
			throw std::runtime_error("File data is not correct!"); 
		}

		// Проверка на избыток данных
		int temp;
		iss >> temp;
		if (!iss.fail()) 
		{
			graph.clear();
			throw std::runtime_error("File data is not correct!");
		}

		add_edge(u, v, w);
	}
}

Graph::~Graph() 
{
	graph.clear();
}

size_t Graph::size() { return count_vertex; }

int Graph::weight(int u, int v)
{
	for (const auto& [to, weight] : graph[u])
	{
		if (to == v) return weight;
	}

	throw std::runtime_error("This edge is not found!");
}

bool Graph::is_edge(int u, int v)
{
	for (const auto& [to, _] : graph[u])
	{
		if (to == v) return true;
	}

	return false;
}

void Graph::add_vertex(int u)
{
	if (graph.find(u) != graph.end()) return;

	graph[u] = {};
	++count_vertex;
}

void Graph::add_edge(int u, int v, int w)
{
	add_vertex(u);
	add_vertex(v);

	// Если ребра не существует, то добавляем его и выходим
	if (!is_edge(u, v))
	{
		graph[u].push_back({ v, w });
		if (!is_orient) graph[v].push_back({ u, w });
		++count_edges;
		return;
	}

	// Если ребро есть, обновляем значение
	for (auto& [to, weight] : graph[u])
	{
		if (to == v)
		{
			weight = w;
			break;
		}
	}
	++count_edges;

	// Если граф ориентированный, то обновлять v->u нельзя
	if (is_orient) return;

	for (auto& [to, weight] : graph[v])
	{
		if (to == u)
		{
			weight = w;
			break;
		}
	}
	++count_edges;
}

void Graph::list_of_edges(int u)
{
	std::cout << u << ":\n  ";
	for (const auto& [v, w] : graph[u])
	{
		std::cout << "(" << v << ", " << w << ") ";
	}
	std::cout << std::endl;
}

void Graph::list_of_edges()
{
	for (const auto& [u, _] : graph)
	{
		list_of_edges(u);
	}
}

void Graph::FordBellman(int start)
{
	if (graph.find(start) == graph.end()) 
		throw std::runtime_error("This start vertex is not found in graph!");

	const int MAX = 1e9;

	struct Edge
	{
		int u, v, w;
	};

	std::vector<Edge> edges;
	for (const auto& [u, eds] : graph)
	{
		for (const auto& [v, w] : eds)
		{
			edges.push_back({ u, v, w });
		}
	}

	std::unordered_map<int, int> dist;
	for (const auto& [to, _] : graph)
	{
		dist[to] = MAX;
	}
	dist[start] = 0;

	for (int i = 0; i < count_vertex - 1; ++i)
	{
		for (const auto& [u, v, w] : edges)
		{
			if (dist[u] != MAX && dist[v] > dist[u] + w)
				dist[v] = dist[u] + w;

			if (!is_orient && dist[v] != MAX && dist[u] > dist[v] + w)
				dist[u] = dist[v] + w;
		}
	}

	for (const auto& [to, _] : graph)
	{
		std::cout << to << ": " << dist[to] << '\n';
	}
}

Graph Graph::MST_Prim(int start)
{
	Graph MST(is_orient);
	std::unordered_set<int> T;

	std::priority_queue<std::pair<int, std::pair<int, int>>,
		std::vector<
			std::pair<int, std::pair<int, int>>
		>,
		std::greater<
			std::pair<int, std::pair<int, int>>>
	> pq;

	for (const auto& [v, w] : graph[start])
	{
		pq.push({ w, {start, v} });
	}
	T.insert(start);

	while (!pq.empty() && T.size() < count_edges)
	{
		auto curr = pq.top();
		pq.pop();

		int w = curr.first;
		int u = curr.second.first;
		int v = curr.second.second;

		if (T.find(v) != T.end()) continue;

		T.insert(v);
		MST.add_edge(u, v, w);

		for (const auto& [next_v, next_w] : graph[v])
		{
			if (T.find(next_v) != T.end()) continue;

			pq.push({ next_w, {v, next_v} });
		}
	}

	return MST;
}