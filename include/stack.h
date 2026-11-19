// Стек на основе списка.
#pragma once
#include "slist.h"

struct Stack {
private:
    SList l;
public:
    void push(const std::string& v);
    std::string pop();
    Strs items() const;
    bool empty() const { return l.size() == 0; }
    std::string top() const { return l.at(0); }
    Strs dump() const;
    void load(const Strs& t);
    std::string show() const;
};
