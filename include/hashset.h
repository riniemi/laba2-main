// Множество строк на хеш-таблице с корзинами-списками.
#pragma once
#include <vector>

#include "common.h"
#include "slist.h"

struct HashSet {
    std::vector<SList*> buckets;
    int count;

    HashSet() {
        count = 0;
        for (int i = 0; i < 8; i = i + 1) buckets.push_back(new SList());
    }

    ~HashSet() {
        for (int i = 0; i < (int)buckets.size(); i = i + 1) delete buckets[i];
    }

    static int hashOf(const std::string& v);
    void grow();

public:
    bool add(const std::string& v);
    bool remove(const std::string& v);
    bool contains(const std::string& v) const;
    int size() const { return count; }
    Strs items() const;

    Strs dump() const { return items(); }
    void load(const Strs& t);
    std::string show() const;
};
