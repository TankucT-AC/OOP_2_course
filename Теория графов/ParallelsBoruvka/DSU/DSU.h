#pragma once

#include "Graph.h"

#include <unordered_map>
#include <atomic>
#include <mutex>

class DSU
{
private:
	mutable std::mutex mtx;

	std::unordered_map<int, int> parent;
	std::unordered_map<int, int> rank;
	std::atomic<int> components_count = 0;
public:
	DSU(const Graph& graph);

	int count() const;
	int find_set(int u);
	void union_sets(int u, int v);
};