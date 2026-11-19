#include "dlist.h"

using namespace std;

DNode* DList::findNode(const string& v) const {
    for (DNode* c = head; c != nullptr; c = c->next)
        if (c->val == v) return c;
    return nullptr;
}

DNode* DList::mustFind(const string& v) const {
    DNode* t = findNode(v);
    if (t == nullptr) fail("value not found: " + v);
    return t;
}

string DList::unlink(DNode* x) {
    string r = x->val;
    if (x->prev != nullptr) x->prev->next = x->next; else head = x->next;
    if (x->next != nullptr) x->next->prev = x->prev; else tail = x->prev;
    delete x;
    n = n - 1;
    return r;
}

void DList::insertAfter(DNode* t, const string& v) {
    DNode* x = new DNode{v, t, t->next};
    if (t->next != nullptr) t->next->prev = x; else tail = x;
    t->next = x;
    n = n + 1;
}

DList::~DList() {
    while (head != nullptr) popHead();
}

void DList::pushHead(const string& v) {
    DNode* x = new DNode{v, nullptr, head};
    if (head != nullptr) head->prev = x; else tail = x;
    head = x;
    n = n + 1;
}

void DList::pushTail(const string& v) {
    DNode* x = new DNode{v, tail, nullptr};
    if (tail != nullptr) tail->next = x; else head = x;
    tail = x;
    n = n + 1;
}

void DList::pushAfter(const string& target, const string& v) { insertAfter(mustFind(target), v); }

void DList::pushBefore(const string& target, const string& v) {
    DNode* t = mustFind(target);
    if (t == head) { pushHead(v); return; }
    insertAfter(t->prev, v);
}

string DList::popHead() {
    if (head == nullptr) fail("list is empty");
    return unlink(head);
}

string DList::popTail() {
    if (head == nullptr) fail("list is empty");
    return unlink(tail);
}

string DList::delAfter(const string& target) {
    DNode* t = mustFind(target);
    if (t->next == nullptr) fail("no element after " + target);
    return unlink(t->next);
}

string DList::delBefore(const string& target) {
    DNode* t = mustFind(target);
    if (t->prev == nullptr) fail("no element before " + target);
    return unlink(t->prev);
}

bool DList::delValue(const string& v) {
    DNode* x = findNode(v);
    if (x != nullptr) unlink(x);
    return x != nullptr;
}

int DList::find(const string& v) const {
    int i = 0;
    for (DNode* c = head; c != nullptr; c = c->next, i = i + 1)
        if (c->val == v) return i;
    return -1;
}

const string& DList::at(int i) const {
    if (i < 0 || i >= n) fail("index out of range: " + to_string(i));
    DNode* c = head;
    while (i > 0) { c = c->next; i = i - 1; }
    return c->val;
}

Strs DList::items() const {
    Strs r;
    for (DNode* c = head; c != nullptr; c = c->next) r.push_back(c->val);
    return r;
}

Strs DList::itemsReversed() const {
    Strs r;
    for (DNode* c = tail; c != nullptr; c = c->prev) r.push_back(c->val);
    return r;
}

DNode* DList::pushHeadNode(const string& v) {
    pushHead(v);
    return head;
}

void DList::moveToHead(DNode* x) {
    if (x == head) return;
    x->prev->next = x->next;
    if (x->next != nullptr) x->next->prev = x->prev; else tail = x->prev;
    x->prev = nullptr;
    x->next = head;
    head->prev = x;
    head = x;
}

void DList::load(const Strs& t) { for (int i = 0; i < (int)t.size(); i = i + 1) pushTail(t[i]); }

string DList::show() const {
    if (head == nullptr) return "NULL";
    return "NULL <-> " + join(items(), " <-> ") + " <-> NULL";
}
