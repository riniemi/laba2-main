// Двоичное дерево поиска чисел.
#pragma once
#include "tree.h"

struct BST {
    TNode* root;

    BST() {
        root = nullptr;
    }

    ~BST();

    int insert(int v);
};
