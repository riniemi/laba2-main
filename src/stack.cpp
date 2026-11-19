#include "stack.h"

using namespace std;

void Stack::push(const string& v) { l.pushHead(v); }

string Stack::pop() {
    if (l.size() == 0) fail("stack is empty");
    return l.popHead();
}

Strs Stack::items() const { return l.items(); }

Strs Stack::dump() const {
    Strs v = l.items();
    Strs r;
    for (int i = (int)v.size() - 1; i >= 0; i = i - 1) r.push_back(v[i]);
    return r;
}

void Stack::load(const Strs& t) { for (int i = 0; i < (int)t.size(); i = i + 1) push(t[i]); }

string Stack::show() const {
    Strs v = l.items();
    if (v.empty()) return "(empty stack)";
    string r;
    for (int i = 0; i < (int)v.size(); i = i + 1) {
        r = r + "| " + v[i] + " |";
        if (i == 0) r = r + "  <- top";
        r = r + "\n";
    }
    return r + "+---+";
}
