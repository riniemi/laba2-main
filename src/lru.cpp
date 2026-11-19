#include "lru.h"

using namespace std;

static const char SEP = '\x1F';

LRUCache::LRUCache(int cap) : capacity(cap), index(DOUBLE, 11, true, true) {}

int LRUCache::allocId(DNode* node) {
    if (!freeIds.empty()) {
        int id = freeIds.back();
        freeIds.pop_back();
        nodes[id] = node;
        return id;
    }
    nodes.push_back(node);
    return (int)nodes.size() - 1;
}

void LRUCache::set(const string& key, const string& value) {
    if (capacity == 0) return;
    string entry;
    if (index.get(key, entry)) {
        int sep = (int)entry.find(SEP);
        int id = atoi(entry.substr(0, sep).c_str());
        index.put(key, to_string(id) + SEP + value);
        order.moveToHead(nodes[id]);
        return;
    }
    if (index.size() == capacity) {
        string oldKey = order.tailNode()->val;
        string e;
        index.get(oldKey, e);
        int sep = (int)e.find(SEP);
        freeIds.push_back(atoi(e.substr(0, sep).c_str()));
        index.erase(oldKey);
        order.popTail();
    }
    DNode* node = order.pushHeadNode(key);
    index.put(key, to_string(allocId(node)) + SEP + value);
}

bool LRUCache::get(const string& key, string& value) {
    string entry;
    if (!index.get(key, entry)) return false;
    int sep = (int)entry.find(SEP);
    order.moveToHead(nodes[atoi(entry.substr(0, sep).c_str())]);
    value = entry.substr(sep + 1);
    return true;
}

string LRUCache::show() {
    Strs parts;
    for (const string& key : order.itemsReversed()) {
        string e;
        index.get(key, e);
        int sep = (int)e.find(SEP);
        parts.push_back(key + " -> " + e.substr(sep + 1));
    }
    if (parts.empty()) return "(empty)";
    return join(parts, ", ");
}
