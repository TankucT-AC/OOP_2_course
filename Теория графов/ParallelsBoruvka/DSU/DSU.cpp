#include "DSU.h"
#include "Graph.h"

DSU::DSU(const Graph& graph)
{
	components_count = 0;
	auto vertexes = graph.list_of_vertex();
	for (const auto v : vertexes)
	{
		parent[v] = v;
		rank[v] = 1;
		++components_count;
	}
}

int DSU::count() const { return components_count; }

int DSU::find_set(int u)
{
	if (parent[u] != u)
		parent[u] = find_set(parent[u]);

	return parent[u];
}

void DSU::union_sets(int u, int v)
{
	u = find_set(u);
	v = find_set(v);
	if (u == v) return;

	--components_count;

	if (rank[u] < rank[v])
	{
		parent[u] = v;
		rank[v] += rank[u];
	}
	else
	{
		parent[v] = u;
		rank[u] = v;
	}
}