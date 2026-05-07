#pragma once

#include <iostream>
#include <tuple>
#include <climits>

#include "union_find.h"

struct Edge {
    int beg{-1};         // первая вершина
    int end{-1};         // вторая вершина
    int weight{INT_MAX}; // вес ребра

    Edge() = default;
    Edge(int u, int v, int w)
    :   beg(u),
        end(v),
        weight(w)
    {}

    void print() const {
        std::cout << " (" << beg << " - " << end << ", w=" << weight << ")\n";
    }

    bool valid() const {
        return (beg >= 0) && (end >= 0);
    }
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class WeightedGraph {
private:
    // --------------------------------------------------------
    // 5. Векторы смежности.
    //    матрица (двумерный массив) M[V][Smax], в которой:
    //    строки  - это вершины графа (их количество V),
    //    столбцы - список номеров соседних вершин, с которыми есть соединение рёбрами.
    //    в случае взвешенного графа, хранится пара {номер вершины, вес ребра}.
    //    также в этот раз будем не передавать ступенчатый массив смежности
    //    в конструктор, а будем добавлять вершины со связями и весами методом
    // --------------------------------------------------------
    std::vector<std::vector<std::pair<int, int>>> mx{};    // mx[u] = {v, weight}
    int                                           V{0};    // количество вершин
    std::vector<Edge>                             edges{}; // список всех рёбер (для удобства)

public:
    ~WeightedGraph() = default;
    WeightedGraph() = default;
    WeightedGraph(const WeightedGraph&) = default;
    WeightedGraph(WeightedGraph&&) = default;
    WeightedGraph& operator=(const WeightedGraph&) = default;
    WeightedGraph& operator=(WeightedGraph&&) = default;

    explicit WeightedGraph(int vertices)
    :   V(vertices) {
        if (V < 0) throw std::invalid_argument("количество вершин не может быть отрицательным");
        mx.resize(V);
    }

    void add_edge(int u, int v, int weight) {
        if (u < 0 || u >= V) {
            throw std::out_of_range("add_edge: вершина u=" + std::to_string(u) + " вне диапазона [0, " + std::to_string(V-1) + "]");
        }
        if (v < 0 || v >= V) {
            throw std::out_of_range("add_edge: вершина v=" + std::to_string(v) + " вне диапазона [0, " + std::to_string(V-1) + "]");
        }
        if (weight < 0) {
            std::cerr << "ВНИМАНИЕ: задан отрицательный вес (" << weight << "), для MST обычно положительные веса \n";
        }
        // граф неориентированный - ребро в обе стороны
        mx[u].push_back({v, weight});
        mx[v].push_back({u, weight});
        edges.push_back(Edge(u, v, weight));
    }

    void print() const {
        for (int i = 0; i < V; ++i) {
            std::cout << "вершина " << i << ": ";
            for (const auto& [v, w] : mx[i]) {
                std::cout << " ->" << v << "(" << w << ")  ";
            }
            std::cout << "\n";
        }

        std::cout << "\nсписок рёбер (" << edges.size() << "):\n";
        for (const Edge& e : edges) {
            e.print();
        }
    }

    std::tuple<bool, std::vector<Edge>> kruskal_mst() {
        // алгоритм Краскала работает от рёбер с минимальным весом.
        // нужно взять список рёбер в отсортированном (от меньшего к большему)
        // по весам порядке.
        // обрабатываем рёбра по очереди, начинаем добавлять рёбра
        // в минимальное остовое дерево при условии, что очередное ребро
        // не создает цикл.
        // чтобы понять, создаст ли ребро цикл - применяем алгоритм системы
        // непересекающихся множеств (Union-Find).
        //
        // у нас происходит сортировка рёбер и проход в цикле по всем рёбрам,
        // т.е. сложность получается где-то O(E log E). не знаю как корректнее
        // оценить сложность Union-Find, пишут, что он имеет "амортизированную"
        // сложность O(1)
        std::vector<Edge> sorted_edges = edges;
        std::sort(sorted_edges.begin(), sorted_edges.end());

        UnionFind uf(V);
        std::vector<Edge> mst;
        for (const auto& edge : sorted_edges) {
            // uf.print();
            if (!uf.connected(edge.beg, edge.end)) {
                uf.merge(edge.beg, edge.end);
                mst.push_back(edge);
            }
        }
        // uf.print();

        // проверяем, получилось ли связное дерево.
        // для этого проверяем основное свойство, что количество рёбер должно
        // быть на единицу меньше количества вершин
        return {(static_cast<int>(mst.size()) == (V-1)), mst};
    }

    std::tuple<bool, std::vector<Edge>> boruvka_mst() {
        // алгоритм Борувки работает от компонент связности.
        // немного похоже на алгоритм Краскала, он тоже использует
        // систему непересекающихся множеств (Union-Find). однако более тесно
        // связан с Union-Find. начинаем работать с состояния, когда у нас столько
        // компонент, сколько и вершин (каждая вершина сама себе компонент).
        // и работаем до тех пор, пока у нас не останется всего одна компонента,
        // или мы не сможем добавить ни одного ребра (если это несвязный граф).
        // затем для каждой вершины в компонентах формируем набор рёбер с самыми
        // маленькими весами (исключая дубли и убеждаясь что ребро не создает цикл).
        // а после проходимся по всем вершинам с учетом найденных рёбер и добавляем
        // в минимальное остовое дерево.
        //
        // сначала проходимся по всем рёбрам и формируем набор самых лёгких к другой
        // компоненте, а затем проходимся по всем вершинам и добавляем в MST,
        // т.е. сложность получается где-то O(E log V). сложность Union-Find, пишут,
        // что "амортизированная" O(1). но есть еще цикл, пока не закончились компоненты
        // и их я не уверен как учитывать в оценке
        UnionFind uf(V);
        std::vector<Edge> mst;
        while (uf.sets_count() > 1) {
            // для каждой вершины храним минимальное ребро к другой компоненте
            std::vector<Edge> cheapest(V, Edge());
            for (const Edge& e : edges) {
                int comp_u = uf.find(e.beg);
                int comp_v = uf.find(e.end);

                if (comp_u == comp_v) continue; // внутри компоненты рёбра не нужны, это цикл

                if (e.weight < cheapest[comp_u].weight) {
                    cheapest[comp_u] = e;
                }
                if (e.weight < cheapest[comp_v].weight) {
                    cheapest[comp_v] = e;
                }
            }

            // добавляем найденные рёбра в MST
            bool edges_added = false;
            for (int i = 0; i < V; i++) {
                const Edge& e = cheapest[i];
                if (!e.valid()) continue;

                // надо снова проверять, а не в одной ли компоненте вершины,
                // т.к. всё могло измениться после предыдущей итерации цикла
                // uf.print();
                if (!uf.connected(e.beg, e.end)) {
                    uf.merge(e.beg, e.end);
                    mst.push_back(e);
                    edges_added = true;
                }
            }
            // uf.print();

            // если не удалось добавить рёбра, значит граф не связный
            if (!edges_added) break;
        }

        // проверяем, получилось ли связное дерево.
        // для этого проверяем основное свойство, что количество рёбер должно
        // быть на единицу меньше количества вершин
        return {(static_cast<int>(mst.size()) == (V-1)), mst};
    }

    std::tuple<bool, std::vector<Edge>> prim_mst(int start_from = 0) {
        if (start_from < 0 || start_from >= V) throw std::out_of_range("стартовая вершина вне диапазона");

        std::vector<bool> in_mst(V, false);
        std::vector<int> cheapest(V, INT_MAX); // минимальный вес ребра к вершине
        std::vector<int> parent(V, -1);        // родитель (начало) для ребра в MST

        // алгоритм Прима работает от вершин. движется собирая соседние вершины с минимальным весом.
        // начинаем со стартовой вершины
        // далее проходимся по всем вершинам в графе и начинаем подбирать
        // соседние вершины с минимальным весом, которые еще не были обработаны
        cheapest[start_from] = 0;
        for (int i = 0; i < V; ++i) {
            int u = -1;
            int min = INT_MAX;
            for (int v = 0; v < V; ++v) {
                if (!in_mst[v] && cheapest[v] < min) {
                    min = cheapest[v];
                    u = v;
                }
            }
            if (u == -1) return {false, {}}; // граф несвязный! невозможно построить MST

            // нашли очередную вершину, подходящую с минимальным весом
            // далее проходимся по её соседям, которые ещё не были добавлены в MST
            // собираем веса этих рёбер, чтобы потом из них выбрать минимальное
            in_mst[u] = true;
            for (const auto& [v, weight] : mx[u]) {
                if (!in_mst[v] && weight < cheapest[v]) {
                    cheapest[v] = weight;
                    parent[v] = u;
                }
            }
        }

        std::vector<Edge> mst;
        for (int v = 0; v < V; v++) {
            if (parent[v] != -1) {
                mst.push_back(Edge(parent[v], v, cheapest[v]));
            }
        }
        return {true, mst};
    }
};
