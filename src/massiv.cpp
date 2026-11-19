#include "massiv.h"

using namespace std;

void Array::check(int i) const {
    if (i < 0 || i >= len) fail("index out of range: " + to_string(i));
}

void Array::insert(int idx, const string& v) {
    if (idx > len) fail("index out of range: " + to_string(idx));
    if (len == cap) {
        cap = cap * 2;
        string* nd = new string[cap];
        for (int i = 0; i < len; i = i + 1) nd[i] = data[i];
        delete[] data;
        data = nd;
    }
    for (int i = len; i > idx; i = i - 1) data[i] = data[i - 1];
    data[idx] = v;
    len = len + 1;
}

void Array::push(const string& v) { insert(len, v); }

const string& Array::get(int i) const { check(i); return data[i]; }

void Array::set(int i, const string& v) { check(i); data[i] = v; }

string Array::erase(int i) {
    check(i);
    string r = data[i];
    for (int j = i; j + 1 < len; j = j + 1) data[j] = data[j + 1];
    len = len - 1;
    return r;
}

Strs Array::dump() const {
    Strs r;
    for (int i = 0; i < len; i = i + 1) r.push_back(data[i]);
    return r;
}

void Array::load(const Strs& t) { for (int i = 0; i < (int)t.size(); i = i + 1) push(t[i]); }

string Array::show() const {
    return "[" + join(dump(), ", ") + "] (len=" + to_string(len) + ")";
}
