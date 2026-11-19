#include "tree.h"

#include <algorithm>

using namespace std;

void CBTree::destroy(TNode* x) {
    if (x) { destroy(x->left); destroy(x->right); delete x; }
}

vector<TNode*> CBTree::nodes() const {
    vector<TNode*> o;
    if (root) o.push_back(root);
    for (int i = 0; i < (int)o.size(); i = i + 1) {
        if (o[i]->left) o.push_back(o[i]->left);
        if (o[i]->right) o.push_back(o[i]->right);
    }
    return o;
}

void CBTree::preorder(TNode* x, Strs& o) const {
    if (!x) return;
    o.push_back(x->key);
    preorder(x->left, o);
    preorder(x->right, o);
}

void CBTree::inorder(TNode* x, Strs& o) const {
    if (!x) return;
    inorder(x->left, o);
    o.push_back(x->key);
    inorder(x->right, o);
}

void CBTree::postorder(TNode* x, Strs& o) const {
    if (!x) return;
    postorder(x->left, o);
    postorder(x->right, o);
    o.push_back(x->key);
}

void CBTree::dumpRec(TNode* x, Strs& o) const {
    o.push_back(x ? x->key : "#");
    if (x) { dumpRec(x->left, o); dumpRec(x->right, o); }
}

TNode* CBTree::build(const Strs& t, int& i) {
    if (i >= (int)t.size()) return nullptr;
    string key = t[i];
    i = i + 1;
    if (key == "#") return nullptr;
    TNode* x = new TNode{normInt(key), nullptr, nullptr};
    x->left = build(t, i);
    x->right = build(t, i);
    return x;
}

string CBTree::insert(const string& k) {
    TNode* z = new TNode{normInt(k), nullptr, nullptr};
    string result = z->key;
    if (!root) { root = z; return result; }
    for (TNode* x : nodes()) {
        if (!x->left) { x->left = z; return result; }
        if (!x->right) { x->right = z; return result; }
    }
    return result;
}

bool CBTree::contains(const string& k) const {
    string key = normInt(k);
    for (TNode* x : nodes()) if (x->key == key) return true;
    return false;
}

bool CBTree::isComplete() const {
    vector<TNode*> o;
    if (root) o.push_back(root);
    bool gap = false;
    for (int i = 0; i < (int)o.size(); i = i + 1) {
        if (!o[i]) { gap = true; continue; }
        if (gap) return false;
        o.push_back(o[i]->left);
        o.push_back(o[i]->right);
    }
    return true;
}

Strs CBTree::traverse(const string& order) const {
    Strs o;
    if (order == "PREORDER") preorder(root, o);
    else if (order == "INORDER") inorder(root, o);
    else if (order == "POSTORDER") postorder(root, o);
    else if (order == "SORTED") {
        for (TNode* x : nodes()) o.push_back(x->key);
        sort(o.begin(), o.end());
    }
    else if (order == "BFS") for (TNode* x : nodes()) o.push_back(x->key);
    else fail("unknown order: " + order);
    return o;
}

Strs CBTree::dump() const {
    Strs o;
    dumpRec(root, o);
    return o;
}

void CBTree::load(const Strs& t) {
    destroy(root);
    root = nullptr;
    int i = 0;
    root = build(t, i);
}

void CBTree::showRec(TNode* x, int depth, string& out) const {
    if (!x) return;
    showRec(x->right, depth + 1, out);
    for (int i = 0; i < depth; i = i + 1) out = out + "  ";
    out = out + x->key + "\n";
    showRec(x->left, depth + 1, out);
}

string CBTree::show() const {
    if (!root) return "(empty tree)";
    string out;
    showRec(root, 0, out);
    if (!out.empty() && out.back() == '\n') out.pop_back();
    return out;
}
