#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <tuple>
#include <algorithm>

#include "queue.h"
#include "stack.h"

struct DemucronSortLevels {
    std::vector<std::vector<int>> levels{};
    std::vector<int>              order{};

    void print_order() const {
        std::cout << "топологически упорядоченный граф: ";
        for (int v : order) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }
    void print_debug() const {
        if (levels.empty()) {
            std::cout << "!!!граф пуст или имеет циклы\n";
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

struct TarjanSortLevels {
    std::vector<std::vector<int>> levels{};
    std::vector<int>              order{};

    void print_order() const {
        std::cout << "топологически упорядоченный граф: ";
        for (int v : order) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }
    void print_debug() const {
        if (levels.empty()) {
            std::cout << "!!!граф пуст или имеет циклы\n";
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

struct KosarajuSCC {
    std::vector<std::vector<int>>    components{};   // список компонент сильной связности (SCC)
    std::vector<int>                 component_id{}; // для каждой вершины - idx компоненты
    std::vector<std::pair<int, int>> edges{};        // рёбра между разными SCC

    void print() const {
        std::cout << "компоненты сильной связности в графе:\n";
        if (components.empty()) {
            std::cout << "  (нет компонент)\n";
        } else {
            std::cout << "всего компонент: " << components.size() << "\n";
            for (size_t i = 0; i < components.size(); ++i) {
                std::cout << "SCC" << i << ": { ";
                for (int v : components[i]) {
                    std::cout << v << " ";
                }
                std::cout << "}\n";
            }
        }

        std::cout << "мосты (рёбра между компонентами сильной связности) в графе:\n";
        if (edges.empty()) {
            std::cout << "  (нет мостов)\n";
        } else {
            for (const auto& [u, v] : edges) {
                int comp_u = component_id[u];
                int comp_v = component_id[v];
                if (comp_u != comp_v) {
                    std::cout << "(SCC" << comp_u << ") " << u << " -> " << v << " (SCC" << comp_v << ")\n";
                }
            }
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

    static constexpr int INVALID = -1;   // нет связи с другими вершинами

    static constexpr int STATE_NONE = 0; // вершина ещё не посещена
    static constexpr int STATE_SEEN = 1; // вершина в обработке
    static constexpr int STATE_DONE = 2; // вершина обработана

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

    std::tuple<bool, DemucronSortLevels> demucron_sort() {
        // создаем массив полустепеней захода для каждой вершины
        std::vector<int> in_degree(mx.size());
        for (const auto& from_u : mx) {
            for (int to_v : from_u) {
                if (to_v == INVALID) continue;
                in_degree[to_v]++; // дуга "u -> v", увеличиваем счётчик вхождения у "v"
            }
        }

        // создаем очередь для вершин текущего уровня.
        // и наполняем вершинами с нулевой полустепенью захода
        Queue<int> queue;
        for (size_t i = 0; i < mx.size(); ++i) {
            if (in_degree[i] == 0) queue.push(i);
        }

        // пока очередь не пуста, обрабатываем вершины и вычитаем
        // из массива полустепеней захода "вклад" каждой вершины с нулевой полустепенью.
        // проходимся по уровням (послойно), сохраняя статистику обработки каждого уровня.
        // если нет вершин с нулевой полустепенью захода, то топологическая сортировка
        // графа невозможна
        DemucronSortLevels result;
        while (!queue.empty()) {
            std::vector<int> current_level;
            Queue<int> next_q;

            // обрабатываем все вершины текущего уровня
            while (!queue.empty()) {
                int u = queue.front();
                queue.pop();
                current_level.push_back(u);

                // "удаляем" вершину: уменьшаем вклад этой вершины в полустепень
                // захода инцидентных вершин. и сразу наполняем очередь вершин
                // для обработки новой вершиной, если у нее нулевая полустепень
                for (int v : mx[u]) {
                    if (v == INVALID) continue;
                    in_degree[v]--;
                    if (in_degree[v] == 0) {
                        next_q.push(v);
                    }
                }
            }

            queue = next_q;
            // сохраняем результаты обработки текущего уровня обработки графа
            if (!current_level.empty()) result.levels.push_back(current_level);
        }

        // проверяем, все ли вершины обработаны
        size_t processed_count = 0;
        for (const auto& level : result.levels) {
            processed_count += level.size();
        }

        bool has_cycle = (processed_count != mx.size());
        if (!has_cycle) {
            for (const auto& level : result.levels) {
                for (int v : level) result.order.push_back(v);
            }
        }
        return {!has_cycle, result}; // true - успех, false - есть цикл
    }

    std::tuple<bool, TarjanSortLevels> tarjan_sort() {
        std::vector<int> state(mx.size(), STATE_NONE);

        // для каждой вершины осуществляем поиск в глубину.
        // доходим до вершины у которой нет дуг к другим вершинам (нулевая
        // полустепень исхода), либо все соседние вершины уже обработаны.
        // собираем эти вершины в стек, а затем извлекаем их в обратном порядке
        bool has_cycle = false;
        Stack<int> stack;
        for (size_t i = 0; i < mx.size(); ++i) {
            if (state[i] != STATE_NONE) continue;
            if (tarjan_dfs(i, state, stack)) {
                has_cycle = true;
                break;
            }
        }

        TarjanSortLevels result;
        if (!has_cycle) {
            while (!stack.empty()) {
                result.order.push_back(stack.top());
                result.levels.push_back({stack.top()});
                stack.pop();
            }
        }
        return {!has_cycle, result}; // true - успех, false - есть цикл
    }

    KosarajuSCC kosaraju_scc() {
        // первый проход в глубину для определения порядка вершин
        std::vector<bool> visited(mx.size(), false);
        Stack<int> order_stack;
        for (size_t i = 0; i < mx.size(); ++i) {
            if (!visited[i]) {
                kosaraju_dfs1(i, visited, order_stack);
            }
        }
        std::fill(visited.begin(), visited.end(), false);

        // строим транспонированный граф (обращаем направление всех дуг)
        std::vector<std::vector<int>> transposed(mx.size());
        for (size_t u = 0; u < mx.size(); ++u) {
            for (int v : mx[u]) {
                if (v == INVALID) continue;
                transposed[v].push_back(u);
            }
        }

        // второй проход в глубину уже на транспонированном графе
        // для выделения компонентов сильной связности
        KosarajuSCC result;
        result.component_id.resize(mx.size());
        while (!order_stack.empty()) {
            int v = order_stack.top();
            order_stack.pop();

            if (!visited[v]) {
                std::vector<int> component;
                kosaraju_dfs2(v, transposed, visited, component);

                // присваиваем номер компоненты каждой вершине в этом компоненте.
                // нам это нужно будет потом для вывода мостов между компонентами
                // сильной связности
                int comp_idx = result.components.size();
                for (int u : component) {
                    result.component_id[u] = comp_idx;
                }

                // сохраняем очередную компоненту сильной связности
                result.components.push_back(component);
            }
        }

        for (size_t u = 0; u < mx.size(); ++u) {
            for (int v : mx[u]) {
                if (v == INVALID) continue;
                if (result.component_id[u] != result.component_id[v]) {
                    result.edges.push_back({u, v});
                }
            }
        }
        return result;
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

    bool tarjan_dfs(int from, std::vector<int>& state, Stack<int>& stack) {
        // идём поиском в глубину.
        // если обнаружена вершина из которой больше нет путей
        // то добавляем ее в стек.
        // текущую вершину помечаем как "в обработке", чтобы потом
        // в рекурсии понять, что мы уже обработали

        state[from] = STATE_SEEN;
        for (int to : mx[from]) {
            if (to == INVALID) continue;

            if (state[to] == STATE_SEEN) return true; // цикл!
            if (state[to] == STATE_NONE) {
                if (tarjan_dfs(to, state, stack)) {
                    return true;
                }
            }
        }
        state[from] = STATE_DONE;

        stack.push(from);
        return false;
    }

    void kosaraju_dfs1(int from, std::vector<bool>& visited, Stack<int>& stack) {
        // выполняется поиск в глубину для первого прохода.
        // заполнение стека с порядком вершин

        visited[from] = true;
        for (int to : mx[from]) {
            if (to == INVALID) continue;
            if (!visited[to]) {
                kosaraju_dfs1(to, visited, stack);
            }
        }
        stack.push(from);
    }

    void kosaraju_dfs2(int from, const std::vector<std::vector<int>>& transposed,
                       std::vector<bool>& visited,
                       std::vector<int>& component) {
        // выполняется поиск в глубину для второго прохода.
        // на транспонированном графе группируем вершины в один компонент
        // сильной связности. начинаем идти от каждой вершины в стеке
        // и обходим соседей пока не останутся посещенные вершины или вершины
        // не закончатся. соответственно в одну компоненту будут добавлены все
        // вершины, которые ограничены этим компонентом.

        visited[from] = true;
        component.push_back(from);

        for (int to : transposed[from]) {
            if (to == INVALID) continue;
            if (!visited[to]) {
                kosaraju_dfs2(to, transposed, visited, component);
            }
        }
    }
};
