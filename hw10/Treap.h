#pragma once

#include <random>
#include <vector>
#include "ITree.h"

class Treap : public ITree
{
private:
    struct Node {
        int     data{0};
        int     priority{0};
        Node*   left{nullptr};
        Node*   right{nullptr};

        Node(int value)
        :   data(value),
            priority(rand())
        {}
    };

public:
    virtual ~Treap();
    Treap();
    Treap(const Treap&) = delete;
    Treap(Treap&&) = delete;
    Treap& operator=(const Treap&) = delete;
    Treap& operator=(Treap&&) = delete;

    std::string name() const override { return "Treap"; }
    void print() const override        { print_(root_); }
    std::string print_checks() const override;

    bool empty() const override    { return root_ == nullptr; }
    int  size() const override     { return size_(root_); }
    int  height() const override   { return height_(root_); }

    void insert(int x) override { root_ = insert_(root_, x); }
    void remove(int x) override { root_ = remove_(root_, x); }
    bool search(int x) override { return search_(root_, x) != nullptr; }

private:
    void  print_(Node* node) const;
    bool  check_bst_() const;
    bool  check_balance_(Node* node) const;
    bool  check_heap_(Node* node) const;
    void  clear_(Node* node);
    void  inorder_(Node* node, std::vector<int>& result) const;
    int   size_(Node* node) const;      // возвращает количество элементов в дереве
    int   height_(Node* node) const;    // возвращает высоту дерева
    int   balance_(Node* node) const;   // возвращает коэффициент баланса для узла
    Node* find_min_(Node* node) const;  // для поиска минимального значения в поддереве
    Node* insert_(Node* node, int x);   // рекурсивная вставка
    Node* remove_(Node* node, int x);   // удаление
    Node* search_(Node* node, int x);   // рекурсивный поиск

    std::pair<Node*, Node*> split_(Node* node, int x); // разделение от узла на два поддерева и вернет вершины
    Node* merge_(Node* left, Node* right);             // слияние двух деревьев (все ключи в left < ключей в right)

private:
    Node* root_{nullptr};
    std::mt19937 rng_;
};
