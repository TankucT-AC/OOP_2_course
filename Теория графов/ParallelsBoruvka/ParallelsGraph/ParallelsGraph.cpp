#include "ParallelsGraph.h"
#include "DSU.h"

#include <tbb/tbb.h>

ParallelsGraph::ParallelsGraph() : Graph() {}
ParallelsGraph::ParallelsGraph(bool is_orient) : Graph(is_orient) {}
ParallelsGraph::ParallelsGraph(const std::wstring& path, bool is_orient) : Graph(path, is_orient) {}

ParallelsGraph ParallelsGraph::MST_BoruvkaParallels(int start)
{
    if (is_orient) 
        throw std::runtime_error("The algorithm searches for a MST only in an undirected graph!");

    DSU dsu(*this);
    ParallelsGraph MST(is_orient);

    // local_arena нужна для ограничения потоков
    tbb::task_arena local_arena(thread_count);

    tbb::concurrent_hash_map<int, tbb::concurrent_vector<int>> min_edges;

    while (dsu.count() > 1)
    {
        // Лямбда-функция для параллельного добавления минимальных ребер
        auto concurrent_min_edges_processing = [&](const auto& vertex_pair) 
        {
            int v = vertex_pair.first;
            const auto& edges = vertex_pair.second;

            for (const auto& [u, w] : edges)
            {
                int comp_u = dsu.find_set(u);
                int comp_v = dsu.find_set(v);

                if (comp_u == comp_v) continue;

                // Функция для атомарного обновления минимального ребра
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

        // Параллельно вычисляем минимальные ребра с строго ограниченным пулом потоков
        local_arena.execute([&] {
            tbb::parallel_for_each(graph.begin(), graph.end(), concurrent_min_edges_processing);
        });
        
        // Остальное параллелить нет смыслы :)
        std::vector<std::tuple<int, int, int>> edges_to_add;

        for (auto it = min_edges.begin(); it != min_edges.end(); ++it) {
            int comp_id = it->first;
            auto& edge_vec = it->second;

            if (edge_vec.size() < 3) continue;

            int u = edge_vec[0];
            int v = edge_vec[1];
            int w = edge_vec[2];

            int comp_u = dsu.find_set(u);
            int comp_v = dsu.find_set(v);

            if (comp_u != comp_v) {
                edges_to_add.emplace_back(u, v, w);
            }
        }

        for (auto& [u, v, w] : edges_to_add) {
            int comp_u = dsu.find_set(u);
            int comp_v = dsu.find_set(v);

            if (comp_u != comp_v) {
                MST.add_edge(u, v, w);
                dsu.union_sets(comp_u, comp_v);
            }
        }

        min_edges.clear();
    }

    return MST;
}