#pragma once

#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>

struct Edge {
    int beg{-1};
    int end{-1};
    int weight{std::numeric_limits<int>::max()};

    Edge() = default;
    Edge(int u, int v, int w)
    :   beg(u),
        end(v),
        weight(w)
    {}

    void print() const {
        std::cout << " (" << beg << " - " << end << ", w=" << std::right << std::setw(3) << weight << ")\n";
    }

    bool valid() const {
        return (beg >= 0) && (end >= 0);
    }
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

struct Path {
    std::vector<int> vertices{};                 // последовательность вершин
    int weight{std::numeric_limits<int>::max()}; // общий вес пути

    Path() = default;

    void print() const {
        if (!valid()) {
            std::cout << "путь не существует";
            return;
        }
        std::cout << "вес=" << std::right << std::setw(4) << weight << ", путь: ";
        for (size_t i = 0; i < vertices.size(); ++i) {
            std::cout << vertices[i];
            if (i != (vertices.size() - 1)) std::cout << " -> ";
        }
    }

    bool valid() const {
        return (weight != std::numeric_limits<int>::max());
    }
};

struct FloydWarshallResult {
    std::vector<std::vector<int>> dist{}; // матрица расстояний от i до j вершины
    std::vector<std::vector<int>> next{}; // следующая вершина в пути (для удобства восстановления пути)
    bool has_negative_cycle{false};       // есть ли цикл отрицательного веса

    FloydWarshallResult() = delete;
    FloydWarshallResult(int V)
    :   dist(V, std::vector<int>(V, std::numeric_limits<int>::max())),
        next(V, std::vector<int>(V, -1)),
        has_negative_cycle(false)
    {}

    void print_matrix() const {
        std::cout << "матрица кратчайших расстояний:\n";
        size_t V = dist.size();
        std::cout << "     ";
        for (size_t i = 0; i < V; ++i) {
            std::cout << "  " << std::right << std::setw(2) << std::to_string(i);
        }
        std::cout << "\n";
        for (size_t i = 0; i < V; ++i) {
            std::cout << "  " << i << ": ";
            for (size_t j = 0; j < V; ++j) {
                if (dist[i][j] == std::numeric_limits<int>::max()) {
                    std::cout << "   *";
                } else {
                    std::cout << " " << std::right << std::setw(3) << std::to_string(dist[i][j]);
                }
            }
            std::cout << "\n";
        }
    }

    void print_paths() const {
        size_t V = dist.size();
        for (size_t i = 0; i < V; ++i) {
            std::cout << "кратчайшие пути от вершины " << i << ":\n";
            for (size_t j = 0; j < V; ++j) {
                if (i == j) continue;

                std::cout << "  до вершины " << j << ": ";
                get_path(i, j).print();
                std::cout << "\n";
            }
        }
    }

    Path get_path(int from, int to) const {
        Path result;
        if (dist[from][to] == std::numeric_limits<int>::max()) {
            return result;
        }

        result.weight = dist[from][to];
        if (from == to) {
            result.vertices.push_back(from);
            return result;
        }

        int curr = from;
        while (curr != to) {
            result.vertices.push_back(curr);
            curr = next[curr][to];
            if (curr == -1) {
                result.weight = std::numeric_limits<int>::max();
                result.vertices.clear();
                return result;
            }
        }
        result.vertices.push_back(to);

        return result;
    }
};

struct BellmanFordResult {
    int source{-1};
    std::vector<int> dist{};        // расстояния от источника до всех вершин
    std::vector<int> parent{};      // предки в дереве кратчайших путей
    bool has_negative_cycle{false}; // есть ли цикл отрицательного веса

    BellmanFordResult() = delete;
    BellmanFordResult(int V, int src)
    :   source(src),
        dist(V, std::numeric_limits<int>::max()),
        parent(V, -1),
        has_negative_cycle(false)
    {
        if (src >= 0) dist[src] = 0;
    }

    void print_matrix_row() const {
        std::cout << "матрица кратчайших расстояний:\n";
        size_t V = dist.size();
        std::cout << "     ";
        for (size_t i = 0; i < V; ++i) {
            std::cout << "  " << std::right << std::setw(2) << std::to_string(i);
        }
        std::cout << "\n";
        std::cout << "  " << source << ": ";
        for (size_t i = 0; i < V; ++i) {
            if (dist[i] == std::numeric_limits<int>::max()) {
                std::cout << "   *";
            } else {
                std::cout << " " << std::right << std::setw(3) << std::to_string(dist[i]);
            }
        }
        std::cout << "\n";
    }

    void print_paths() const {
        size_t V = dist.size();
        std::cout << "кратчайшие пути от вершины " << source << ":\n";
        for (size_t i = 0; i < V; ++i) {
            if (i == static_cast<size_t>(source)) continue;

            std::cout << "  до вершины " << i << ": ";
            get_path(i).print();
            std::cout << "\n";
        }
    }

    Path get_path(int target) const {
        Path result;
        if (dist[target] == std::numeric_limits<int>::max()) {
            return result;
        }

        result.weight = dist[target];
        std::vector<int> rev_path;
        int curr = target;
        while (curr != -1) {
            rev_path.push_back(curr);
            curr = parent[curr];
        }

        for (auto it = rev_path.rbegin(); it != rev_path.rend(); ++it) {
            result.vertices.push_back(*it);
        }
        return result;
    }
};

struct DeykstraResult {
    int source{-1};
    std::vector<int> dist{};   // расстояния от источника до всех вершин
    std::vector<int> parent{}; // предки в дереве кратчайших путей

    DeykstraResult() = delete;
    DeykstraResult(int V, int src)
    :   source(src),
        dist(V, std::numeric_limits<int>::max()),
        parent(V, -1)
    {
        if (src >= 0) dist[src] = 0;
    }


    void print_matrix_row() const {
        std::cout << "матрица кратчайших расстояний:\n";
        size_t V = dist.size();
        std::cout << "     ";
        for (size_t i = 0; i < V; ++i) {
            std::cout << "  " << std::right << std::setw(2) << std::to_string(i);
        }
        std::cout << "\n";
        std::cout << "  " << source << ": ";
        for (size_t i = 0; i < V; ++i) {
            if (dist[i] == std::numeric_limits<int>::max()) {
                std::cout << "   *";
            } else {
                std::cout << " " << std::right << std::setw(3) << std::to_string(dist[i]);
            }
        }
        std::cout << "\n";
    }

    void print_paths() const {
        size_t V = dist.size();
        std::cout << "кратчайшие пути от вершины " << source << ":\n";
        for (size_t i = 0; i < V; ++i) {
            if (i == static_cast<size_t>(source)) continue;

            std::cout << "  до вершины " << i << ": ";
            get_path(i).print();
            std::cout << "\n";
        }
    }

    Path get_path(int target) const {
        Path result;
        if (dist[target] == std::numeric_limits<int>::max()) {
            return result;
        }

        result.weight = dist[target];
        std::vector<int> rev_path;
        int curr = target;
        while (curr != -1) {
            rev_path.push_back(curr);
            curr = parent[curr];
        }

        for (auto it = rev_path.rbegin(); it != rev_path.rend(); ++it) {
            result.vertices.push_back(*it);
        }
        return result;
    }
};
