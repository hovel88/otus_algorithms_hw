#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>

struct GraphSortLevels {
    std::vector<std::vector<int>> levels{};     // результат: уровни вершин

    void print() const {
        std::cout << "печать массива уровней с номерами вершин, принадлежащих этому уровню:\n";
        if (levels.empty()) {
            std::cout << "!!!граф пуст или не содержит вершин с нулевой полустепенью захода\n";
            return;
        }

        std::cout << "всего уровней: " << levels.size() << "\n";
        for (size_t i = 0; i < levels.size(); ++i) {
            std::cout << "level[" << i << "]: ";
            for (int v : levels[i]) {
                std::cout << v << " ";
            }
            std::cout << "\n";
        }
    }
};

class Graph
{
private:
    // --------------------------------------------------------
    // 5. Векторы смежности.
    //    матрица (двумерный массив) M[V][Smax], в которой:
    //    строки  - это вершины графа (их количество V),
    //    столбцы - список номеров соседних вершин, с которыми есть соединение рёбрами.
    //    количество столбцов равно величине максимальной степени вершин графа (Smax).
    //    пустые ячейки матрицы заполняются невалидными значениями (например, -1)
    //    для формирования прямоугольной матрицы
    // --------------------------------------------------------
    std::vector<std::vector<int>> mx{};
    static constexpr int INVALID = -1;

public:
    ~Graph() = default;
    Graph() = default;
    Graph(const Graph&) = default;
    Graph(Graph&&) = default;
    Graph& operator=(const Graph&) = default;
    Graph& operator=(Graph&&) = default;

    // принимаем граф для удобства в виде ступенчатых массивов смежности
    // и решейпим в формат векторов смежности
    explicit Graph(const std::vector<std::vector<int>>& edges)
    :   mx(edges)
    {
        reshape(max_degree(edges));
    }

    void print() const {
        std::cout << "исходный граф в виде вектора смежности:\n";
        for (size_t i = 0; i < mx.size(); i++) {
            std::cout << "вершина " << i << ": ";
            for (const auto& neighbour : mx[i]) {
                if (neighbour < 0) {
                    std::cout << "   *  ";
                } else {
                    std::cout << " ->" << std::to_string(neighbour) << "  ";
                }
            }
            std::cout << "\n";
        }
    }

    std::tuple<bool, GraphSortLevels> demucron_sort() {
        // ШАГ 1: создаем массив полустепеней захода для каждой вершины
        std::vector<int> in_degree(mx.size());
        for (const auto& from_u : mx) {
            for (int to_v : from_u) {
                if (to_v == INVALID) continue;
                in_degree[to_v]++; // дуга "u -> v", увеличиваем счётчик вхождения у "v"
            }
        }

        // ШАГ 2: создаем очередь для вершин текущего уровня.
        // и наполняем вершинами с нулевой полустепенью захода
        std::queue<int> q;
        for (size_t i = 0; i < mx.size(); ++i) {
            if (in_degree[i] == 0) q.push(i);
        }

        // ШАГ 3: пока очередь не пуста, обрабатываем вершины и вычитаем
        // из массива полустепеней захода "вклад" каждой вершины с нулевой полустепенью.
        // проходимся по уровням (послойно), сохраняя статистику обработки каждого уровня.
        // если нет вершин с нулевой полустепенью захода, то топологическая сортировка
        // графа невозможна
        GraphSortLevels gsl;
        while (!q.empty()) {
            std::vector<int> current_level;
            std::queue<int> next_q;

            // обрабатываем все вершины текущего уровня
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                current_level.push_back(u);

                // "удаляем" вершину: уменьшаем вклад этой вершины в полустепень захода инцидентных вершин.
                // и сразу наполняем очередь вершин для обработки новой вершиной, если у нее нулевая полустепень
                for (int v : mx[u]) {
                    if (v == INVALID) continue;
                    in_degree[v]--;
                    if (in_degree[v] == 0) {
                        next_q.push(v);
                    }
                }
            }

            q = next_q;
            // сохраняем текущий уровень обработки в результат
            if (!current_level.empty()) gsl.levels.push_back(current_level);
        }

        // проверяем, все ли вершины обработаны
        size_t processed_count = 0;
        for (const auto& level : gsl.levels) {
            processed_count += level.size();
        }
        return {(processed_count == mx.size()), gsl}; // true - успех, false - есть цикл
    }

private:
    static size_t max_degree(const std::vector<std::vector<int>>& edges) {
        size_t max{0};
        for (const auto& e : edges) {
            if (max < e.size()) max = e.size();
        }
        return max;
    }

    void reshape(size_t max_degree) {
        for (auto& one : mx) {
            while (one.size() < max_degree) one.push_back(INVALID);
        }
    }
};
