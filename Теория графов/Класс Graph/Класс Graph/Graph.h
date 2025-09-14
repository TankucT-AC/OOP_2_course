// В этом файле будет находится макет класса Graph

#pragma once

#include <string>
#include <unordered_map>

class Graph
{
private:
	size_t count_vertex;
	size_t count_edges;
	bool is_orient;

	std::unordered_map<int, std::unordered_map<int, int>> graph;

public:
	Graph();
	Graph(bool is_orient);
	Graph(const std::wstring& path, bool is_orient);
	~Graph();

	size_t size() const;
	int weight(int u, int v);
	bool is_edge(int u, int v);
	void add_vertex(int u);
	void add_edge(int u, int v, int w);
	void list_of_edges();
	void list_of_edges(int u);

	// Специализация: 3b и 4b:
	void FordBellman(int start);
	Graph MST_Prim(int start);
};
