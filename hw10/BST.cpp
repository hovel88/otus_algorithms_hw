#include <algorithm>
#include <iostream>

#include "BST.h"

BST::~BST()
{
    clear_(root_);
}

std::string BST::print_checks() const
{
    bool bst      = check_bst_();
    bool balanced = check_balance_(root_);
    std::string str;
    str.append("[BST: ").append(( bst ? "✓" : "✗" )).append("]");
    str.append("[balance: ").append(( balanced ? "✓" : "✗" )).append("]");
    return str;
}

void BST::print_(Node* node) const
{
    if (!node) return;
    print_(node->left);
    std::cout << node->data
              << ( node->left ? " L" : "")
              << ( node->right ? " R" : "")
              << std::endl;
    print_(node->right);
}

bool BST::check_bst_() const
{
    auto get_inorder = [this]() -> std::vector<int> {
        std::vector<int> result;
        inorder_(root_, result);
        return result;
    };

    std::vector<int> inorder_list = get_inorder();
    for (size_t i = 1; i < inorder_list.size(); i++) {
        if (inorder_list[i] <= inorder_list[i-1]) return false;
    }
    return true;
}

bool BST::check_balance_(Node* node) const
{
    if (!node) return true;

    if (std::abs(balance_(node)) > 1) return false;
    return check_balance_(node->left) && check_balance_(node->right);
}

void BST::clear_(Node* node)
{
    if (!node) return;

    clear_(node->left);
    clear_(node->right);
    delete node;
}

void BST::inorder_(Node* node, std::vector<int>& result) const
{
    // рекурсивно обходим дерево слева-направо и в глубину.
    // возвращаем последовательность элементов
    if (!node) return;
    inorder_(node->left, result);
    result.push_back(node->data);
    inorder_(node->right, result);
}

int BST::size_(Node* node) const
{
    return node ? ( 1 + size_(node->left) + size_(node->right) ) : 0;
}

int BST::height_(Node* node) const
{
    return node ? ( 1 + std::max(height_(node->left), height_(node->right)) ) : 0;
}

int BST::balance_(Node* node) const
{
    return node ? ( height_(node->left) - height_(node->right) ) : 0;
}

BST::Node* BST::find_min_(Node* node) const
{
    while (node->left) node = node->left;
    return node;
}

BST::Node* BST::insert_(Node* node, int x)
{
    if (!node) return new Node(x);

    // в левых поддеревьях хранятся значения меньше текущего,
    // а в правых поддеревьях - значения больше текущего.
    // дубликаты не хранятся, поэтому просто возвращаем узел,
    // который уже был вставлен ранее
    if (x < node->data) {
        node->left = insert_(node->left, x);
    } else
    if (x > node->data) {
        node->right = insert_(node->right, x);
    } else {
        // x == node->data
        // return node;
    }
    return node;
}

BST::Node* BST::remove_(Node* node, int x)
{
    if (!node) return nullptr;

    if (x < node->data) {
        node->left = remove_(node->left, x);
    } else
    if (x > node->data) {
        node->right = remove_(node->right, x);
    } else {
        // x == node->data
        // нашли узел для удаления. и тут есть несколько случаев

        if (!node->left || !node->right) {
            // случай 1: нет потомков или есть только один потомок.
            // можем безопасно удалять узел, но вернуть указатель на
            // существующего одного потомка, чтобы потом приклеить
            // поддерево к родителю (этого потомка может и не быть, не страшно)
            Node* temp = node->left ? node->left : node->right;
            if (!temp) {
                // в данном случае нет ни левого, ни правого потомка. спокойно затираем узел
                temp = node;
                node = nullptr;
            } else {
                // в данном случае есть либо левый, либо правый потомок. используем его
                *node = *temp;
            }
            delete temp;
        } else {
            // случай 2: есть два потомка.
            // исходим из свойства, что в правом поддереве находятся
            // всегда элементы больше текущего. поэтому если мы возьмем
            // минимальный элемент в правом поддереве (удалив его из правого
            // поддерева конечно же) и поставим его на место текущего элемента,
            // то мы сохраним свойство как для правого, так и для левого
            // поддерева
            Node* min_right = find_min_(node->right);
            node->data = min_right->data;
            node->right = remove_(node->right, min_right->data);
        }

        // случай 1: нет потомков или только один потомок.
        // можем безопасно удалять узел, но вернуть указатель на
        // существующего одного потомка, чтобы потом приклеить
        // поддерево к родителю
        // if (node->left == nullptr) {
        //     Node* temp = node->right;
        //     delete node;
        //     return temp; // тут может быть и null, ничего страшного
        // } else
        // if (node->right == nullptr) {
        //     Node* temp = node->left;
        //     delete node;
        //     return temp; // тут может быть и null, ничего страшного
        // }
    }

    return node;
}

BST::Node* BST::search_(Node* node, int x)
{
    if (!node) return nullptr;

    if (x < node->data) {
        return search_(node->left, x);
    } else
    if (x > node->data) {
        return search_(node->right, x);
    } else {
        // x == node->data
        return node;
    }
}
