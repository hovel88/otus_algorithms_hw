#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

// реализация системы непересекающихся множеств
class UnionFind
{
private:
    std::vector<int> parent_{}; // хранилище родителей вершин
    std::vector<int> size_{};   // размер множества, позволяет прилеплять меньшее множество к большему
    int              max_{0};   // максимальный размер индекса в множестве

public:
    ~UnionFind() = default;
    UnionFind() = delete;
    UnionFind(const UnionFind& other)
    :   parent_(other.parent_),
        size_(other.size_),
        max_(other.max_) {}
    UnionFind(UnionFind&& other) noexcept
    :   parent_(std::move(other.parent_)),
        size_(std::move(other.size_)),
        max_(other.max_)
    {
        other.max_ = 0;
    }

    UnionFind& operator=(const UnionFind& other) {
        if (this != &other) {
            UnionFind temp(other);
            swap(temp); // пользуем концепцию swap, она noexcept
        }
        return *this;
    }
    UnionFind& operator=(UnionFind&& other) noexcept {
        if (this != &other) {
            parent_ = std::move(other.parent_);
            size_ = std::move(other.size_);
            max_ = other.max_;
            other.max_ = 0;
        }
        return *this;
    }

    explicit UnionFind(int n)
    :   max_(n)
    {
        if (n <= 0) throw std::invalid_argument("UnionFind: размер должен быть положительным, получено " + std::to_string(n));

        parent_.resize(n);
        size_.resize(n, 1);

        // инициализированное состояние:
        // изначально каждая вершина является корнем своего множества
        for (int i = 0; i < n; i++) {
            parent_[i] = i;
        }
    }

    void swap(UnionFind& other) noexcept {
        std::swap(parent_, other.parent_);
        std::swap(size_, other.size_);
        std::swap(max_, other.max_);
    }

    int find(int x) {
        check_index_(x, "find");

        // ищем представителя множества, рекурсивно переходя к корню
        // множества, т.е. главному прародителю всего множества.
        // при этом выполняется так называемое "сжатие пути", что позволяет
        // получить сложность алгоритма - O(1) с "амортизацией",
        // т.е. последующие запросы отработают значительно быстрее
        if (parent_[x] != x) {
            parent_[x] = find(parent_[x]);
        }
        return parent_[x];
    }

    bool merge(int x, int y) {
        check_index_(x, "merge");
        check_index_(y, "merge");

        // объединить множества, содержащие x и y.
        // должны вернуть true, только если объединение произошло,
        // но если x и y уже относятся к одному множеству, то объединять нечего

        int root_x = find(x);
        int root_y = find(y);

        if (root_x == root_y) return false;

        // объединяем по размеру. пишут что такой подход работает лучше,
        // когда объединяются множества так, что меньшее прилепляется к большему
        if (size_[root_x] < size_[root_y]) {
            parent_[root_x] = root_y;
            size_[root_y] += size_[root_x];
            size_[root_x] -= size_[root_x];
        } else {
            parent_[root_y] = root_x;
            size_[root_x] += size_[root_y];
            size_[root_y] -= size_[root_y];
        }
        return true;
    }

    bool connected(int x, int y) const {
        check_index_(x, "connected");
        check_index_(y, "connected");

        // true, если x и y находятся в одном и том же множестве,
        // т.е. имеют единого прародителя множества
        return find_only_(x) == find_only_(y);
    }

    int get_size(int x) const {
        check_index_(x, "get_size");

        // возвращает размер множества, в котором находится элемент
        return size_[find_only_(x)];
    }

    int get_size() const {
        return max_;
    }

    int sets_count() const {
        std::set<int> roots;
        for (int i = 0; i < max_; ++i) {
            roots.insert(find_only_(i));
        }
        return roots.size();
    }

    void print() const {
        std::vector<std::vector<int>> sets(max_);
        for (int i = 0; i < max_; ++i) {
            sets[find_only_(i)].push_back(i);
        }
        sets.erase(std::remove_if(sets.begin(), sets.end(),
            [](const std::vector<int>& s) { return s.empty(); }),
            sets.end());

        std::cout << "Union-Find (размер=" << max_ << ", множеств=" << sets_count() << "):";
        std::cout << "\n  index : ";
        for (int i = 0; i < max_; ++i) std::cout << i << " ";
        std::cout << "\n  parent: ";
        for (int i = 0; i < max_; ++i) std::cout << parent_[i] << " ";
        std::cout << "\n  size  : ";
        for (int i = 0; i < max_; ++i) std::cout << size_[i] << " ";
        std::cout << "\n  множества:\n";
        for (size_t i = 0; i < sets.size(); i++) {
            std::cout << "    " << i << ": { ";
            for (int v : sets[i]) {
                std::cout << v << " ";
            }
            std::cout << "}\n";
        }
    }

private:
    void check_index_(int x, const std::string& operation) const {
        if (x < 0) {
            throw std::out_of_range("UnionFind::" + operation + ": отрицательный индекс " + std::to_string(x) + " недопустим");
        }
        if (x >= max_) {
            throw std::out_of_range("UnionFind::" + operation + ": индекс " + std::to_string(x) + " вне диапазона [0, " + std::to_string(max_ - 1) + "]");
        }
    }

    int find_only_(int x) const {
        // тоже самое как и find(), только не производится "сжатие пути"
        // до родителя, и таким образом сложность всегда O(N)
        int root = x;
        while (parent_[root] != root) {
            root = parent_[root];
        }
        return root;
    }
};
