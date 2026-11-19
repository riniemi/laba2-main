#include "bst.h"

#include <vector>

using namespace std;

BST::~BST() {
    vector<TNode*> todo;
    if (root) todo.push_back(root);
    while (!todo.empty()) {
        TNode* x = todo.back();
        todo.pop_back();
        if (x->left) todo.push_back(x->left);
        if (x->right) todo.push_back(x->right);
        delete x;
    }
}

static int cmpInt(const string& a, const string& b) {
    bool na = a[0] == '-', nb = b[0] == '-';
    if (na != nb) return na ? -1 : 1;
    int r;
    if ((int)a.size() != (int)b.size()) r = (int)a.size() < (int)b.size() ? -1 : 1;
    else r = a == b ? 0 : (a < b ? -1 : 1);
    return na ? -r : r;
}

int BST::insert(int v) {
    string key = to_string(v);
    if (!root) {
        root = new TNode{key, nullptr, nullptr};
        return 1;
    }
    int depth = 1;
    TNode* x = root;
    for (;;) {
        int c = cmpInt(key, x->key);
        if (c == 0) return 0;
        TNode*& next = c < 0 ? x->left : x->right;
        depth = depth + 1;
        if (!next) {
            next = new TNode{key, nullptr, nullptr};
            return depth;
        }
        x = next;
    }
}
