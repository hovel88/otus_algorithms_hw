#pragma once

#include <vector>
#include "ITree.h"

class BST : public ITree
{
private:
    struct Node {
        int     data{0};
        Node*   left{nullptr};
        Node*   right{nullptr};

        Node(int value)
        :   data(value)
        {}
    };

public:
    virtual ~BST();
    BST() = default;
    BST(const BST&) = delete;
    BST(BST&&) = delete;
    BST& operator=(const BST&) = delete;
    BST& operator=(BST&&) = delete;

    std::string name() const override { return "BST"; }
    void print() const override       { print_(root_); }
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
    void  clear_(Node* node);
    void  inorder_(Node* node, std::vector<int>& result) const;
    int   size_(Node* node) const;      // возвращает количество элементов в дереве
    int   height_(Node* node) const;    // возвращает высоту дерева
    int   balance_(Node* node) const;   // возвращает коэффициент баланса для узла
    Node* find_min_(Node* node) const;  // для поиска минимального значения в поддереве
    Node* insert_(Node* node, int x);   // рекурсивная вставка
    Node* remove_(Node* node, int x);   // удаление при нескольких случаях (нет потомков, один потомок, два потомка)
    Node* search_(Node* node, int x);   // рекурсивный поиск

private:
    Node* root_{nullptr};
};
