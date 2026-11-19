// Динамический массив строк.
#pragma once
#include "common.h"

struct Array {
    std::string* data;
    int len;
    int cap;

    Array() {
        data = new std::string[4];
        len = 0;
        cap = 4;
    }

    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;

    ~Array() { delete[] data; }

    int size() const { return len; }
    void insert(int idx, const std::string& v);
    void push(const std::string& v);
    const std::string& get(int i) const;
    void set(int i, const std::string& v);
    std::string erase(int i);

    Strs dump() const;
    void load(const Strs& t);
    std::string show() const;

private:
    void check(int i) const;
};
