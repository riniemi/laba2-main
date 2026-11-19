// Задача 7 (LRU-кэш). Обработка запросов SET/GET к кэшу.
#include <iostream>
#include <sstream>

#include "io.h"
#include "lru.h"

using namespace std;

static string lower(string s) {
    for (int i = 0; i < (int)s.size(); i = i + 1)
        if (s[i] >= 'A' && s[i] <= 'Z') s[i] = s[i] - 'A' + 'a';
    return s;
}

int main(int argc, char** argv) {
    bool trace = false;
    for (int i = 1; i < argc; i = i + 1) if (string(argv[i]) == "--trace") trace = true;

    string text = readInput(argc, argv);
    for (int i = 0; i < (int)text.size(); i = i + 1)
        if (text[i] == ',' || text[i] == '=') text[i] = ' ';
    istringstream in(text);
    Strs t;
    for (string w; in >> w;) t.push_back(w);

    int i = 0;
    auto skipWord = [&](const string& name) {
        if (i < (int)t.size() && lower(t[i]) == name) i = i + 1;
    };
    auto number = [&]() -> int {
        if (i >= (int)t.size() || t[i].empty() || t[i].find_first_not_of("0123456789") != string::npos) {
            cerr << "ERROR: ожидалось неотрицательное число" << endl;
            exit(1);
        }
        int v = atoi(t[i].c_str());
        i = i + 1;
        return v;
    };
    skipWord("cap");
    int cap = number();
    skipWord("q");
    int q = number();
    skipWord("queries");

    LRUCache cache(cap);
    Strs results;
    try {
        for (int n = 0; n < q; n = n + 1) {
            if (i >= (int)t.size()) fail("запросов меньше, чем Q");
            string cmd = upper(t[i]);
            i = i + 1;
            if (cmd == "SET") {
                if (i + 1 >= (int)t.size()) fail("для SET нужны ключ и значение");
                string key = t[i], value = t[i + 1];
                i = i + 2;
                cache.set(key, value);
                if (trace) cout << "SET " << key << " " << value << ": " << cache.show() << endl;
            } else if (cmd == "GET") {
                if (i >= (int)t.size()) fail("для GET нужен ключ");
                string key = t[i];
                i = i + 1;
                string value;
                bool found = cache.get(key, value);
                results.push_back(found ? value : "-1");
                if (trace) cout << "GET " << key << ": " << results.back() << " (" << cache.show() << ")" << endl;
            } else {
                fail("неизвестный запрос: " + t[i - 1]);
            }
        }
    } catch (const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }
    cout << join(results, " ") << endl;
}
