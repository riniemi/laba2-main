// Хеш-таблица с открытой адресацией.
#pragma once
#include <string>

#include "bloom.h"
#include "massiv.h"

enum Method { LINEAR, DOUBLE };

struct HashTable {
    Method method;
    Array slots;
    int cap;
    int count;
    int tombs;
    bool autoGrow;
    bool useBloom;
    BloomFilter bloom;

    int home(const std::string& key) const;
    int stepOf(const std::string& key) const;
    int findIndex(const std::string& key);
    void insertFresh(const std::string& key, const std::string& value);
    void rehash(int newCap);
    void init(Method m, int capacity);

public:
    int probes;
    int bloomRejects;
    int bloomFalse;

    HashTable(Method m, int capacity = 11, bool grow = true, bool bloomOn = true);

    bool put(const std::string& key, const std::string& value);
    bool get(const std::string& key, std::string& value);
    bool contains(const std::string& key);
    bool erase(const std::string& key);
    int size() const { return count; }
    int capacity() const { return cap; }
    Method methodOf() const { return method; }
    void resetCounters() { probes = 0; bloomRejects = 0; bloomFalse = 0; }

    Strs dump() const;
    void load(const Strs& t);
    std::string show() const;
    std::string stats() const;
};
