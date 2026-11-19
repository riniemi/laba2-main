// Двусвязный список (L) строк.
// Используется там, где нужны быстрые вставки и удаления в любом месте (например, в LRU-кэше).
#pragma once
#include "common.h"

// узел списка: значение и указатели на предыдущий/следующий узлы
struct DNode { std::string val; DNode *prev, *next; };

struct DList {
private:
    DNode *head = nullptr, *tail = nullptr;      // первый и последний узлы; nullptr при пустом списке
    size_t n = 0;                                // число элементов
    // найти узел по значению; nullptr, если нет, O(n)
    DNode* findNode(const std::string& v) const;
    // найти узел или выбросить ошибку, O(n)
    DNode* mustFind(const std::string& v) const;
    // отвязать и удалить узел, вернуть его значение, O(1)
    std::string unlink(DNode* x);
    // вставить новый узел после t, O(1)
    void insertAfter(DNode* t, const std::string& v);
public:
    DList() {}                                   // создает пустой список
    // копирование и присваивание запрещены: объект владеет узлами
    DList(const DList&) = delete;
    DList& operator=(const DList&) = delete;
    ~DList();                                    // освобождает все узлы

    void pushHead(const std::string& v);         // добавить в голову, O(1)
    void pushTail(const std::string& v);         // добавить в хвост, O(1)
    // вставить после узла со значением target; ошибка, если его нет, O(n)
    void pushAfter(const std::string& target, const std::string& v);
    // вставить перед узлом со значением target; ошибка, если его нет, O(n)
    void pushBefore(const std::string& target, const std::string& v);
    std::string popHead();                       // удалить и вернуть голову; ошибка при пустом списке, O(1)
    std::string popTail();                       // удалить и вернуть хвост; ошибка при пустом списке, O(1)
    // удалить узел после target и вернуть значение; ошибка, если его нет, O(n)
    std::string delAfter(const std::string& target);
    // удалить узел перед target и вернуть значение; ошибка, если его нет, O(n)
    std::string delBefore(const std::string& target);
    bool delValue(const std::string& v);         // удалить первый узел со значением v, O(n)
    int find(const std::string& v) const;        // индекс первого узла со значением v или -1, O(n)
    const std::string& at(int i) const;       // значение по индексу; ошибка, если вне границ, O(n)
    Strs items() const;                          // все значения от головы к хвосту
    Strs itemsReversed() const;                  // все значения от хвоста к голове
    DNode* pushHeadNode(const std::string& v);   // как pushHead, но возвращает созданный узел
    void moveToHead(DNode* x);                   //  перенести узел в голову, O(1)
    DNode* tailNode() const { return tail; }     // последний узел

    Strs dump() const { return items(); }        // сериализация в массив строк (как items)
    void load(const Strs& t);                    // заполнить список значениями из массива, O(n)
    std::string show() const;                    // текст: NULL <-> a <-> b <-> NULL
};
