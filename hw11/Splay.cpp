#include <algorithm>
#include <iostream>
#include <stack>

#include "Splay.h"

Splay::~Splay()
{
    clear_(root_);
}

std::string Splay::print_checks() const
{
    bool bst = check_bst_(root_);
    bool balanced = check_balance_(root_);
    std::string str;
    str.append("[BST: ").append((bst ? "✓" : "✗")).append("]");
    str.append("[balance: ").append((balanced ? "✓" : "✗")).append("]");
    return str;
}

void Splay::insert(int x)
{
    if (!root_) {
        root_ = new Node(x);
        return;
    }

    // после вставки нового элемента в Splay, он должен оказаться в корне дерева.
    // однако такой элемент уже может существовать в дереве. поэтому сначала
    // попытаемся его найти. поиск узла в Splay переместит его в корень,
    // в общем-то, это то, что нам и надо.
    // однако, если узел уже есть, то добавлять еще один в дерево не будем
    if (find_node_(x)) return;

    // если мы тут, значит такого элемента в дереве нет, но после поиска
    // в корень дерева был перемещен последний посещенный узел, который
    // или больше или меньше значения 'x', но самый ближайший по величине к 'x'.
    Node* node = new Node(x);
    if (x < root_->data) {
        // левое поддерево корня перевешиваем слева от узла,
        // а справа от узла навешиваем бывший корень

        if (root_->left) root_->left->parent = node;

        node->left  = root_->left;
        node->right = root_;

        root_->parent = node;
        root_->left   = nullptr;
    } else
    if (x > root_->data) {
        // правое поддерево корня перевешиваем справа от узла,
        // а слева от узла навешиваем бывший корень

        if (root_->right) root_->right->parent = node;

        node->right = root_->right;
        node->left  = root_;

        root_->parent = node;
        root_->right  = nullptr;
    }

    root_ = node;
}

void Splay::remove(int x)
{
    // сначала нужно найти элемент.
    // после поиска найденный элемент переместится на самый верх/
    // однако если элемента уже нет, то и хорошо
    Node* node = find_node_(x);
    if (!node) return;

    // если мы тут, значит узел был найден и вытащен в корень дерева.
    // теперь его можно спокойно удалять.
    // дальше работаем с дочерними левым и правым поддеревьями
    Node* l_tree = node->left;
    Node* r_tree = node->right;
    delete node;
    if (l_tree) l_tree->parent = nullptr;
    if (r_tree) r_tree->parent = nullptr;

    // может быть ситуация, что у нас есть только одно дочернее поддерево,
    // тогда его и оставим в качестве нового дерева
    if (!l_tree) {
        root_ = r_tree;
        return;
    }
    if (!r_tree) {
        root_ = l_tree;
        return;
    }

    // если у нас есть и левое и правое поддерево, то объединяем их.
    // для этого находим минимальный элемент в правом поддереве,
    // вытаскиваем его в корень дерева, а затем в качестве левой ветки
    // пристегиваем левое поддерево удаленного узла.
    // т.о. сохраняется свойство бинарного дерева
    Node* min_right = find_min_(node->right);
    splay_(min_right);
    min_right->left = l_tree;
    l_tree->parent  = min_right;

    root_ = min_right;
}

bool Splay::search(int x)
{
    return find_node_(x) != nullptr;
}

void Splay::print_(Node* node, int indent) const
{
    if (!node) return;
    print_(node->left, indent + 2);
    std::cout << std::string(indent, ' ')
              << node->data
              << ( node->left ? " L" : "")
              << ( node->right ? " R" : "")
              << std::endl;
    print_(node->right, indent + 2);
}

bool Splay::check_bst_(Node* node) const
{
    Node* prev = nullptr;
    return check_bst_rec_(node, prev);
}

bool Splay::check_bst_rec_(Node* node, Node*& prev) const
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

bool Splay::check_balance_(Node* node) const
{
    return check_balance_rec_(node).balanced;
}

Splay::BR_t Splay::check_balance_rec_(Node* node) const
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

void Splay::clear_(Node* node)
{
    if (!node) return;

    clear_(node->left);
    clear_(node->right);
    delete node;
}

int Splay::size_(Node* node) const
{
    return node ? (1 + size_(node->left) + size_(node->right)) : 0;
}

int Splay::height_(Node* node) const
{
    return node ? (1 + std::max(height_(node->left), height_(node->right))) : 0;
}

Splay::Node* Splay::find_min_(Node* node) const
{
    while (node->left) node = node->left;
    return node;
}

Splay::Node* Splay::find_node_(int x)
{
    Node* curr = root_;
    Node* last = nullptr;
    while (curr) {
        last = curr;
        if (x < curr->data) {
            curr = curr->left;
        } else
        if (x > curr->data) {
            curr = curr->right;
        } else {
            // x == curr->data

            // нашли нужный узел, Splay вытаскивает в корень узел
            // при поиске, своего рода функция кеширования.
            // т.е. считаем, что если этот узел понадобился сейчас,
            // то вероятно он понадобится и еще раз в ближайшее время
            splay_(curr);
            return curr;
        }
    }

    // даже если мы не нашли подходящий узел,
    // мы должны вытянуть на место корня последний посещенный узел
    if (last) splay_(last);
    return nullptr;
}

void Splay::splay_(Node* n)
{
    if (!n) return;

    // нам нужно вытащить указанный узел на самый верх дерева.
    // выполняем это до тех пор, пока у узла существует предок
    while (n->parent) {
        // будем оперировать понятиями "папа" (узел p), т.е. непосредственный
        // предок нашего узла. и "дедушка" (узел g), т.е. родитель родителя
        // нашего узла.
        // и тут в Splay есть несколько вариантов действий:
        // - если "дедушка" есть
        // - если "дедушки" нет

        Node* p = n->parent;
        Node* g = p->parent;
        if (!g) {
            // "дедушки" нет, тогда у нас один из вариантов:
            // - один правый (Zig) поворот
            // - один левый (Zag) поворот
            if (p->left == n) {
                /*
                 * Zig - один правый поворот
                 * splay(4):
                 *       6             4
                 *      / \    Zig    / \
                 *     4   7   ==>   3   6
                 *    / \               / \
                 *   3   5             5   7
                 */
                right_rot_(n);
            } else
            if (p->right == n) {
                /*
                 * Zag - один левый поворот
                 * splay(6):
                 *       4                 6
                 *      / \      Zag      / \
                 *     3   6     ==>     4   7
                 *        / \           / \
                 *       5   7         3   5
                 */
                left_rot_(n);
            }
        } else {
            // "дедушка" есть, тогда тут варианты:
            // - два малых правых (Zig-Zig) поворота
            // - два малых левых (Zag-Zag) поворота
            // - малый правый и малый левый (Zig-Zag) повороты
            // - малый левый и малый правый (Zag-Zig) повороты
            if ((g->left == p) && (p->left == n)) {
                /*
                 * Zig-Zig - два правых поворота
                 * splay(3):
                 *       6             4             3
                 *      / \    Zig    / \      Zig    \
                 *     4   7   ==>   3   6     ==>     4
                 *    / \               / \             \
                 *   3   5             5   7             6
                 *                                      / \
                 *                                     5   7
                 */
                right_rot_(p);
                right_rot_(n);
            } else
            if ((g->right == p) && (p->right == n)) {
                /*
                 * Zag-Zag - два левых поворота
                 * splay(7):
                 *       4                6                 7
                 *      / \     Zag      / \    Zag        /
                 *     3   6    ==>     4   7   ==>       6
                 *        / \          / \               /
                 *       5   7        3   5             4
                 *                                     / \
                 *                                    3   5
                 */
                left_rot_(p);
                left_rot_(n);
            } else
            if ((g->right == p) && (p->left == n)) {
                /*
                 * Zig-Zag - правый и левый повороты
                 * splay(5):
                 *       4               4                   5
                 *      / \      Zig    / \        Zag      / \
                 *     3   6     ==>   3   5       ==>     4   6
                 *        / \               \             /     \
                 *       5   7               6           3       7
                 *                            \
                 *                             7
                 */
                right_rot_(n);
                left_rot_(n);
            } else
            if ((g->left == p) && (p->right == n)) {
                /*
                 * Zag-Zig - левый и правый повороты
                 * splay(5):
                 *       6                 6               5
                 *      / \    Zag        / \    Zig      / \
                 *     4   7   ==>       5   7   ==>     4   6
                 *    / \               /               /     \
                 *   3   5             4               3       7
                 *                    /
                 *                   3
                 */
                left_rot_(n);
                right_rot_(n);
            }
        }
    }
    root_ = n;
}

void Splay::right_rot_(Node* n)
{
    /*
     * будем оперировать понятием "папа" (узел p),
     *   т.е. непосредственный предок нашего узла.
     * и понятием "дедушка" (узел g),
     *   т.е. родитель родителя нашего узла
     *
     *       6     (4)     4
     *      / \    Zig    / \
     *     4   7   ==>   3   6
     *    / \               / \
     *   3   5             5   7
     */

    Node* p = n->parent;
    Node* g = p->parent;

    p->left = n->right;
    if (n->right) n->right->parent = p;
    n->right = p;

    p->parent = n;
    n->parent = g;

    // дополнительно нужно обновить указатели у "дедушки",
    // чтобы дерево не рассыпалось
    if (g) {
        if (g->left == p) {
            g->left = n;
        } else {
            g->right = n;
        }
    }
}

void Splay::left_rot_(Node* n)
{
    /*
     * будем оперировать понятием "папа" (узел p),
     *   т.е. непосредственный предок нашего узла.
     * и понятием "дедушка" (узел g),
     *   т.е. родитель родителя нашего узла
     *
     *       4       (6)       6
     *      / \      Zag      / \
     *     3   6     ==>     4   7
     *        / \           / \
     *       5   7         3   5
     */

    Node* p = n->parent;
    Node* g = p->parent;

    p->right = n->left;
    if (n->left) n->left->parent = p;
    n->left = p;

    p->parent = n;
    n->parent = g;

    // дополнительно нужно обновить указатели у "дедушки",
    // чтобы дерево не рассыпалось
    if (g) {
        if (g->left == p) {
            g->left = n;
        } else {
            g->right = n;
        }
    }
}
