// LRU-кэш фиксированного размера.
#pragma once
#include <string>
#include <vector>

#include "dlist.h"
#include "hashtable.h"

struct LRUCache {
    int capacity;
    DList order;
    HashTable index;
    std::vector<DNode*> nodes;
    std::vector<int> freeIds;

    int allocId(DNode* node);

public:
    explicit LRUCache(int cap);

    void set(const std::string& key, const std::string& value);
    bool get(const std::string& key, std::string& value);
    int size() const { return index.size(); }
    std::string show();
};
