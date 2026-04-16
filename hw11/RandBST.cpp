#include <algorithm>
#include <iostream>

#include "RandBST.h"

RandBST::~RandBST()
{
    clear_(root_);
}

RandBST::RandBST()
:   root_(nullptr),
    rng_(std::random_device{}()),
    dist_(0, 1000000)
{
}

RandBST::Node* RandBST::join_(Node* left, Node* right)
{
    if (!left) return right;
    if (!right) return left;

    /*
     * по факту мы имеем два поддерева с корнями 'l' и 'r',
     * причем любой ключ первого дерева меньше любого ключа
     * во втором дереве. в качестве корня нового дерева можно
     * взять любой из двух корней. если это будет 'l', тогда
     * левое поддерево 'l' можно оставить как есть, а справа
     * к 'l' подвесить объединение двух деревьев: правого
     * поддерева 'l' и всего дерева 'r'. но также можно выбрать
     * в качестве вершины и 'r'.
     *
     *     l       r          l
     *    / \  +  / \   =>   / \
     *   A   B   C   D      A   (B+C+D)
     * 
     *     l       r               r
     *    / \  +  / \   =>        / \
     *   A   B   C   D     (A+B+C)   D
     *
     * выбор из этих вариантов объединения поддеревьев будем
     * делать также с вероятностью. если размер левого дерева
     * равен n, правого равен m, тогда 'l' станет новым корнем
     * с вероятностью n/(n+m), а 'r' с вероятностью m/(n+m).
     */

    if (dist_(rng_) % (left->size + right->size) < left->size) {
        left->right = join_(left->right, right);
        update_size_(left);
        return left;
    } else {
        right->left = join_(left, right->left);
        update_size_(right);
        return right;
    }
}

std::string RandBST::print_checks() const
{
    bool bst      = check_bst_(root_);
    bool balanced = check_balance_(root_);
    std::string str;
    str.append("[BST: ").append((bst ? "✓" : "✗")).append("]");
    str.append("[balance: ").append((balanced ? "✓" : "✗")).append("]");
    return str;
}

void RandBST::print_(Node* node, int indent) const
{
    if (!node) return;
    print_(node->right, indent + 2);
    std::cout << std::string(indent, ' ')
              << node->data << " (sz=" << node->size << ")"
              << ( node->left ? " L" : "")
              << ( node->right ? " R" : "")
              << std::endl;
    print_(node->left, indent + 2);
}

bool RandBST::check_bst_(Node* node) const
{
    Node* prev = nullptr;
    return check_bst_rec_(node, prev);
}

bool RandBST::check_bst_rec_(Node* node, Node*& prev) const
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

bool RandBST::check_balance_(Node* node) const
{
    return check_balance_rec_(node).balanced;
}

RandBST::BR_t RandBST::check_balance_rec_(Node* node) const
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

void RandBST::clear_(Node* node)
{
    if (!node) return;

    clear_(node->left);
    clear_(node->right);
    delete node;
}

int RandBST::size_(Node* node) const
{
    return node ? node->size : 0;
}

int RandBST::height_(Node* node) const
{
    return node ? (1 + std::max(height_(node->left), height_(node->right))) : 0;
}

RandBST::Node* RandBST::insert_(Node* node, int x)
{
    if (!node) return new Node(x);

    // если заранее перемешать все ключи и потом построить из них дерево,
    // то построенное дерево окажется неплохо сбалансированным (считается,
    // что его высота будет порядка (2 log N) против (log N) для идеально
    // сбалансированного дерева). в этом случае корнем может с одинаковой
    // вероятностью оказаться любой из исходных ключей. но нам заранее ключи
    // не известны, поэтому применяем вероятностную оценку:
    // - с вероятностью 1/(size+1) вставляем в корень,
    // - с вероятностью 1-(1/(size+1)) вставляем как в обычное бинарное дерево

    if (dist_(rng_) % (node->size + 1) == 0) {
        return put_root_(node, x);
    }

    if (x < node->data) {
        node->left = insert_(node->left, x);
    } else
    if (x > node->data) {
        node->right = insert_(node->right, x);
    } else {
        // x == node->data
        // return node;
    }

    update_size_(node);
    return node;
}

RandBST::Node* RandBST::remove_(Node* node, int x)
{
    if (!node) return nullptr;

    if (x < node->data) {
        node->left = remove_(node->left, x);
    } else
    if (x > node->data) {
        node->right = remove_(node->right, x);
    } else {
        // x == node->data
        // нашли узел для удаления

        // объединяем левое и правое поддеревья найденного узла,
        // удаляем узел, возвращаем корень объединенного дерева
        Node* joined = join_(node->left, node->right);
        delete node;
        return joined;
    }

    update_size_(node);
    return node;
}

RandBST::Node* RandBST::search_(Node* node, int x)
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

void RandBST::update_size_(Node* node)
{
    if (!node) return;
    node->size = 1
               + (node->left ? node->left->size : 0)
               + (node->right ? node->right->size : 0);
}

RandBST::Node* RandBST::put_root_(Node* node, int x)
{
    if (!node) return new Node(x);

    // вставка нового узла с ключом 'x' в корень дерева 'node'.
    // сначала рекурсивно вставляем новый ключ в корень левого
    // или правого поддеревьев (в зависимости от результата сравнения
    // с корневым ключом) и выполняем правый (левый) поворот,
    // который поднимает нужный нам узел в корень дерева

    if (x < node->data) {
        // положили новый узел в левый потомок корневого узла,
        // затем подняли его через правый поворот
        node->left = put_root_(node->left, x);
        node = right_rot_(node);
    } else
    if (x > node->data) {
        // положили новый узел в правый потомок корневого узла,
        // затем подняли его через левый поворот
        node->right = put_root_(node->right, x);
        node = left_rot_(node);
    }

    return node;
}

RandBST::Node* RandBST::right_rot_(Node* n)
{
    /*
     * для вставки нового рандомного корня, потребуется два варианта
     * поворота: правый и левый.
     * при повороте вокруг узла 'n', мы должны сделать новым корнем
     * узел 'r' и не забыть обновить размеры для поддеревьев
     * после каждой операции
     *
     *       n             r
     *      / \           / \
     *     r   nR  ==>  rL   n
     *    / \               / \
     *   rL  rR            rR  nR
     */

    Node* r = n->left;
    if (!r) return n;
    n->left  = r->right;
    r->right = n;

    update_size_(n);
    update_size_(r);
    return r;
}

RandBST::Node* RandBST::left_rot_(Node* n)
{
    /*
     * для вставки нового рандомного корня, потребуется два варианта
     * поворота: правый и левый.
     * при повороте вокруг узла 'n', мы должны сделать новым корнем
     * узел 'r' и не забыть обновить размеры для поддеревьев
     * после каждой операции
     *
     *       n               r
     *      / \             / \
     *    nL   r     ==>   n   rR
     *        / \         / \
     *       rL rR       nL rL
     */

    Node* r = n->right;
    if (!r) return n;
    n->right = r->left;
    r->left  = n;

    update_size_(n);
    update_size_(r);
    return r;
}
