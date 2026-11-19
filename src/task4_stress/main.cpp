// Задача 4 (ударения). Подсчёт слов с ошибкой в ударении.
#include <iostream>
#include <sstream>

#include "hashset.h"
#include "io.h"
#include "massiv.h"

using namespace std;

static string lower(string s) {
    for (int i = 0; i < (int)s.size(); i = i + 1)
        if (s[i] >= 'A' && s[i] <= 'Z') s[i] = s[i] - 'A' + 'a';
    return s;
}

static int upperCount(const string& s) {
    int n = 0;
    for (int i = 0; i < (int)s.size(); i = i + 1)
        if (s[i] >= 'A' && s[i] <= 'Z') n = n + 1;
    return n;
}

int main(int argc, char** argv) {
    istringstream in(readInput(argc, argv));
    int n;
    if (!(in >> n) || n < 0) {
        cerr << "ERROR: в первой строке должно быть число слов словаря N" << endl;
        return 1;
    }

    Array dict;
    for (int i = 0; i < n; i = i + 1) {
        string w;
        if (!(in >> w)) { cerr << "ERROR: в словаре меньше слов, чем N" << endl; return 1; }
        dict.push(w);
    }
    Array text;
    for (string w; in >> w;) text.push(w);

    HashSet variants, words;
    for (int i = 0; i < dict.size(); i = i + 1) {
        variants.add(dict.get(i));
        words.add(lower(dict.get(i)));
    }

    int errors = 0;
    for (int i = 0; i < text.size(); i = i + 1) {
        const string& w = text.get(i);
        if (words.contains(lower(w))) {
            if (!variants.contains(w)) errors = errors + 1;
        } else if (upperCount(w) != 1) {
            errors = errors + 1;
        }
    }
    cout << errors << endl;
}
