#include <algorithm>
#include <iostream>

#include "AVL_V2.h"

AVL_V2::~AVL_V2()
{
    clear_(root_);
}

std::string AVL_V2::print_checks() const
{
    bool bst      = check_bst_();
    bool balanced = check_balance_(root_);
    std::string str;
    str.append("[BST: ").append(( bst ? "✓" : "✗" )).append("]");
    str.append("[balance: ").append(( balanced ? "✓" : "✗" )).append("]");
    return str;
}

void AVL_V2::print_(Node* node) const
{
    if (!node) return;
    print_(node->left);
    std::cout << node->data << " (h=" << node->height << ")"
              << ( node->left ? " L" : "")
              << ( node->right ? " R" : "")
              << std::endl;
    print_(node->right);
}

bool AVL_V2::check_bst_() const
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

bool AVL_V2::check_balance_(Node* node) const
{
    if (!node) return true;

    if (std::abs(balance_(node)) > 1) return false;
    return check_balance_(node->left) && check_balance_(node->right);
}

void AVL_V2::clear_(Node* node)
{
    if (!node) return;

    clear_(node->left);
    clear_(node->right);
    delete node;
}

void AVL_V2::inorder_(Node* node, std::vector<int>& result) const
{
    // рекурсивно обходим дерево слева-направо и в глубину.
    // возвращаем последовательность элементов
    if (!node) return;
    inorder_(node->left, result);
    result.push_back(node->data);
    inorder_(node->right, result);
}

int AVL_V2::size_(Node* node) const
{
    return node ? ( 1 + size_(node->left) + size_(node->right) ) : 0;
}

int AVL_V2::height_(Node* node) const
{
    return node ? node->height : 0;
}

int AVL_V2::balance_(Node* node) const
{
    return node ? ( height_(node->left) - height_(node->right) ) : 0;
}

AVL_V2::Node* AVL_V2::find_min_(Node* node) const
{
    while (node->left) node = node->left;
    return node;
}

AVL_V2::Node* AVL_V2::insert_(Node* node, int x)
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
        return node;
    }
    // для AVL, в отличие от BST, нужно дополнительно отбаллансировать
    return rebalance_(node);
}

AVL_V2::Node* AVL_V2::remove_(Node* node, int x)
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
    }

    // для AVL, в отличие от BST, нужно дополнительно отбалансировать
    if (node) node = rebalance_(node);
    return node;
}

AVL_V2::Node* AVL_V2::search_(Node* node, int x)
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

void AVL_V2::update_height_(Node* node)
{
    if (!node) return;
    node->height = 1 + std::max(height_(node->left), height_(node->right));
}

AVL_V2::Node* AVL_V2::rebalance_(Node* node)
{
    if (!node) return nullptr;

    update_height_(node);

    int balance = balance_(node);
    if (balance > 1) {
        // левое поддерево тяжелее.
        // проверяем баланс левого поддерева
        if (balance_(node->left) < 0) {
            // правое плечо левого поддерева тяжелее
            return big_right_rot_(node);
        } else {
            // левое плечо левого поддерева чуть тяжелее или равно
            return small_right_rot_(node);
        }
    } else
    if (balance < -1) {
        // правое поддерево тяжелее.
        // проверяем баланс правого поддерева
        if (balance_(node->right) > 0) {
            // левое плечо правого поддерева тяжелее
            return big_left_rot_(node);
        } else {
            // правое плечо правого поддерева чуть тяжелее или равно
            return small_left_rot_(node);
        }
    }
    return node;
}

AVL_V2::Node* AVL_V2::small_right_rot_(Node* node)
{
    /*
     * выполняется когда
     *      h_left(A) - h_right(A) = 2;
     *      h_left(B) - h_right(B) = 1 (или 0)
     * вес (высота) слева (B) от узла A сильно больше чем справа (AR) от узла А
     * и при этом для узла B левая (BL) и правая (BR) ветви сбалансированы
     * но левая ветвь чуть тяжелее.
     * вытаскиваем B выше A. BR делаем левым потомком A
     * BL остается всё также меньше A, меньше B (свойство выполняется)
     * AR остается всё также больше А, больше B (свойство выполняется)
     * BR остается всё также меньше A, больше B (свойство выполняется)
     *      A                B
     *     / \              / \ 
     *    B   AR    ==>   BL   A
     *   / \                  / \ 
     * BL   BR              BR   AR
     */
    Node* A  = node;
    Node* B  = A->left;
    Node* BR = B->right;

    B->right = A;
    A->left  = BR;

    update_height_(A);
    update_height_(B);
    return B;
}

AVL_V2::Node* AVL_V2::small_left_rot_(Node* node)
{
    /*
     * выполняется когда
     *      h_left(A) - h_right(A) = -2;
     *      h_left(B) - h_right(B) = -1 (или 0)
     * вес (высота) справа (B) от узла A сильно больше чем слева (AL) от узла А
     * и при этом для узла B левая (BL) и правая (BR) ветви сбалансированы
     * но правая ветвь чуть тяжелее.
     * вытаскиваем B выше A. BL делаем правым потомком A
     * AL остается всё также меньше A, меньше B (свойство выполняется)
     * BR остается всё также больше A, больше B (свойство выполняется)
     * BL остается всё также больше A, меньше B (свойство выполняется)
     *     A                B
     *    / \              / \ 
     *  AL   B     ==>    A   BR
     *      / \          / \ 
     *     BL  BR       AL  BL
     */
    Node* A  = node;
    Node* B  = A->right;
    Node* BL = B->left;

    B->left  = A;
    A->right = BL;

    update_height_(A);
    update_height_(B);
    return B;
}

AVL_V2::Node* AVL_V2::big_right_rot_(Node* node)
{
    /*
     * выполняется когда
     *      h_left(A) - h_right(A) = 2;
     *      h_left(B) - h_right(B) = -1
     * вес (высота) слева (B) от узла A сильно больше чем справа (AR) от узла А
     * и при этом для узла B левая (BL) и правая (C) ветви сбалансированы
     * но правая ветвь чуть тяжелее. тут нельзя просто вытащить узел B выше А,
     * подвязав C слева от A, потому что C и так тяжелее, она и останется тяжелее
     * висеть в том же уровне.
     * поэтому вытаскиваем С выше B. CL делаем правым потомком B (малый левый)
     * а затем вытаскиваем C выше A. CR делаем левым потомком A (малый правый)
     *      A                   A                 C
     *     / \                 / \              /   \ 
     *    B   AR    ==>       C   AR   ==>     B     A
     *   / \                 / \              / \   / \ 
     * BL   С               B   CR          BL  CL CR  AR
     *     / \             / \ 
     *   CL   CR         BL   CL
     */
    // в версии AVL_V2 реализуем через прямые присваивания
    Node* A  = node;
    Node* B  = A->left;
    Node* C  = B->right;
    Node* CL = C->left;
    Node* CR = C->right;

    C->left  = B;
    C->right = A;
    B->right = CL;
    A->left  = CR;

    update_height_(A);
    update_height_(B);
    update_height_(C);
    return C;
}

AVL_V2::Node* AVL_V2::big_left_rot_(Node* node)
{
    /*
     * выполняется когда
     *      h_left(A) - h_right(A) = -2;
     *      h_left(B) - h_right(B) = 1
     * вес (высота) справа (B) от узла A сильно больше чем слева (AL) от узла А
     * и при этом для узла B левая (C) и правая (BR) ветви сбалансированы
     * но левая ветвь чуть тяжелее. тут нельзя просто вытащить узел B выше А,
     * подвязав C справа от A, потому что C и так тяжелее, она и останется тяжелее
     * висеть в том же уровне.
     * поэтому вытаскиваем С выше B. CR делаем левым потомком B (малый правый)
     * а затем вытаскиваем C выше A. CL делаем правым потомком A (малый левый)
     *     A                A                     С
     *    / \              / \                  /   \ 
     *  AL   B     ==>   AL   C       ==>     A      B
     *      / \              / \             / \    / \ 
     *     C   BR           CL  B          AL  CL  CR  BR
     *    / \                  / \ 
     *  CL   CR              CR   BR
     */

    // в версии AVL_V1 реализуем через операции малых поворотов
    Node* A  = node;
    Node* B  = A->right;
    Node* C  = B->left;
    Node* CL = C->left;
    Node* CR = C->right;

    C->left  = A;
    C->right = B;
    A->right = CL;
    B->left  = CR;

    update_height_(A);
    update_height_(B);
    update_height_(C);
    return C;
}
