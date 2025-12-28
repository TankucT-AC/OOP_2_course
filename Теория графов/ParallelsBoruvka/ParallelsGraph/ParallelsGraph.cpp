#include "ParallelsGraph.h"
#include "DSU.h"

#include <tbb/tbb.h>
#include <fstream>
#include <iostream>

ParallelsGraph::ParallelsGraph() : Graph() {}

ParallelsGraph::ParallelsGraph(bool is_orient, int init_thread_count) : Graph(is_orient) 
{
    thread_count = init_thread_count;
}

ParallelsGraph::ParallelsGraph(const std::string& path, bool is_orient, int init_thread_count) : Graph(path, is_orient) 
{
    thread_count = init_thread_count;
}

void ParallelsGraph::write_graph(const std::string& out)
{
    std::ofstream outFile(out);
    outFile << "Opening hours: " << duration.count() << " .ns" << "\n";
    outFile << "Threads count: " << thread_count << "\n";
    outFile << "Edges sum: " << edges_sum << "\n\n";

    std::unordered_map<int, std::unordered_set<int>> used;
    for (const auto& [u, edges] : graph)
    {
        if (used.find(u) == used.end()) used[u] = {};
        for (const auto& [v, w] : edges)
        {
            if (used[u].find(v) == used[u].end())
            {
                outFile << u << " " << v << " " << w << "\n";
                used[u].insert(v);
                used[v].insert(u);
            }
        }
    }
    outFile.close();
}

void ParallelsGraph::SetEdgesSum(int new_sum)
{
    edges_sum = new_sum;
}

void ParallelsGraph::SetDuration(std::chrono::microseconds new_duration)
{
    duration = new_duration;
}

ParallelsGraph ParallelsGraph::MST_BoruvkaParallels()
{

    if (is_orient) throw std::runtime_error("This graph is directed!");
    if (count_vertex == 0) throw std::runtime_error("This graph is empty!");
    if (!isOnlyCC()) throw std::runtime_error("This graph is not connected!");

    auto start = std::chrono::high_resolution_clock::now();
    DSU dsu(*this);
    ParallelsGraph MST(is_orient, thread_count);
    int curr_MST_edges_sum = 0;

    // local_arena ограничивает число потоков
    tbb::task_arena local_arena(thread_count);

    tbb::concurrent_hash_map<int, tbb::concurrent_vector<int>> min_edges;

    while (dsu.count() > 1)
    {
        // функция для потокобезопасной вставки минимальных ребер
        auto concurrent_min_edges_processing = [&](const auto& vertex_pair) 
        {
            int v = vertex_pair.first;
            const auto& edges = vertex_pair.second;

            for (const auto& [u, w] : edges)
            {
                int comp_u = dsu.find_set(u);
                int comp_v = dsu.find_set(v);

                if (comp_u == comp_v) continue;

                // функция обновления минимальных ребер
                auto update_min_edge = [&](int comp) {
                    tbb::concurrent_hash_map<int, tbb::concurrent_vector<int>>::accessor acc;

                    if (min_edges.insert(acc, comp)) {
                        acc->second = { u, v, w };
                    }
                    else {
                        if (w < acc->second[2]) {
                            acc->second = { u, v, w };
                        }
                    }
                };

                update_min_edge(comp_u);
                update_min_edge(comp_v);
            }
        };

        // непосредственно часть с параллельным добавлением минимальных ребер
        local_arena.execute([&] {
            tbb::parallel_for_each(graph.begin(), graph.end(), concurrent_min_edges_processing);
        });
        
        // остальное параллелить нет смысла ввиду избыточных накладных расходов :)
        std::vector<std::tuple<int, int, int>> edges_to_add;

        for (auto it = min_edges.begin(); it != min_edges.end(); ++it) {
            int comp_id = it->first;
            auto& edge_vec = it->second;

            int u = edge_vec[0];
            int v = edge_vec[1];
            int w = edge_vec[2];

            edges_to_add.emplace_back(u, v, w);
        }

        for (auto& [u, v, w] : edges_to_add) {
            int comp_u = dsu.find_set(u);
            int comp_v = dsu.find_set(v);

            if (comp_u != comp_v) {
                MST.add_edge(u, v, w);
                curr_MST_edges_sum += w;
                dsu.union_sets(comp_u, comp_v);
            }
        }

        min_edges.clear();
    }

    auto end = std::chrono::high_resolution_clock::now();

    // замеряем время выполнения программы
    auto new_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    MST.SetDuration(new_duration);
    MST.SetEdgesSum(curr_MST_edges_sum);

    return MST;
}