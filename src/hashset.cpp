#include "hashset.h"

#include <algorithm>

using namespace std;

int HashSet::hashOf(const string& v) {
    int h = 5381;
    for (int i = 0; i < (int)v.size(); i = i + 1) h = h * 33 + (unsigned char)v[i];
    return h;
}

void HashSet::grow() {
    vector<SList*> bigger;
    for (int i = 0; i < (int)buckets.size() * 2; i = i + 1) bigger.push_back(new SList());
    for (int i = 0; i < (int)buckets.size(); i = i + 1) {
        SList* b = buckets[i];
        for (const string& s : b->items())
            bigger[hashOf(s) % (int)bigger.size()]->pushHead(s);
        delete b;
    }
    buckets = bigger;
}

bool HashSet::contains(const string& v) const {
    return buckets[hashOf(v) % (int)buckets.size()]->find(v) >= 0;
}

bool HashSet::add(const string& v) {
    if (contains(v)) return false;
    if (count >= (int)buckets.size()) grow();
    buckets[hashOf(v) % (int)buckets.size()]->pushHead(v);
    count = count + 1;
    return true;
}

bool HashSet::remove(const string& v) {
    bool removed = buckets[hashOf(v) % (int)buckets.size()]->delValue(v);
    if (removed) count = count - 1;
    return removed;
}

Strs HashSet::items() const {
    Strs r;
    for (int i = 0; i < (int)buckets.size(); i = i + 1)
        for (const string& s : buckets[i]->items()) r.push_back(s);
    sort(r.begin(), r.end());
    return r;
}

void HashSet::load(const Strs& t) { for (int i = 0; i < (int)t.size(); i = i + 1) add(t[i]); }

string HashSet::show() const {
    Strs v = items();
    Strs q;
    for (int i = 0; i < (int)v.size(); i = i + 1) q.push_back(quote(v[i]));
    return "{" + join(q, ", ") + "}";
}
