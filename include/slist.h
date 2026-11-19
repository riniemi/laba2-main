// Односвязный список строк.
#pragma once
#include "common.h"

struct SNode {
    std::string val;
    SNode* next;
};

struct SList {
    SNode* head;
    SNode* tail;
    int n;

    SList() {
        head = nullptr;
        tail = nullptr;
        n = 0;
    }

    ~SList();

    int size() const {
        return n;
    }

    void pushHead(const std::string& v);
    void pushTail(const std::string& v);
    void pushAfter(const std::string& target, const std::string& v);
    void pushBefore(const std::string& target, const std::string& v);
    std::string popHead();
    std::string popTail();
    std::string delAfter(const std::string& target);
    std::string delBefore(const std::string& target);
    bool delValue(const std::string& v);
    int find(const std::string& v) const;
    const std::string& at(int i) const;
    Strs items() const;
    Strs dump() const;
    void load(const Strs& t);
    std::string show() const;

private:
    SNode* findNode(const std::string& v) const;
    SNode* mustFind(const std::string& v) const;
};
