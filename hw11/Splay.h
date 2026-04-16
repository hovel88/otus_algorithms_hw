#pragma once

#include "ITree.h"

class Splay : public ITree
{
private:
    struct Node {
        int   data{0};
        Node* left{nullptr};
        Node* right{nullptr};
        Node* parent{nullptr};  // для удобства splay операций

        Node(int value) : data(value) {}
    };

    typedef struct BalanceResult {
        bool balanced{false};
        int height{0};
    } BR_t;

public:
    virtual ~Splay();
    Splay() = default;
    Splay(const Splay&) = delete;
    Splay(Splay&&) = delete;
    Splay& operator=(const Splay&) = delete;
    Splay& operator=(Splay&&) = delete;

    std::string name() const override { return "Splay"; }
    void print() const override       { print_(root_, 0); }
    std::string print_checks() const override;

    bool empty() const override  { return root_ == nullptr; }
    int  size() const override   { return size_(root_); }
    int  height() const override { return height_(root_); }

    void insert(int x) override;
    void remove(int x) override;
    bool search(int x) override;

private:
    void  print_(Node* node, int indent) const;
    bool  check_bst_(Node* node) const;
    bool  check_bst_rec_(Node* node, Node*& prev) const;
    bool  check_balance_(Node* node) const;
    BR_t  check_balance_rec_(Node* node) const;
    void  clear_(Node* node);
    int   size_(Node* node) const;
    int   height_(Node* node) const;
    Node* find_min_(Node* node) const;

    Node* find_node_(int x);
    void splay_(Node* n);
    void right_rot_(Node* n);
    void left_rot_(Node* n);

private:
    Node* root_{nullptr};
};
