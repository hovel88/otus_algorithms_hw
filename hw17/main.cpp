#include "weighted_graph_sp.h"

WeightedGraphSP create_g1()
{
    WeightedGraphSP g(4);
    g.add_edge(0, 1, -2);
    g.add_edge(0, 2, 5);
    g.add_edge(0, 3, 7);
    g.add_edge(1, 2, 6);
    g.add_edge(1, 3, 8);
    g.add_edge(2, 0, -1);
    g.add_edge(3, 1, 3);
    g.add_edge(3, 2, -4);
    return g;
}

WeightedGraphSP create_g2()
{
    WeightedGraphSP g(4);
    g.add_edge(0, 1, 5);
    g.add_edge(0, 2, 10);
    g.add_edge(1, 2, 3);
    g.add_edge(2, 3, 1);
    g.add_edge(1, 3, 8);
    return g;
}

WeightedGraphSP create_g3()
{
    WeightedGraphSP g(3);
    g.add_edge(0, 1, 1);
    g.add_edge(1, 2, -3);
    g.add_edge(2, 0, -1); // 0 -> 1 -> 2 -> 0: 1 + (-3) + (-1) = -3
    return g;
}

int main(int argc, char** argv)
{
    std::cout << "Программа для демонстрации алгоритмов нахождения кратчайшего пути в графах\n";

    struct graphs {
        std::string     name{};
        WeightedGraphSP graph{};
    };
    std::vector<graphs> collection = {{
        {"граф из лекции (4 вершины)",               create_g1()},
        {"граф (4 вершины) без отрицательных весов", create_g2()},
        {"граф (3 вершины) c отрицательным циклом",  create_g3()}
    }};

    for (size_t idx = 0; idx < collection.size(); ++idx) {
        std::cout << "\n" << std::string(70, '=') << "\n";
        std::cout << "ТЕСТ: " << collection[idx].name;
        std::cout << "\n" << std::string(70, '=') << "\n";

        auto& graph = collection[idx].graph;
        graph.print();

        {
            std::cout << "\n--- АЛГОРИТМ ФЛОЙДА-УОРШАЛЛА ---\n\n";
            auto result = graph.floyd_warshall_sp();
            if (!result.has_negative_cycle) {
                result.print_paths();
            } else {
                std::cout << "кратчайшие пути не определены: алгоритм обнаружил цикл с отрицательным весом!\n";
            }
            result.print_matrix();
        }
        {
            std::cout << "\n--- АЛГОРИТМ БЕЛЛМАНА-ФОРДА ---\n\n";
            // алгоритм работает для поиска из конкретной вершины-источника
            // поэтому запустим для нескольких источников
            std::vector<int> sources = {0, 1, (graph.get_vertices_count() - 1)};
            for (int src : sources) {
                if (src >= graph.get_vertices_count()) continue;
                auto result = graph.bellman_ford_sp(src);
                if (!result.has_negative_cycle) {
                    result.print_paths();
                } else {
                    std::cout << "кратчайшие пути не определены: алгоритм обнаружил цикл с отрицательным весом!\n";
                }
                result.print_matrix_row();
            }
        }
        {
            std::cout << "\n--- АЛГОРИТМ ДЕЙКСТРЫ ---\n\n";
            if (graph.has_negative_weights()) {
                std::cout << "кратчайшие пути не определены: алгоритм не работает с графами с отрицательными весами!\n";
            } else {
                // алгоритм работает для поиска из конкретной вершины-источника
                // поэтому запустим для нескольких источников
                std::vector<int> sources = {0, 1, (graph.get_vertices_count() - 1)};
                for (int src : sources) {
                    if (src >= graph.get_vertices_count()) continue;
                    auto result = graph.deykstra_sp(src);
                    result.print_paths();
                    result.print_matrix_row();
                }
            }
        }
    }

    return 0;
}
