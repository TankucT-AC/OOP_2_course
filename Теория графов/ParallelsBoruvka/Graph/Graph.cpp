// Реализация методов класса Graph

#include "Graph.h"

#include <unordered_set>
#include <iostream>
#include <iomanip>
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

Graph::Graph(const std::wstring& path, bool is_orient)
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

size_t Graph::size() const { return count_vertex; }

int Graph::weight(int u, int v)
{
	if (is_edge(u, v))
		return graph[u][v];

	throw std::runtime_error("This edge is not found!");
}

bool Graph::is_edge(int u, int v)
{
	if (graph[u].find(v) != graph[u].end())
		return true;

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

	graph[u][v] = w;
	count_edges++;

	if (!is_orient)
	{
		graph[v][u] = w;
		count_edges++;
	}
}

void Graph::list_of_edges(int u)
{
	std::cout << "Vertex " << u << " -> ";
	if (graph[u].empty())
	{
		std::cout << "No edges\n";
		return;
	}

	for (const auto& [v, w] : graph[u])
	{
		std::cout << v << "(" << w << ") ";
	}
	std::cout << std::endl;
}

void Graph::list_of_edges()
{
	if (!count_vertex)
	{
		std::cout << "Graph is empty!\n";
		return;
	}

	std::cout << "Graph adjacency list:\n";
	std::cout << "====================\n";
	for (const auto& [u, _] : graph)
	{
		list_of_edges(u);
	}
	std::cout << "====================\n";
}

std::vector<int> Graph::list_of_vertex() const
{
	std::vector<int> result;

	for (const auto& [v, _] : graph)
	{
		result.push_back(v);
	}

	return result;
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
		std::cout << to << ": ";
		if (dist[to] == MAX) std::cout << "No edges\n";
		else std::cout << dist[to] << "\n";
	}
}

Graph Graph::MST_Prim(int start)
{
	if (is_orient)
		throw std::runtime_error("The algorithm searches for a MST only in an undirected graph!");

	if (graph.find(start) == graph.end())
		throw std::runtime_error("This start vertex is not found in graph!");

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