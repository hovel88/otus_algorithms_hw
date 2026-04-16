#pragma once

#include <random>

#include "ITree.h"

class RandBST : public ITree
{
private:
    struct Node {
        int   data{0};
        int   size{1};
        Node* left{nullptr};
        Node* right{nullptr};

        Node(int value) : data(value) {}
    };

    typedef struct BalanceResult {
        bool balanced{false};
        int height{0};
    } BR_t;

public:
    virtual ~RandBST();
    RandBST();
    RandBST(const RandBST&) = delete;
    RandBST(RandBST&&) = delete;
    RandBST& operator=(const RandBST&) = delete;
    RandBST& operator=(RandBST&&) = delete;

    std::string name() const override { return "RandBST"; }
    void print() const override       { print_(root_, 0); }
    std::string print_checks() const override;

    bool empty() const override  { return root_ == nullptr; }
    int  size() const override   { return size_(root_); }
    int  height() const override { return height_(root_); }

    void insert(int x) override { root_ = insert_(root_, x); }
    void remove(int x) override { root_ = remove_(root_, x); }
    bool search(int x) override { return search_(root_, x) != nullptr; }

private:
    void  print_(Node* node, int indent) const;
    bool  check_bst_(Node* node) const;
    bool  check_bst_rec_(Node* node, Node*& prev) const;
    bool  check_balance_(Node* node) const;
    BR_t  check_balance_rec_(Node* node) const;
    void  clear_(Node* node);
    int   size_(Node* node) const;
    int   height_(Node* node) const;
    Node* insert_(Node* node, int x);
    Node* remove_(Node* node, int x);
    Node* search_(Node* node, int x);

    void  update_size_(Node* node);
    Node* put_root_(Node* node, int x);
    Node* right_rot_(Node* n);
    Node* left_rot_(Node* n);
    Node* join_(Node* left, Node* right);

private:
    Node*                                   root_{nullptr};
    mutable std::mt19937                    rng_;
    mutable std::uniform_int_distribution<> dist_;
};
