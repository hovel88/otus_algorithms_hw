#include <algorithm>
#include <iostream>

#include "Treap.h"

Treap::~Treap()
{
    clear_(root_);
}

Treap::Treap()
:   root_(nullptr),
    rng_(std::random_device{}())
{
    std::srand(time(nullptr));
}

std::string Treap::print_checks() const
{
    bool bst      = check_bst_();
    bool balanced = check_balance_(root_);
    bool heap     = check_heap_(root_);
    std::string str;
    str.append("[BST: ").append(( bst ? "✓" : "✗" )).append("]");
    str.append("[balance: ").append(( balanced ? "✓" : "✗" )).append("]");
    str.append("[куча: ").append(( heap ? "✓" : "✗" )).append("]");
    return str;
}

void Treap::print_(Node* node) const
{
    if (!node) return;
    print_(node->left);
    std::cout << node->data << " (p=" << node->priority << ")"
              << ( node->left ? " L" : "")
              << ( node->right ? " R" : "")
              << std::endl;
    print_(node->right);
}

bool Treap::check_bst_() const
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

bool Treap::check_balance_(Node* node) const
{
    if (!node) return true;

    if (std::abs(balance_(node)) > 1) return false;
    return check_balance_(node->left) && check_balance_(node->right);
}

bool Treap::check_heap_(Node* node) const
{
    if (!node) return true;

    if (node->left  && node->left->priority  > node->priority)  return false;
    if (node->right && node->right->priority > node->priority)  return false;
    return check_heap_(node->left) && check_heap_(node->right);
}

void Treap::clear_(Node* node)
{
    if (!node) return;

    clear_(node->left);
    clear_(node->right);
    delete node;
}

void Treap::inorder_(Node* node, std::vector<int>& result) const
{
    // рекурсивно обходим дерево слева-направо и в глубину.
    // возвращаем последовательность элементов
    if (!node) return;

    inorder_(node->left, result);
    result.push_back(node->data);
    inorder_(node->right, result);
}

int Treap::size_(Node* node) const
{
    return node ? ( 1 + size_(node->left) + size_(node->right) ) : 0;
}

int Treap::height_(Node* node) const
{
    return node ? ( 1 + std::max(height_(node->left), height_(node->right)) ) : 0;
}

int Treap::balance_(Node* node) const
{
    return node ? ( height_(node->left) - height_(node->right) ) : 0;
}

Treap::Node* Treap::find_min_(Node* node) const
{
    while (node->left) node = node->left;
    return node;
}

Treap::Node* Treap::insert_(Node* node, int x)
{
    // попытаемся поделить дерево на два поддерева ПО КЛЮЧУ,
    // получим в одном - поддерев элементов строго меньше ключа 'x',
    // а в другом - больше или равно ключу 'x'.
    // далее надо убедиться, нет ли в правой части (больше или равно 'x')
    // дубликата узла с таким ключом. для этого попытаемся рассечь
    // правое поддерево на две части по ключу 'x+1', т.о. получим среднюю
    // часть (в которой должны быть элементы меньше 'x+1', т.е. получается
    // что именно 'x') и самую правую часть. в серединке может находиться дубликат,
    // но если его там нет, то создаем новый элемент в серединке.
    // далее для формирования итогового дерева, мы сперва объединяем левое
    // поддерево с серединкой, и затем это дерево объединяем с правым поддеревом,
    // т.е. теперь в множество добавился новый элемент. а если он там уже был,
    // то в множестве ничего не изменится

    auto [left, right] = split_(node, x);
    auto [mid, right2] = split_(right, x + 1);

    if (!mid) {
        mid = new Node(x); // дубликата не было, создаем элемент
    }

    return merge_(merge_(left, mid), right2);
}

Treap::Node* Treap::remove_(Node* node, int x)
{
    // удаление можно сделать через несколько операций split и merge.
    // т.е. не вызывать remove_() рекурсивно.
    // сначала разделить исходное дерево по ключу 'x' на две части:
    // левую (где строго меньше 'x') и правую (где больше или равно 'x').
    // затем правую часть еще раз разделить на две:
    // меньше 'x+1' (в которой будет находиться как раз 'x') и больше или равно 'x+1'.
    // а затем вызвать merge двух поддеревьев за исключением узла 'x'.

    // однако можно пойти чуть иначе, и рекурсивно найти узел с ключом 'x'.
    // объединить в новое поддерево левую и правую его ветви, и это новое поддерево
    // присвоить родителю узла с ключом 'x', исключив тем самым найденный узел.
    // хотя результат в целом будет правильным - новое дерево с исключенным узлом,
    // но вот сама структура дерева будет иной.
    // потому что рекурсия постарается полностью сохранить структуру узлов, в независимости
    // от приоритетов с приоритетами, элементы останутся в том же порядке.
    // а подход через split/merge создаст совершенно другую структуру и порядок слияния
    // будет зависеть от приоритетов корней

    auto [left, right] = split_(node, x);
    auto [mid, right2] = split_(right, x + 1);

    delete mid;

    return merge_(left, right2);
}

Treap::Node* Treap::search_(Node* node, int x)
{
    // поиск также как и удаление, можно реализовать как через операции
    // split/merge, или через рекурсивный вызов метода search_.
    // в первом случае, можно разделить исходное дерево на две части:
    // левую (меньше 'x') и правую (больше или равную 'x').
    // затем еще раз разделить правую часть на две, по ключу 'x+1', получим
    // серединку (где элементы меньше 'x+1', т.е. по факту как раз 'x')
    // и больше или равную 'x+1'.
    // эту серединку вернуть из функции. но перед этим снова собрать дерево
    // через две операции merge как в случае с insert.

    // но я считаю, что подход split/merge избыточен, много операций присваиваний.
    // поэтому сделаем через рекурсию

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

std::pair<Treap::Node*, Treap::Node*> Treap::split_(Node* node, int x)
{
    if (!node) return {nullptr, nullptr};

    // split в декартовом дереве происходит ПО КЛЮЧУ
    // разделяем исходное дерево (от узла node) на две части:
    // - элементы, которые меньше ключа 'x'
    // - элементы, которые больше или равны ключу 'x'
    // в результате образуется два поддерева (две иерархии),
    // корневые узлы которых мы вернем.
    if (node->data < x) {
        // в правом поддеревере (т.е. больше текущего узла) могут находиться элементы,
        // которые меньше ключа, поэтому пытаемся отсечь поддерево оттуда.
        // но т.к. это правое поддерево (т.е. элементы там больше текущего узла),
        // то для соблюдения свойства дерева - добавляем их в правое плечо
        auto [left, right] = split_(node->right, x);
        node->right = left;
        return {node, right};
    } else {
        // node->data >= x
        // в левом поддеревере (т.е. меньше текущего узла) могут находиться элементы,
        // которые больше ключа, поэтому пытаемся отсечь поддерево оттуда.
        // но т.к. это левое поддерево (т.е. элементы там меньше текущего узла),
        // то для соблюдения свойства дерева - добавляем их в левое плечо
        auto [left, right] = split_(node->left, x);
        node->left = right;
        return {left, node};
    }
}

Treap::Node* Treap::merge_(Node* left, Node* right)
{
    if (!left) return right;
    if (!right) return left;

    // merge в декартовом дереве происходит ПО ПРИОРИТЕТУ
    // по итогу из двух поддеревьев получаем одно

    if (left->priority > right->priority) {
        left->right = merge_(left->right, right);
        return left;
    } else {
        right->left = merge_(left, right->left);
        return right;
    }
}
