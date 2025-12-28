#pragma once

#include <vector>

enum Color { RED, BLACK };

// Структура узла красно-черного дерева
struct Node {
    int value;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    Node(int val) : value(val), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

// Класс красно-черного дерева
class RedBlackTree {
private:
    Node* root;
    Node* NIL; // Специальный NIL узел

    // Вспомогательные функции
    void leftRotate(Node* x);
    void rightRotate(Node* x);
    void fixInsert(Node* k);
    void fixDelete(Node* x);
    void transplant(Node* u, Node* v);
    Node* minimum(Node* node);
    Node* maximum(Node* node);
    void deleteNode(Node* node, int value);
    Node* searchHelper(Node* node, int value);
    void inOrderHelper(Node* node, std::vector<int>& result);
    void clearHelper(Node* node);

public:
    RedBlackTree();
    ~RedBlackTree();

    void insert(int value);
    void remove(int value);
    bool search(int value);
    int findMin();
    int findMax();
    std::vector<int> findMinK(int k);
    std::vector<int> findMaxK(int k);
    void printInOrder();
    void clear();
    bool isEmpty() const { return root == NIL; }
};