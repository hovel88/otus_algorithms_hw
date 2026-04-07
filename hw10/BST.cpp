#include <algorithm>
#include <iostream>

#include "BST.h"

BST::~BST()
{
    clear_(root_);
}

std::string BST::print_checks() const
{
    bool bst      = check_bst_(root_);
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

bool BST::check_bst_(Node* node) const
{
    Node* prev = nullptr;
    return check_bst_rec_(node, prev);
}

bool BST::check_bst_rec_(Node* node, Node*& prev) const
{
    if (!node) return true;

    // рекурсивно обходим дерево слева-направо и в глубину.
    // просто проверяем значение на >= и сразу возвращаем булевый
    // результат вместо формирования вектора и его дальнейшего
    // анализа на упорядоченность. в результате можно на лету
    // за один проход определить что свойство не выполняется
    // и досрочно завершить рекурсию, т.о. сложность становится:
    // O(1) по памяти и O(N) по времени.
    // кроме того, если данные будут не простыми int, а сложными
    // объектами, то добавляется необходимость определить операции
    // сравнения этих объектов (достаточно операций < и ==, остальные
    // можно вывести из них)

    if (!check_bst_rec_(node->left, prev)) return false;
    if (prev && prev->data >= node->data) return false;
    prev = node;
    return check_bst_rec_(node->right, prev);
}

bool BST::check_balance_(Node* node) const
{
    return check_balance_rec_(node).balanced;
}

BST::BR_t BST::check_balance_rec_(Node* node) const
{
    if (!node) return {true, 0};

    // рекурсивно обходим дерево слева-направо и в глубину.
    // вместо того, чтобы рекурсивно обходить дерево и для каждого
    // узла опять и опять рекурсивно высчитывать высоту (т.к. для BST
    // высота не хранится в самом дереве) делая очень много обходов дерева,
    // мы сразу на лету подсчитываем высоту и определяем сбалансированность
    // поддерева, т.о. можем сразу вернуть результат и прервать рекурсию,
    // т.е. выполняем за один обход дерева, сложность O(N)

    auto left_res = check_balance_rec_(node->left);
    if (!left_res.balanced) return {false, 0};

    auto right_res = check_balance_rec_(node->right);
    if (!right_res.balanced) return {false, 0};

    int balance = left_res.height - right_res.height;
    if (std::abs(balance) > 1) return {false, 0};

    return {true, 1 + std::max(left_res.height, right_res.height)};
}

void BST::clear_(Node* node)
{
    if (!node) return;

    clear_(node->left);
    clear_(node->right);
    delete node;
}

int BST::size_(Node* node) const
{
    return node ? ( 1 + size_(node->left) + size_(node->right) ) : 0;
}

int BST::height_(Node* node) const
{
    return node ? ( 1 + std::max(height_(node->left), height_(node->right)) ) : 0;
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

        // случай 1: нет потомков или есть только один потомок.
        // можем безопасно удалять узел, но вернуть указатель на
        // существующего одного потомка, чтобы потом приклеить
        // поддерево к родителю (этого потомка может и не быть, не страшно)
        if (!node->left) {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        if (!node->right) {
            Node* temp = node->left;
            delete node;
            return temp;
        }

        // случай 2: есть два потомка.
        // исходим из свойства, что в правом поддереве находятся
        // всегда элементы больше текущего. поэтому если мы возьмем
        // минимальный элемент в правом поддереве (удалив его из правого
        // поддерева конечно же) и поставим его на место текущего элемента,
        // то мы сохраним свойство как для правого, так и для левого
        // поддерева
        // кроме того, используем семантику перемещения, вместо копирования
        // на случай, если data это сложный объект, чтобы свести к минимуму
        // дорогостоящее копирование
        Node* min_right = find_min_(node->right);
        std::swap(node->data, min_right->data);
        node->right = remove_(node->right, min_right->data);
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
