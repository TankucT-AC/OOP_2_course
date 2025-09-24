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

    Graph& ref_this = *this;
    DSU dsu(ref_this);
    ParallelsGraph MST(is_orient);

    tbb::task_arena local_arena(thread_count);
    tbb::concurrent_hash_map<int, tbb::concurrent_vector<int>> min_edges;

    while (dsu.count() > 1)
    {
        auto concurrent_min_edges_processing = [&](const tbb::blocked_range<size_t>& range)
            {
                for (size_t i = range.begin(); i != range.end(); ++i)
                {
                    // Получаем итератор к i-ой вершине
                    auto it = graph.begin();
                    std::advance(it, i);
                    int v = it->first;
                    auto& edges = it->second;

                    for (auto& [u, w] : edges)
                    {
                        int comp_u = dsu.find_set(u);
                        int comp_v = dsu.find_set(v);

                        if (comp_u == comp_v) continue;

                        // Безопасное обновление минимальных ребер
                        tbb::concurrent_hash_map<int, tbb::concurrent_vector<int>>::accessor acc;

                        // Для компоненты comp_u
                        if (min_edges.find(acc, comp_u))
                        {
                            // Сравниваем с текущим минимальным ребром
                            if (w < acc->second[2])
                            {
                                acc->second[0] = u;
                                acc->second[1] = v;
                                acc->second[2] = w;
                            }
                        }
                        else
                        {
                            // Вставляем новое ребро
                            tbb::concurrent_vector<int> new_edge;
                            new_edge.push_back(u);
                            new_edge.push_back(v);
                            new_edge.push_back(w);
                            min_edges.insert({ comp_u, new_edge });
                        }
                        acc.release();

                        // Для компоненты comp_v
                        if (min_edges.find(acc, comp_v))
                        {
                            if (w < acc->second[2])
                            {
                                acc->second[0] = u;
                                acc->second[1] = v;
                                acc->second[2] = w;
                            }
                        }
                        else
                        {
                            tbb::concurrent_vector<int> new_edge;
                            new_edge.push_back(u);
                            new_edge.push_back(v);
                            new_edge.push_back(w);
                            min_edges.insert({ comp_v, new_edge });
                        }
                    }
                }
            };

        local_arena.execute([&] {
            tbb::parallel_for(tbb::blocked_range<size_t>(0, graph.size()), concurrent_min_edges_processing);
        });
        

        // Последовательно добавляем ребра в MST (этап должен быть последовательным)
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

        // Добавляем ребра и объединяем компоненты
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