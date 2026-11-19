#include "slist.h"

using namespace std;

SNode* SList::findNode(const string& v) const {
    for (SNode* c = head; c != nullptr; c = c->next) {
        if (c->val == v) return c;
    }
    return nullptr;
}

SNode* SList::mustFind(const string& v) const {
    SNode* t = findNode(v);
    if (t == nullptr) fail("value not found: " + v);
    return t;
}

SList::~SList() {
    while (head != nullptr) popHead();
}

void SList::pushHead(const string& v) {
    head = new SNode{v, head};
    if (tail == nullptr) tail = head;
    n = n + 1;
}

void SList::pushTail(const string& v) {
    SNode* x = new SNode{v, nullptr};
    if (tail != nullptr) tail->next = x;
    else head = x;
    tail = x;
    n = n + 1;
}

void SList::pushAfter(const string& target, const string& v) {
    SNode* t = mustFind(target);
    t->next = new SNode{v, t->next};
    if (t == tail) tail = t->next;
    n = n + 1;
}

void SList::pushBefore(const string& target, const string& v) {
    mustFind(target);
    if (head->val == target) { pushHead(v); return; }
    SNode* p = head;
    while (p->next->val != target) p = p->next;
    p->next = new SNode{v, p->next};
    n = n + 1;
}

string SList::popHead() {
    if (head == nullptr) fail("list is empty");
    SNode* t = head;
    string r = t->val;
    head = t->next;
    if (head == nullptr) tail = nullptr;
    delete t;
    n = n - 1;
    return r;
}

string SList::popTail() {
    if (head == nullptr) fail("list is empty");
    if (head == tail) return popHead();
    SNode* p = head;
    while (p->next != tail) p = p->next;
    string r = tail->val;
    delete tail;
    tail = p;
    p->next = nullptr;
    n = n - 1;
    return r;
}

string SList::delAfter(const string& target) {
    SNode* t = mustFind(target);
    if (t->next == nullptr) fail("no element after " + target);
    SNode* x = t->next;
    string r = x->val;
    t->next = x->next;
    if (x == tail) tail = t;
    delete x;
    n = n - 1;
    return r;
}

string SList::delBefore(const string& target) {
    SNode* t = mustFind(target);
    if (t == head) fail("no element before " + target);
    if (head->next == t) return popHead();
    SNode* p = head;
    while (p->next->next != t) p = p->next;
    SNode* x = p->next;
    string r = x->val;
    p->next = t;
    delete x;
    n = n - 1;
    return r;
}

bool SList::delValue(const string& v) {
    if (findNode(v) == nullptr) return false;
    if (head->val == v) { popHead(); return true; }
    SNode* p = head;
    while (p->next->val != v) p = p->next;
    SNode* x = p->next;
    p->next = x->next;
    if (x == tail) tail = p;
    delete x;
    n = n - 1;
    return true;
}

int SList::find(const string& v) const {
    int i = 0;
    for (SNode* c = head; c != nullptr; c = c->next, i = i + 1)
        if (c->val == v) return i;
    return -1;
}

const string& SList::at(int i) const {
    if (i < 0 || i >= n) fail("index out of range: " + to_string(i));
    SNode* c = head;
    while (i > 0) { c = c->next; i = i - 1; }
    return c->val;
}

Strs SList::items() const {
    Strs r;
    for (SNode* c = head; c != nullptr; c = c->next) r.push_back(c->val);
    return r;
}

void SList::load(const Strs& t) {
    for (int i = 0; i < (int)t.size(); i = i + 1) pushTail(t[i]);
}

string SList::show() const {
    if (head == nullptr) return "NULL";
    return join(items(), " -> ") + " -> NULL";
}

Strs SList::dump() const {
    return items();
}
