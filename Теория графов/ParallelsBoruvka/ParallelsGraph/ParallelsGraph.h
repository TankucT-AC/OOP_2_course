#pragma once

#include "Graph.h"

#include <chrono>

using namespace std::chrono_literals;

class ParallelsGraph : public Graph
{
private:
	int thread_count = 4;
	std::chrono::microseconds duration = 0ms;
	int edges_sum = 0;
public:
	ParallelsGraph();
	ParallelsGraph(bool is_orient, int init_thread_count);
	ParallelsGraph(const std::string& path, bool is_orient, int init_thread_count);

	ParallelsGraph MST_BoruvkaParallels();
	void SetEdgesSum(int new_sum);
	void SetDuration(std::chrono::microseconds duration);
	void write_graph(const std::string& out);
};