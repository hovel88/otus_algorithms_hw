#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <chrono>
#include <random>
#include <algorithm>

#include "BST.h"
#include "AVL_V1.h"
#include "AVL_V2.h"
#include "Treap.h"
#include "Splay.h"
#include "RandBST.h"

struct TreeStats {
    std::string              desc{};            // описание
    std::string              name{};            // название алгоритма
    std::chrono::nanoseconds time_insert{};     // длительность выполнения вставки
    std::chrono::nanoseconds time_search{};     // длительность выполнения поиска
    std::chrono::nanoseconds time_remove{};     // длительность выполнения удаления
    int                      size{0};           // количество элементов в дереве
    int                      height{0};         // высота дерева
    std::string              checks{};          // дебажный вывод по проверкам свойств дерева
};

std::vector<int> generate_random_numbers(size_t n)
{
    std::vector<int> numbers(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, n * 10); // Диапазон чисел

    for (size_t i = 0; i < n; i++) {
        numbers[i] = dis(gen);
    }
    return numbers;
}

class PerformanceTester
{
public:
    PerformanceTester(size_t size)
    :   N(size) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, N * 10);

        random_data.resize(N);
        for (size_t i = 0; i < N; i++) {
            random_data[i] = dis(gen);
        }

        sorted_data = random_data;
        std::sort(sorted_data.begin(), sorted_data.end());

        size_t search_delete_count = N / 4;
        search_delete_elements.resize(search_delete_count);
        for (size_t i = 0; i < search_delete_count; i++) {
            search_delete_elements[i] = dis(gen);
        }
    }

    void run() {
        std::cout << "\n--- Тестирование с количеством элементов: " << N << " ---\n";

        std::vector<TreeStats> stats;
        stats.push_back(test_tree<BST>("random data",     random_data, search_delete_elements, search_delete_elements));
        stats.push_back(test_tree<AVL_V1>("random data",  random_data, search_delete_elements, search_delete_elements));
        stats.push_back(test_tree<AVL_V2>("random data",  random_data, search_delete_elements, search_delete_elements));
        stats.push_back(test_tree<Treap>("random data",   random_data, search_delete_elements, search_delete_elements));
        stats.push_back(test_tree<RandBST>("random data", random_data, search_delete_elements, search_delete_elements));
        stats.push_back(test_tree<Splay>("random data",   random_data, search_delete_elements, search_delete_elements));
        stats.push_back(test_tree<BST>("sorted data",     sorted_data, search_delete_elements, search_delete_elements));
        stats.push_back(test_tree<AVL_V1>("sorted data",  sorted_data, search_delete_elements, search_delete_elements));
        stats.push_back(test_tree<AVL_V2>("sorted data",  sorted_data, search_delete_elements, search_delete_elements));
        stats.push_back(test_tree<Treap>("sorted data",   sorted_data, search_delete_elements, search_delete_elements));
        stats.push_back(test_tree<RandBST>("sorted data", sorted_data, search_delete_elements, search_delete_elements));
        stats.push_back(test_tree<Splay>("sorted data",   sorted_data, search_delete_elements, search_delete_elements));
        print_stats(stats);
    }

private:
    const size_t N{0};
    std::vector<int> random_data;
    std::vector<int> sorted_data;
    std::vector<int> search_delete_elements;

    template<typename Tree>
    TreeStats test_tree(const std::string& desc,
                        const std::vector<int>& insert_data,
                        const std::vector<int>& search_data,
                        const std::vector<int>& remove_data) {
        Tree tree;
        TreeStats result;

        // вставка
        auto start = std::chrono::high_resolution_clock::now();
        for (int x : insert_data) {
            tree.insert(x);
        }
        auto stop = std::chrono::high_resolution_clock::now();
        result.time_insert = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

        if (insert_data.size() <= 20) {
            std::cout << "debug show " << tree.name() << " (size " << insert_data.size() << ") after insert (" << desc << "):\n";
            tree.print();
        }

        // поиск
        start = std::chrono::high_resolution_clock::now();
        for (int x : search_data) {
            tree.search(x);
        }
        stop = std::chrono::high_resolution_clock::now();
        result.time_search = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

        // удаление
        start = std::chrono::high_resolution_clock::now();
        for (int x : remove_data) {
            tree.remove(x);
        }
        stop = std::chrono::high_resolution_clock::now();
        result.time_remove = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

        if (insert_data.size() <= 20) {
            std::cout << "debug show " << tree.name() << " (size " << insert_data.size() << ") after remove (" << desc << "):\n";
            tree.print();
        }

        result.size   = tree.size();
        result.height = tree.height();
        result.checks = tree.print_checks();
        result.name   = tree.name();
        result.desc   = desc;
        return result;
    }

    void print_stats(const std::vector<TreeStats>& stats) {
        std::cout << std::left  << std::setw(10) << "Tree"
                  << std::left  << std::setw(15) << "Description"
                  << std::right << std::setw(15) << "Insert (ns)"
                  << std::right << std::setw(15) << "Search (ns)"
                  << std::right << std::setw(15) << "Remove (ns)"
                  << std::right << std::setw(12) << "Size"
                  << std::right << std::setw(12) << "Height"
                  << std::left  << "  Checks"
                  << std::endl;

        for (const auto& st : stats) {
            std::cout << std::left  << std::setw(10) << st.name
                      << std::left  << std::setw(15) << st.desc
                      << std::right << std::setw(15) << st.time_insert.count()
                      << std::right << std::setw(15) << st.time_search.count()
                      << std::right << std::setw(15) << st.time_remove.count()
                      << std::right << std::setw(12) << st.size
                      << std::right << std::setw(12) << st.height
                      << std::left  << "  " << st.checks
                      << std::endl;
        }
    }
};

template<typename Tree>
void sanity_check()
{
    Tree tree;
    std::cout << "\n--- ПРОВЕРКА АДЕКВАТНОСТИ (" << tree.name() << ") ---\n";

    std::cout << "Вставляем: 5, 3, 7, 1, 9\n";
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(1);
    tree.insert(9);
    std::cout << "Дерево: высота=" << tree.height() << "  размер=" << tree.size() << "\n";
    tree.print();

    std::cout << "Поиск 3: " << (tree.search(3) ? "найден (успех)" : "не найден (неудача)") << "\n";
    std::cout << "Поиск 8: " << (tree.search(8) ? "найден (неудача)" : "не найден (успех)") << "\n";

    std::cout << "Удаляем: 3\n";
    tree.remove(3);
    std::cout << "Дерево: высота=" << tree.height() << "  размер=" << tree.size() << "\n";

    std::cout << "Поиск 3: " << (tree.search(3) ? "найден (неудача)" : "не найден (успех)") << "\n";
    std::cout << "Поиск 9: " << (tree.search(9) ? "найден (успех)" : "не найден (неудача)") << "\n";
    tree.print();
}

int main(int argc, char** argv)
{
    std::cout << "Программа оценки работы алгоритмов деревьев поиска (BST, AVL, Treap)" << std::endl;

    sanity_check<BST>();
    sanity_check<AVL_V1>();
    sanity_check<AVL_V2>();
    sanity_check<Treap>();
    sanity_check<RandBST>();
    sanity_check<Splay>();

    std::cout << "\n--- ТЕСТИРОВАНИЕ ПРОИЗВОДИТЕЛЬНОСТИ ---\n";

    // Сравнивается работа на случайных и упорядоченных данных
    // 1. Замеряется время вставки N элементов
    // 2. Поиск N/10 случайных элементов
    // 3. Удаление N/10 случайных элементов
    std::vector<size_t> sizes = {1000, 5000, 10000, 50000, 100000};
    // std::vector<size_t> sizes = {1000};
    for (size_t sz : sizes) {
        PerformanceTester tester(sz);
        tester.run();
    }

    return 0;
}
