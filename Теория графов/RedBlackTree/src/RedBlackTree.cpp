#include "RedBlackTree.h"
#include <vector>
#include <iostream>

// Конструктор
RedBlackTree::RedBlackTree() {
    NIL = new Node(0);
    NIL->color = BLACK;
    NIL->left = NIL->right = NIL->parent = NIL;
    root = NIL;
}

// Деструктор
RedBlackTree::~RedBlackTree() {
    clear();
    delete NIL;
}

// Левый поворот
void RedBlackTree::leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;

    if (y->left != NIL) {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == NIL) {
        root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

// Правый поворот
void RedBlackTree::rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;

    if (y->right != NIL) {
        y->right->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == NIL) {
        root = y;
    }
    else if (x == x->parent->right) {
        x->parent->right = y;
    }
    else {
        x->parent->left = y;
    }

    y->right = x;
    x->parent = y;
}

// Исправление дерева после вставки
void RedBlackTree::fixInsert(Node* k) {
    Node* u;

    while (k->parent->color == RED) {
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left;

            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            }
            else {
                if (k == k->parent->left) {
                    k = k->parent;
                    rightRotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                leftRotate(k->parent->parent);
            }
        }
        else {
            u = k->parent->parent->right;

            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            }
            else {
                if (k == k->parent->right) {
                    k = k->parent;
                    leftRotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rightRotate(k->parent->parent);
            }
        }

        if (k == root) {
            break;
        }
    }

    root->color = BLACK;
}

// Вставка элемента
void RedBlackTree::insert(int value) {
    Node* node = new Node(value);
    node->parent = NIL;
    node->left = NIL;
    node->right = NIL;
    node->color = RED;

    Node* y = NIL;
    Node* x = root;

    while (x != NIL) {
        y = x;
        if (node->value < x->value) {
            x = x->left;
        }
        else if (node->value > x->value) {
            x = x->right;
        }
        else {
            // Элемент уже существует
            delete node;
            return;
        }
    }

    node->parent = y;

    if (y == NIL) {
        root = node;
    }
    else if (node->value < y->value) {
        y->left = node;
    }
    else {
        y->right = node;
    }

    if (node->parent == NIL) {
        node->color = BLACK;
        return;
    }

    if (node->parent->parent == NIL) {
        return;
    }

    fixInsert(node);
}

// Нахождение минимального элемента в поддереве
Node* RedBlackTree::minimum(Node* node) {
    while (node->left != NIL) {
        node = node->left;
    }
    return node;
}

// Нахождение максимального элемента в поддереве
Node* RedBlackTree::maximum(Node* node) {
    while (node->right != NIL) {
        node = node->right;
    }
    return node;
}

// Замена одного поддерева другим
void RedBlackTree::transplant(Node* u, Node* v) {
    if (u->parent == NIL) {
        root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

// Исправление дерева после удаления
void RedBlackTree::fixDelete(Node* x) {
    Node* s;

    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            s = x->parent->right;

            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                s = x->parent->right;
            }

            if (s->left->color == BLACK && s->right->color == BLACK) {
                s->color = RED;
                x = x->parent;
            }
            else {
                if (s->right->color == BLACK) {
                    s->left->color = BLACK;
                    s->color = RED;
                    rightRotate(s);
                    s = x->parent->right;
                }

                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        }
        else {
            s = x->parent->left;

            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                s = x->parent->left;
            }

            if (s->right->color == BLACK && s->left->color == BLACK) {
                s->color = RED;
                x = x->parent;
            }
            else {
                if (s->left->color == BLACK) {
                    s->right->color = BLACK;
                    s->color = RED;
                    leftRotate(s);
                    s = x->parent->left;
                }

                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }

    x->color = BLACK;
}

// Удаление узла (вспомогательная функция)
void RedBlackTree::deleteNode(Node* node, int value) {
    Node* z = NIL;
    Node* x;
    Node* y;

    while (node != NIL) {
        if (node->value == value) {
            z = node;
        }

        if (node->value <= value) {
            node = node->right;
        }
        else {
            node = node->left;
        }
    }

    if (z == NIL) {
        return;
    }

    y = z;
    Color yOriginalColor = y->color;

    if (z->left == NIL) {
        x = z->right;
        transplant(z, z->right);
    }
    else if (z->right == NIL) {
        x = z->left;
        transplant(z, z->left);
    }
    else {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        }
        else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;

    if (yOriginalColor == BLACK) {
        fixDelete(x);
    }
}

// Удаление элемента
void RedBlackTree::remove(int value) {
    deleteNode(root, value);
}

// Поиск элемента (вспомогательная функция)
Node* RedBlackTree::searchHelper(Node* node, int value) {
    if (node == NIL || value == node->value) {
        return node;
    }

    if (value < node->value) {
        return searchHelper(node->left, value);
    }

    return searchHelper(node->right, value);
}

// Поиск элемента
bool RedBlackTree::search(int value) {
    Node* result = searchHelper(root, value);
    return result != NIL;
}

// Обход в порядке возрастания (вспомогательная функция)
void RedBlackTree::inOrderHelper(Node* node, std::vector<int>& result) {
    if (node != NIL) {
        inOrderHelper(node->left, result);
        result.push_back(node->value);
        inOrderHelper(node->right, result);
    }
}

// Нахождение минимального элемента
int RedBlackTree::findMin() {
    if (root == NIL) {
        return std::numeric_limits<int>::min();
    }
    return minimum(root)->value;
}

// Нахождение максимального элемента
int RedBlackTree::findMax() {
    if (root == NIL) {
        return std::numeric_limits<int>::max();
    }
    return maximum(root)->value;
}

// Нахождение k минимальных элементов
std::vector<int> RedBlackTree::findMinK(int k) {
    std::vector<int> result;
    if (root == NIL || k <= 0) {
        return result;
    }

    std::vector<int> allElements;
    inOrderHelper(root, allElements);

    k = std::min(k, (int)allElements.size());
    result.assign(allElements.begin(), allElements.begin() + k);

    return result;
}

// Нахождение k максимальных элементов
std::vector<int> RedBlackTree::findMaxK(int k) {
    std::vector<int> result;
    if (root == NIL || k <= 0) {
        return result;
    }

    std::vector<int> allElements;
    inOrderHelper(root, allElements);

    k = std::min(k, (int)allElements.size());
    result.assign(allElements.end() - k, allElements.end());

    return result;
}

// Вывод элементов в порядке возрастания
void RedBlackTree::printInOrder() {
    std::vector<int> elements;
    inOrderHelper(root, elements);

    if (elements.empty()) {
        std::cout << "Tree is empty.\n";
        return;
    }

    std::cout << "Tree elements (in order): ";
    for (size_t i = 0; i < elements.size(); ++i) {
        std::cout << elements[i];
        if (i != elements.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

// Очистка дерева (вспомогательная функция)
void RedBlackTree::clearHelper(Node* node) {
    if (node != NIL) {
        clearHelper(node->left);
        clearHelper(node->right);
        delete node;
    }
}

// Очистка дерева
void RedBlackTree::clear() {
    clearHelper(root);
    root = NIL;
}