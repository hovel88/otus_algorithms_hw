#pragma once

#include <vector>
#include "ITree.h"

class AVL_V1 : public ITree
{
private:
    struct Node {
        int     data{0};
        int     height{0};
        Node*   left{nullptr};
        Node*   right{nullptr};

        Node(int value)
        :   data(value),
            height(1)
        {}
    };

public:
    virtual ~AVL_V1();
    AVL_V1() = default;
    AVL_V1(const AVL_V1&) = delete;
    AVL_V1(AVL_V1&&) = delete;
    AVL_V1& operator=(const AVL_V1&) = delete;
    AVL_V1& operator=(AVL_V1&&) = delete;

    std::string name() const override { return "AVL_V1"; }
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
    bool  check_bst_(Node* node) const;
    bool  check_bst_rec_(Node* node, Node*& prev) const;
    bool  check_balance_(Node* node) const;
    void  clear_(Node* node);
    int   size_(Node* node) const;      // возвращает количество элементов в дереве
    int   height_(Node* node) const;    // возвращает высоту дерева
    Node* find_min_(Node* node) const;  // для поиска минимального значения в поддереве
    Node* insert_(Node* node, int x);   // рекурсивная вставка
    Node* remove_(Node* node, int x);   // удаление при нескольких случаях (нет потомков, один потомок, два потомка)
    Node* search_(Node* node, int x);   // рекурсивный поиск

    void  update_height_(Node* node);   // пересчитывает уровни дерева
    int   balance_(Node* node) const;   // возвращает коэффициент баланса для узла
    Node* rebalance_(Node* node);       // перебалансирует дерево от узла
    Node* small_right_rot_(Node* node); // малый правый поворот
    Node* small_left_rot_(Node* node);  // малый левый поворот
    Node* big_right_rot_(Node* node);   // большой правый поворот (левый-правый)
    Node* big_left_rot_(Node* node);    // большой левый поворот (правый-левый)

private:
    Node* root_{nullptr};
};
