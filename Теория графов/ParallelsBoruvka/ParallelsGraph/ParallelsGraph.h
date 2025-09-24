#pragma once

#include "Graph.h"

class ParallelsGraph : public Graph
{
private:
	int thread_count = 4;
public:
	ParallelsGraph();
	ParallelsGraph(bool is_orient);
	ParallelsGraph(const std::wstring& path, bool is_orient);

	ParallelsGraph MST_BoruvkaParallels(int start);
};