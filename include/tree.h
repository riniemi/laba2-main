// Двоичное дерево, хранящее числа.
#pragma once
#include <vector>
#include "common.h"

struct TNode {
    std::string key;
    TNode* left;
    TNode* right;
};

struct CBTree {
    TNode* root;

    CBTree() {
        root = nullptr;
    }

    ~CBTree() {
        destroy(root);
    }

    std::string insert(const std::string& k);
    bool contains(const std::string& k) const;
    bool isComplete() const;
    Strs traverse(const std::string& order) const;
    Strs dump() const;
    void load(const Strs& t);
    std::string show() const;

private:
    static void destroy(TNode* x);
    std::vector<TNode*> nodes() const;
    void preorder(TNode* x, Strs& o) const;
    void inorder(TNode* x, Strs& o) const;
    void postorder(TNode* x, Strs& o) const;
    void dumpRec(TNode* x, Strs& o) const;
    TNode* build(const Strs& t, int& i);
    void showRec(TNode* x, int depth, std::string& out) const;
};
