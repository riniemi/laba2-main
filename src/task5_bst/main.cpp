// Задача 5 (двоичное дерево поиска). Вставка чисел и вывод уровня.
#include <iostream>
#include <sstream>

#include "bst.h"
#include "io.h"

using namespace std;

int main(int argc, char** argv) {
    istringstream in(readInput(argc, argv));
    BST tree;
    bool first = true;
    int v;
    while (in >> v && v != 0) {
        int depth = tree.insert(v);
        if (depth == 0) continue;
        if (!first) cout << ' ';
        cout << depth;
        first = false;
    }
    cout << endl;
}
