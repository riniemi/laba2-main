// Задача 1 (XML). Проверка строки из тегов.
#include <iostream>

#include "io.h"
#include "stack.h"

using namespace std;

static bool isValidXml(const string& s) {
    Stack st;
    int i = 0, n = (int)s.size();
    while (i < n) {
        if (s[i] != '<') return false;
        i = i + 1;
        bool closing = i < n && s[i] == '/';
        if (closing) i = i + 1;
        int begin = i;
        while (i < n && s[i] >= 'a' && s[i] <= 'z') i = i + 1;
        if (i == begin || i >= n || s[i] != '>') return false;
        string name = s.substr(begin, i - begin);
        i = i + 1;
        if (!closing) {
            st.push(name);
        } else {
            if (st.empty() || st.top() != name) return false;
            st.pop();
        }
    }
    return st.empty();
}

int main(int argc, char** argv) {
    string text = readInput(argc, argv);
    int eol = (int)text.find('\n');
    if (eol >= 0) text = text.substr(0, eol);

    string s;
    for (int i = 0; i < (int)text.size(); i = i + 1) {
        char c = text[i];
        if (c != ' ' && c != '\r' && c != '\t') s += c;
    }
    bool hadSpaces = s.size() != text.size();

    const string alphabet = "abcdefghijklmnopqrstuvwxyz</>";
    for (int i = (int)s.size() - 1; i >= 0; i = i - 1) {
        char original = s[i];
        for (int j = 0; j < (int)alphabet.size(); j = j + 1) {
            char c = alphabet[j];
            if (c == original) continue;
            s[i] = c;
            if (isValidXml(s)) {
                string out;
                for (int k = 0; k < (int)s.size(); k = k + 1) {
                    char ch = s[k];
                    out += ch;
                    if (hadSpaces && ch == '>') out += ' ';
                }
                while (!out.empty() && out.back() == ' ') out.pop_back();
                cout << out << endl;
                return 0;
            }
        }
        s[i] = original;
    }
    cout << "Incorrect" << endl;
    return 0;
}
