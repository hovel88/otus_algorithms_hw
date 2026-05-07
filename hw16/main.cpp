#include "weighted_graph.h"

WeightedGraph create_g1()
{
    WeightedGraph g(4);
    g.add_edge(0, 1, 1);
    g.add_edge(0, 2, 4);
    g.add_edge(0, 3, 3);
    g.add_edge(1, 2, 2);
    g.add_edge(1, 3, 5);
    g.add_edge(2, 3, 6);
    return g;
}

WeightedGraph create_g2()
{
    WeightedGraph g(7);
    g.add_edge(0, 1, 2);
    g.add_edge(0, 2, 2);
    g.add_edge(0, 4, 1);
    g.add_edge(0, 6, 3);
    g.add_edge(1, 2, 3);
    g.add_edge(1, 3, 3);
    g.add_edge(3, 4, 2);
    g.add_edge(4, 5, 4);
    g.add_edge(5, 6, 2);
    return g;
}

void print_MST(const std::vector<Edge>& mst, bool is_spanning_tree)
{
    if (!is_spanning_tree) std::cout << "внимание, граф несвязный!:\n";
    else                   std::cout << "минимальное остовое (покрывающее) дерево (MST):\n";

    if (mst.empty()) {
        std::cout << "  (пусто)\n";
        return;
    }

    int total = 0;
    for (const Edge& e : mst) {
        e.print();
        total += e.weight;
    }
    std::cout << "общий вес: " << total << "\n";
}

int main(int argc, char** argv)
{
    std::cout << "Программа для демонстрации алгоритмов нахождения минимального остового дерева в графах\n";

    struct graphs {
        std::string   name{};
        WeightedGraph graph{};
    };
    std::vector<graphs> collection = {{
        {"простой граф (4 вершины)",  create_g1()},
        {"граф из лекции (7 вершин)", create_g2()}
    }};

    for (size_t idx = 0; idx < collection.size(); ++idx) {
        std::cout << "\n" << std::string(70, '=') << "\n";
        std::cout << "ТЕСТ: " << collection[idx].name;
        std::cout << "\n" << std::string(70, '=') << "\n";

        auto& graph = collection[idx].graph;
        graph.print();

        {
            std::cout << "\n--- АЛГОРИТМ КРАСКАЛА ---\n\n";
            auto [is_spanning_tree, mst] = graph.kruskal_mst();
            print_MST(mst, is_spanning_tree);
        }

        {
            std::cout << "\n--- АЛГОРИТМ БОРУВКИ ---\n\n";
            auto [is_spanning_tree, mst] = graph.boruvka_mst();
            print_MST(mst, is_spanning_tree);
        }

        {
            std::cout << "\n--- АЛГОРИТМ ПРИМА ---\n\n";
            auto [is_spanning_tree, mst] = graph.prim_mst(0);
            print_MST(mst, is_spanning_tree);
        }
    }

    return 0;
}
