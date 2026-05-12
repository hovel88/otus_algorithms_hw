#pragma once

#include <algorithm>

#include "common.h"
#include "priority_queue.h"

class WeightedGraphSP {
private:
    std::vector<std::vector<std::pair<int, int>>> mx{};    // mx[u] = {v, weight}
    int                                           V{0};    // количество вершин
    std::vector<Edge>                             edges{}; // список всех рёбер (для удобства)

public:
    ~WeightedGraphSP() = default;
    WeightedGraphSP() = default;
    WeightedGraphSP(const WeightedGraphSP&) = default;
    WeightedGraphSP(WeightedGraphSP&&) = default;
    WeightedGraphSP& operator=(const WeightedGraphSP&) = default;
    WeightedGraphSP& operator=(WeightedGraphSP&&) = default;

    WeightedGraphSP(int vertices)
    : V(vertices)
    {
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
        // граф ориентированный, ребро только в указанную сторону
        mx[u].push_back({v, weight});
        edges.push_back(Edge(u, v, weight));
    }

    void print() const {
        for (int i = 0; i < V; ++i) {
            std::cout << "вершина " << i << ": ";
            for (const auto& [v, w] : mx[i]) {
                std::string s;
                s.append(" ->").append(std::to_string(v)).append("(").append(std::to_string(w)).append(")");
                std::cout << std::left << std::setw(9) << s;
            }
            if (mx[i].empty()) std::cout << "(нет исходящих)";
            std::cout << "\n";
        }

        std::cout << "\nсписок рёбер (" << edges.size() << "):\n";
        for (const Edge& e : edges) {
            e.print();
        }
    }

    int get_vertices_count() const { return V; }
    bool has_negative_weights() const {
        for (const Edge& e : edges) {
            if (e.weight < 0) return true;
        }
        return false;
    }

    FloydWarshallResult floyd_warshall_sp() {
        // алгоритм Флойда-Уоршалла формирует список кротчайших путей между
        // ВСЕМИ парами вершин.
        // его ограничение - не может работать, если есть циклы с отрицательной
        // весовой суммой. однако сами веса рёбер могут быть отрицательные.
        // алгоритм достаточно простой в реализации и понятный, однако его
        // алгоритмическая сложность O(V^3), что достаточно медленно.
        // соответственно применять его лучше на небольших графах.
        // для реализации потребуется матрица dist[V][V], в которой будем хранить
        // кротчайшие расстояния между вершинами u и v.
        // идея в том, что мы постепенно разрешаем использовать промежуточные (k)
        // вершины при переходе от одной (u) вершины к другой (v), и на каждом
        // шаге по матрице dist проверяем условие, а не получился ли путь
        // (u->k) + (k->v) легче чем (u->v).
        // таким образом не только находятся более короткие пути, но и "обходы",
        // если нет ребра между u и v

        // инициализируем матрицу расстояний: путь из вершины в себя же - обнуляем,
        // иначе - заполняем весами рёбер (если ребра нет, то у нас задано
        // максимальное значение для веса ребра, т.е. оно присутствует, но с большим весом)
        FloydWarshallResult result(V);
        for (int v = 0; v < V; ++v) {
            result.dist[v][v] = 0;
            result.next[v][v] = v;
            for (const auto& [u, weight] : mx[v]) {
                if (weight < result.dist[v][u]) {
                    result.dist[v][u] = weight;
                    result.next[v][u] = u;
                }
            }
        }

        for (int k = 0; k < V; ++k) {
            // разрешаем использовать вершину k
            // для поиска кротчайшего расстояния между вершинами u и v
            for (int v = 0; v < V; ++v) {
                if (k == v) continue;
                for (int u = 0; u < V; ++u) {
                    if (k == u) continue;

                    // если одно из рёбер отсутствует (бесконечный вес), то путь
                    // точно не получится кротчайшим
                    if (result.dist[v][k] == std::numeric_limits<int>::max()
                    ||  result.dist[k][u] == std::numeric_limits<int>::max()) continue;

                    int dist = result.dist[v][k] + result.dist[k][u];
                    if (dist < result.dist[v][u]) {
                        result.dist[v][u] = dist;
                        result.next[v][u] = result.next[v][k];
                    }

                }
            }
        }

        // проверяем на циклы с отрицательным весом
        for (int i = 0; i < V; ++i) {
            if (result.dist[i][i] < 0) {
                result.has_negative_cycle = true;
                break;
            }
        }
        return result;
    }

    BellmanFordResult bellman_ford_sp(int source) {
        if (source < 0 || source >= V) throw std::out_of_range("вершина-источник вне диапазона");

        // алгоритм Беллмана-Форда находит кратчайшие пути из конкретной вершины
        // до остальных. он может обнаружить отрицательные циклы в графе.
        // это "жадный" алгоритм, т.е. он работает аналогично поиску в ширину.
        // сложность алгоритма получается порядка O(V^2).
        // идея алгоритма проста: сначала инициализируем все дистанции до вершин максимальным
        // возможным значением. исходим из конкретной вершины (u) и проверяем связи (рёбра)
        // с каждой из соседних (v). алгоритм проходится V-1 раз.
        // сохраняем суммы минимальных дистанций (пути) до каждой вершины в том случае,
        // если сумма дистанций очередного пути оказалась меньше дистанции последнего
        // оптимального пути. а также запоминаем через какую вершину (u)
        // этот путь был найден, чтобы потом восстановить его в обратном порядке

        BellmanFordResult result(V, source);
        for (int k = 1; k <= (V - 1); ++k) {
            bool found = false; // флаг для оптимизации раннего выхода из цикла
            for (const Edge& e : edges) {
                // если путь до вершины существует
                // и можно улучшить путь до v, сделать меньше чем уже существующий
                if (result.dist[e.beg] == std::numeric_limits<int>::max()) continue;

                int dist = result.dist[e.beg] + e.weight;
                if (dist < result.dist[e.end]) {
                    result.dist[e.end]   = dist;
                    result.parent[e.end] = e.beg;
                    found = true;
                }
            }

            if (!found) {
                // не удалось для вершины на этой итерации подобрать полее подходящие пути
                break;
            }
        }

        // проверяем на циклы с отрицательным весом
        for (const Edge& e : edges) {
            if (result.dist[e.beg] != std::numeric_limits<int>::max() &&
                result.dist[e.beg] + e.weight < result.dist[e.end]) {
                result.has_negative_cycle = true;
                break;
            }
        }
        return result;
    }

    DeykstraResult deykstra_sp(int source) {
        if (source < 0 || source >= V) throw std::out_of_range("вершина-источник вне диапазона");

        DeykstraResult result(V, source);
        if (has_negative_weights()) return result;

        // алгоритм Дейкстры находит кратчайшие пути из конкретной вершины до всех
        // остальных вершин. алгоритм не может работать с отрицательными весами рёбер!
        // для реализации алгоритма используем приоритетную очередь, где наименьший
        // вес это наивысший приоритет. приоритетная очередь у нас основана
        // на бинарной куче (по умолчанию max-heap). структура данных бинарной кучи
        // по факту обеспечивает логарифмическую сложность, однако нам всё еще
        // приходится все рёбра, поэтому, по итогу сложность O(E log V).
        // в исходном состоянии расстояния от вершин пока не известны, поэтому
        // инициализируются максимальным значением.
        // из ещё не посещенных вершин выбирается та, что с наименьшим весом пути
        // (добавляем её в приоритетную очередь) и рассматриваем для нее соседние вершины.
        // а также запоминаем через какую вершину (u) этот путь был найден, чтобы потом
        // восстановить его в обратном порядке.
        // если все вершины посещены, то завершаем работу

        //                         weight, vertex
        auto cmp = [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return a.first > b.first;  // > для min-heap
        };
        PriorityQueue<std::pair<int, int>, decltype(cmp)> pq(cmp);

        pq.push({0, source});
        while (!pq.empty()) {
            auto [curr_dist, u] = pq.top();
            pq.pop();

            if (curr_dist != result.dist[u]) continue;
            for (const auto& [v, weight] : mx[u]) {
                int dist = curr_dist + weight;
                if (dist < result.dist[v]) {
                    result.dist[v]   = dist;
                    result.parent[v] = u;
                    pq.push({dist, v});
                }
            }
        }
        return result;
    }
};
