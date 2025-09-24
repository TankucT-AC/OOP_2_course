#pragma once

#include "Graph.h"
#include <unordered_map>

class DSU
{
private:
	std::unordered_map<int, int> parent;
	std::unordered_map<int, int> rank;
	int components_count;
public:
	DSU(const Graph& graph);

	int count() const;
	int find_set(int u);
	void union_sets(int u, int v);
};