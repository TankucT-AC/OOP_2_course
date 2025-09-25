#include "DSU.h"
#include "Graph.h"

DSU::DSU(const Graph& graph)
{

	auto vertexes = graph.list_of_vertex();
	for (const auto v : vertexes)
	{
		parent[v] = v;
		rank[v] = 1;
		components_count += 1;
	}
}

int DSU::count() const 
{
	std::lock_guard<std::mutex> lock(mtx);
	return components_count.load(); 
}

int DSU::find_set(int u)
{
	std::lock_guard<std::mutex> lock(mtx);

	int root = u;
	while (parent[root] != root) 
	{
		root = parent[root];
	}

	int temp = u;
	while (parent[temp] != root) 
	{
		int next = parent[temp];
		parent[temp] = root;
		temp = next;
	}

	return root;
}

void DSU::union_sets(int u, int v)
{
	u = find_set(u);
	v = find_set(v);

	std::lock_guard<std::mutex> lock(mtx);

	if (u == v) return;

	components_count -= 1;

	if (rank[u] < rank[v])
	{
		parent[u] = v;
		rank[v] += rank[u];
	}
	else
	{
		parent[v] = u;
		rank[u] += rank[v];
	}
}