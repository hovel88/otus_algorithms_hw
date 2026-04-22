#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <forward_list>
#include <set>
#include <algorithm>

/*
 * ДВУДОЛЬНЫЙ ГРАФ A(3,4) с 5 рёбрами
 * Вершины: левая доля  A = {0,1,2}
 *          правая доля B = {3,4,5,6}
 * Рёбра: (0-3), (1-3), (1-4), (1-5), (2-6)
 *
 *  (0) --- (3)
 *        /    
 *       /     
 *  (1) --- (4)
 *       \     
 *        \    
 *          (5)
 *  (2) --- (6)
 */

const size_t V = 7; // всего 7 вершин
const size_t E = 5; // всего 5 рёбер

size_t counter = 0;
struct Edge {
    const size_t id{0};
    int u, v;
    Edge(int u_, int v_) : id(counter++), u(u_), v(v_) {}
};
using EdgeCollection = std::vector<Edge>;

static const EdgeCollection edges = {
    {0,3}, {1,3}, {1,4}, {1,5}, {2,6}
};

static size_t max_degree()
{
    std::vector<int> deg(V, 0);
    for (const auto& e : edges) {
        deg[e.u]++;
        deg[e.v]++;
    }
    auto it = std::max_element(deg.begin(), deg.end());
    if (it == deg.end()) return 0;
    return *it;
}

static void print_degrees()
{
    std::vector<int> deg(V, 0);
    for (const auto& e : edges) {
        deg[e.u]++;
        deg[e.v]++;
    }
    for (size_t i = 0; i < V; i++) {
        std::cout << "deg(" << i << ") = " << deg[i] << (deg[i] == 0 ? " (изолированная)" : "") << "\n";
    }
}


// --------------------------------------------------------
// 1. Множества.
//    список (массив) вершин, и список рёбер
// --------------------------------------------------------

class GLists
{
private:
    std::set<int>                 vertices{};
    std::set<std::pair<int, int>> edges{};

    GLists() = default;

public:
    static GLists build(const EdgeCollection& collection) {
        GLists graph;
        for (const auto& one : collection) {
            graph.vertices.emplace(one.u);
            graph.vertices.emplace(one.v);
            graph.edges.emplace(one.u, one.v);
        }
        return graph;
    }

    void print() {
        std::cout << "  Вершины: { ";
        for (int v : vertices) std::cout << v << " ";
        std::cout << "}\n";
        std::cout << "  Рёбра: { ";
        for (const auto& e : edges) std::cout << "(" << e.first << "-" << e.second << ") ";
        std::cout << "}\n";
    }
};

// --------------------------------------------------------
// 2. Матрица смежности (V x V)
//    двумерный массив, строки и столбцы - вершины,
//    в ячейке матрицы 1 означает наличие ребра
// --------------------------------------------------------

class GAdjMarix
{
private:
    std::vector<std::vector<bool>> mx{};
    size_t                         sz{0};

    GAdjMarix() = default;

public:
    static GAdjMarix build(const EdgeCollection& collection, size_t v_count) {
        GAdjMarix graph;
        graph.sz = v_count;
        graph.mx = {v_count, std::vector<bool>(v_count, false)};
        for (const auto& one : collection) {
            if (one.u < static_cast<int>(v_count)
            &&  one.v < static_cast<int>(v_count)) {
                graph.mx[one.u][one.v] = true;
                graph.mx[one.v][one.u] = true; // неориентированный граф
            }
        }
        return graph;
    }

    void print() {
        std::cout << "     ";   for (size_t i = 0; i < sz; i++) std::cout << "V_" << i << " ";
        std::cout << "\n     "; for (size_t i = 0; i < sz; i++) std::cout << "--- ";
        std::cout << "\n";
        for (size_t i = 0; i < sz; i++) {
            std::cout << "V_" << i << ": ";
            for (size_t j = 0; j < sz; j++) std::cout << " " << mx[i][j] << "  ";
            std::cout << "\n";
        }
    }
};

// --------------------------------------------------------
// 3. Матрица инцидентности (V x E)
//    двумерный массив, строки - вершины, столбцы - рёбра,
//    в ячейке матрицы 1 означает что вершина инцидентна ребру
// --------------------------------------------------------

class GIncMarix
{
private:
    std::vector<std::vector<int>> mx{};
    size_t                        vertices_sz{0};
    size_t                        edges_sz{0};

    GIncMarix() = default;

public:
    static GIncMarix build(const EdgeCollection& collection, size_t v_count, size_t e_count) {
        GIncMarix graph;
        graph.vertices_sz = v_count;
        graph.edges_sz    = e_count;
        graph.mx = {v_count, std::vector<int>(e_count, 0)};
        for (size_t e_idx = 0; e_idx < e_count; e_idx++) {
            auto u = collection[e_idx].u;
            auto v = collection[e_idx].v;
            graph.mx[u][e_idx]++;
            graph.mx[v][e_idx]++;
        }
        return graph;
    }

    void print() {
        std::cout << "     ";   for (size_t i = 0; i < edges_sz; i++) std::cout << "E_" << i << " ";
        std::cout << "\n     "; for (size_t i = 0; i < edges_sz; i++) std::cout << "--- ";
        std::cout << "\n";
        for (size_t i = 0; i < vertices_sz; i++) {
            std::cout << "V_" << i << ": ";
            for (size_t j = 0; j < edges_sz; j++) std::cout << " " << mx[i][j] << "  ";
            std::cout << "\n";
        }
    }
};

// --------------------------------------------------------
// 4. Перечень рёбер
//    протсой список пар вершин, где первая вершина - это начало
//    ребра, а вторая вершина - конец ребра. в неоринетированном
//    графе нет направления, поэтому ребра дублируют (искусственно
//    создавая направления), чтобы не просматривать алгоритмом
//    и первую и вторую вершины
// --------------------------------------------------------

class GEdgeList
{
private:
    std::vector<std::pair<int, int>> edges{};

    GEdgeList() = default;

public:
    static GEdgeList build(const EdgeCollection& collection) {
        GEdgeList graph;
        for (const auto& one : collection) {
            graph.edges.emplace_back(one.u, one.v);
            graph.edges.emplace_back(one.v, one.u);
        }
        return graph;
    }

    void print() {
        std::cout << "{ ";
        for (const auto& e : edges) std::cout << "(" << e.first << "-" << e.second << ") ";
        std::cout << "}\n";
    }
};

// --------------------------------------------------------
// 5. Векторы смежности.
//    матрица (двумерный массив), в которой строки - это вершины графа,
//    а столбцы - номера (список) соседних вершин, с которыми есть
//    соединение рёбрами.
//    количество столбцов равно величине максимальной степени
//    вершин графа. пусные ячейки матрицы заполняются невалидными
//    значениями (например, -1) для формирования прямоугольной матрицы
// --------------------------------------------------------

class GNeighboursVector
{
private:
    std::vector<std::vector<int>> mx{};

    GNeighboursVector() = default;

public:
    static GNeighboursVector build(const EdgeCollection& collection, size_t v_count) {
        GNeighboursVector graph;
        graph.mx.resize(v_count);
        for (const auto& one : collection) {
            graph.mx[one.u].push_back(one.v);
            graph.mx[one.v].push_back(one.u);
        }
        auto max_deg = max_degree();
        for (auto& one : graph.mx) {
            while (one.size() < max_deg) one.push_back(-1);
        }
        return graph;
    }

    void print() {
        for (size_t i = 0; i < mx.size(); i++) {
            std::cout << "V_" << i << ": ";
            for (const auto& neighbour : mx[i]) std::cout << " " << ( neighbour < 0 ? "*" : std::to_string(neighbour) ) << "  ";
            std::cout << "\n";
        }
    }
};

// --------------------------------------------------------
// 6. Массивы смежности.
//    тоже самое что и вектор смежности, только это не прямоугольная
//    матрица, а ступенчатый массив. в котором строки - это вершины
//    графа, а количество столбцов в каждой строке разное.
//    позволяет не выделять лишнюю память
// --------------------------------------------------------

class GNeighboursArray
{
private:
    std::vector<std::vector<int>> mx{};

    GNeighboursArray() = default;

public:
    static GNeighboursArray build(const EdgeCollection& collection, size_t v_count) {
        GNeighboursArray graph;
        graph.mx.resize(v_count);
        for (const auto& one : collection) {
            graph.mx[one.u].push_back(one.v);
            graph.mx[one.v].push_back(one.u);
        }
        return graph;
    }

    void print() {
        for (size_t i = 0; i < mx.size(); i++) {
            std::cout << "V_" << i << ": ";
            for (const auto& neighbour : mx[i]) std::cout << " " << neighbour << "  ";
            std::cout << "\n";
        }
    }
};


// --------------------------------------------------------
// 7. Список смежности.
//    тоже самое что и вектор смежности, только это не прямоугольная
//    матрица, а массив из списков. в котором строки - это вершины
//    графа, а каждая строка это список номеров вершин, с которыми
//    связана текущая вершина рёбрами. это не прямоугольная матрица
//    как в случае вектора смежности. и это не ступенчатый массив
//    как в случае с массивом смежности. тут используется связный список,
//    что может быть полезно в случае, когда вершины добавляются, чтобы
//    не перевыделять новые массивы
// --------------------------------------------------------

class GNeighboursList
{
private:
    std::vector<std::forward_list<int>> mx{};

    GNeighboursList() = default;

public:
    static GNeighboursList build(const EdgeCollection& collection, size_t v_count) {
        GNeighboursList graph;
        graph.mx.resize(v_count);
        for (const auto& one : collection) {
            graph.mx[one.u].emplace_front(one.v);
            graph.mx[one.v].emplace_front(one.u);
        }
        return graph;
    }

    void print() {
        for (size_t i = 0; i < mx.size(); i++) {
            std::cout << "V_" << i << ": ";
            for (const auto& neighbour : mx[i]) std::cout << " " << neighbour << "  ";
            std::cout << "\n";
        }
    }
};

// --------------------------------------------------------
// 8. Структура с оглавлением.
//    в одном линейном массиве хранятся подряд одно за одним все массивы смежности,
//    в одну строку. сначала все соседи вершины 0, затем все соседи вершины 1 и т.д.
//    в отдельном линейном массиве создается оглавление с указателями (индексами)
//    на начало списка для каждой вершины
// --------------------------------------------------------

class GIndexedEdgeList
{
private:
    std::vector<int> edges{};   // все рёбра подряд: соседи 0, соседи 1, ...
    std::vector<int> index{};   // индекс начала списка для каждой вершины

    GIndexedEdgeList() = default;

public:
    static GIndexedEdgeList build(const EdgeCollection& collection, size_t v_count) {
        std::vector<std::vector<int>> mx{};
        mx.resize(v_count);
        for (const auto& one : collection) {
            mx[one.u].emplace_back(one.v);
            mx[one.v].emplace_back(one.u);
        }
        GIndexedEdgeList graph;
        graph.index.resize(v_count + 1); // +1, чтобы хранить конец последнего списка
        graph.index[0] = 0;
        for (size_t i = 0; i < v_count; ++i) {
            graph.edges.insert(graph.edges.end(), mx[i].begin(), mx[i].end());
            graph.index[i + 1] = graph.edges.size(); // конец списка соседей вершины i
        }
        return graph;
    }

    void print() {
        std::cout << "Соседи каждой вершины:\n";
        if (!index.empty()) {
            for (size_t v = 0; v < (index.size() - 1); ++v) {
                std::cout << "V_" << v << ": ";
                auto neighbors = std::vector<int>(edges.begin() + index[v],
                                                edges.begin() + index[v + 1]);
                for (int n : neighbors) std::cout << " " << n << "  ";
                std::cout << "\n";
            }
        }
        std::cout << "Массив index: ";
        for (int idx : index) std::cout << idx << " ";
        std::cout << "\nМассив edges: ";
        for (int e : edges) std::cout << e << " ";
        std::cout << "\n";
    }
};

// --------------------------------------------------------
// 9. Список вершин и список рёбер.
//    имеется список всех вершин графа, элементами этого спсика
//    являются другие списки с инцедентными вершинами для этой вершины
// --------------------------------------------------------

class GVertexEdgeList
{
private:
    struct VItem {
        size_t v_idx{0};
        std::list<int> neighbours{};

        VItem(size_t id) : v_idx(id) {}
    };
    std::list<VItem> lists{};

    GVertexEdgeList() = default;

public:
    static GVertexEdgeList build(const EdgeCollection& collection, size_t v_count) {
        std::vector<std::vector<int>> mx{};
        mx.resize(v_count);
        for (const auto& one : collection) {
            mx[one.u].emplace_back(one.v);
            mx[one.v].emplace_back(one.u);
        }
        GVertexEdgeList graph;
        for (size_t i = 0; i < v_count; ++i) {
            graph.lists.emplace_back(i);
            for (const auto& one : mx[i]) {
                graph.lists.back().neighbours.push_back(one);
            }
        }
        return graph;
    }

    void print() {
        for (const auto& one : lists) {
            std::cout << "V_" << one.v_idx << " -> { ";
            for (const auto& n : one.neighbours) {
                std::cout << "(" << n << ") ";
            }
            std::cout << "}\n";
        }
    }
};



std::string export_to_dot()
{
    // генерация DOT для визуализации Graphviz
    std::string dot = "graph G {\n";
    dot += "  // Неориентированный двудольный граф A(3,4) с 5 рёбрами\n";
    dot += "  rankdir=LR;\n";
    dot += "  node [shape=circle, style=filled];\n\n";

    // Левая доля (синие)
    dot += "  // Левая доля A = {0,1,2}\n";
    for (size_t i = 0; i < 3; i++) {
        dot += "  " + std::to_string(i) + " [fillcolor=lightblue, label=\"" + std::to_string(i) + "\"];\n";
    }

    // Правая доля (розовые)
    dot += "\n  // Правая доля B = {3,4,5,6}\n";
    for (size_t i = 3; i < V; i++) {
        dot += "  " + std::to_string(i) + " [fillcolor=pink, label=\"" + std::to_string(i) + "\"];\n";
    }

    // Кластеры для группировки
    dot += "\n  subgraph cluster_left {\n";
    dot += "    label = \"Доля A (3 вершины)\";\n";
    dot += "    color = blue;\n";
    dot += "    style = dashed;\n";
    for (size_t i = 0; i < 3; i++) dot += "    " + std::to_string(i) + ";\n";
    dot += "  }\n\n";

    dot += "  subgraph cluster_right {\n";
    dot += "    label = \"Доля B (4 вершины)\";\n";
    dot += "    color = red;\n";
    dot += "    style = dashed;\n";
    for (size_t i = 3; i < V; i++) dot += "    " + std::to_string(i) + ";\n";
    dot += "  }\n\n";

    // Рёбра
    dot += "  // Рёбра (только между долями)\n";
    for (size_t i = 0; i < edges.size(); i++) {
        dot += "  " + std::to_string(edges[i].u) + " -- " + std::to_string(edges[i].v) + " [label=\"e" + std::to_string(i) + "\"];\n";
    }

    dot += "}\n";
    return dot;
}



int main(int argc, char** argv)
{
    std::cout << "Программа представления структур хранения графов\n\n";
    std::cout << "  (0) --- (3)\n";
    std::cout << "        /    \n";
    std::cout << "       /     \n";
    std::cout << "  (1) --- (4)\n";
    std::cout << "       \\     \n";
    std::cout << "        \\    \n";
    std::cout << "          (5)\n";
    std::cout << "  (2) --- (6)\n\n";

    std::cout << "\nСтепени вершин:\n";
    print_degrees();
    std::cout << "\nМаксимальная степень вершин графа: " << max_degree() << std::endl;

    std::cout << "\n1. Перечисление множеств:\n";
    GLists::build(edges).print();
    std::cout << "\n2. Матрица смежности (V x V):\n";
    GAdjMarix::build(edges, V).print();
    std::cout << "\n3. Матрица инцидентности (V x E):\n";
    GIncMarix::build(edges, V, E).print();
    std::cout << "\n4. Перечень рёбер:\n";
    GEdgeList::build(edges).print();
    std::cout << "\n5. Векторы смежности:\n";
    GNeighboursVector::build(edges, V).print();
    std::cout << "\n6. Массивы смежности:\n";
    GNeighboursArray::build(edges, V).print();
    std::cout << "\n7. Списки смежности:\n";
    GNeighboursList::build(edges, V).print();
    std::cout << "\n8. Структура с оглавлением:\n";
    GIndexedEdgeList::build(edges, V).print();
    std::cout << "\n9. Список вершин со списком рёбер:\n";
    GVertexEdgeList::build(edges, V).print();

    std::string dot_content = export_to_dot();
    // std::cout << "\n-----BEGIN DOT-format-----\n";
    // std::cout << dot_content;
    // std::cout << "\n-----END DOT-format-----\n";
    std::ofstream dot_file("graph_undirected.dot");
    if (dot_file.is_open()) {
        dot_file << dot_content;
        dot_file.close();
        std::cout << "\nФайл 'graph_undirected.dot' создан!\n";
        std::cout << "Для генерации PNG выполнить: dot -Tpng graph_undirected.dot -o graph.png\n";
    }

    return 0;
}
