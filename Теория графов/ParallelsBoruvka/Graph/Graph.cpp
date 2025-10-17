#include "Graph.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>

// Я класть хотел на принцип DRY, уж извините

void Graph::dfs(int u, std::unordered_set<int>& used)
{
	used.insert(u);
	for (const auto& [v, w] : graph[u])
	{
		if (used.find(v) == used.end())
			dfs(v, used);
	}
}

bool Graph::isOnlyCC()
{
	std::unordered_set<int> used;
	auto count = 0;
	for (const auto& [u, edge] : graph)
	{
		if (used.find(u) == used.end())
		{
			dfs(u, used);
			++count;
		}

		if (count > 1) return false;
	}

	return true;
}


Graph::Graph() : count_edges(0), count_vertex(0), is_orient(false) {}

Graph::Graph(bool is_orient) : count_edges(0), count_vertex(0)
{
	this->is_orient = is_orient;
}

Graph::Graph(const std::string& path, bool is_orient) : count_edges(0), count_vertex(0)
{
	this->is_orient = is_orient;

	std::ifstream fin(path);
	if (!fin.is_open())
	{
		throw std::runtime_error("ERROR: File is not found!");
	}

	if (fin.peek() == EOF)
	{
		fin.close();
		throw std::runtime_error("ERROR: File is empty!");
	}

	std::string line;
	while (std::getline(fin, line))
	{
		std::istringstream iss(line);
		int u, v, w;
		iss >> u >> v >> w;

		if (iss.fail())
		{
			graph.clear();
			throw std::runtime_error("File data is not correct!");
		}

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

Graph::~Graph() {}

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

	const int INF = 1e9;

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
		dist[to] = INF;
	}
	dist[start] = 0;

	for (int i = 0; i < count_vertex - 1; ++i)
	{
		for (const auto& [u, v, w] : edges)
		{
			if (dist[u] != INF && dist[v] > dist[u] + w)
				dist[v] = dist[u] + w;

			if (!is_orient && dist[v] != INF && dist[u] > dist[v] + w)
				dist[u] = dist[v] + w;
		}
	}

	for (const auto& [u, v, w] : edges)
	{
		if (dist[u] != INF && dist[v] > dist[u] + w)
			throw std::runtime_error("There is a negative cycle in the graph!");

		if (!is_orient && dist[v] != INF && dist[u] > dist[v] + w)
			throw std::runtime_error("There is a negative cycle in the graph!");
	}

	for (const auto& [to, _] : graph)
	{
		std::cout << to << ": ";
		if (dist[to] == INF) std::cout << "No edges\n";
		else std::cout << dist[to] << "\n";
	}
}

Graph Graph::MST_Prim()
{
	if (is_orient) throw std::runtime_error("This graph is directed!");
	if (count_vertex == 0) throw std::runtime_error("This graph is empty!");
	if (!isOnlyCC()) throw std::runtime_error("This graph is not connected!");

	Graph MST(is_orient);
	std::unordered_set<int> T;

	auto start = graph.begin()->first;
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