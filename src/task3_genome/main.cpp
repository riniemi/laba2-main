// Задача 3 (геномы). Мера близости двух геномов по парам соседних символов.
#include <iostream>
#include <sstream>

#include "hashset.h"
#include "io.h"

using namespace std;

static string trimmed(const string& s) {
    int b = (int)s.find_first_not_of(" \t\r\n");
    if (b < 0) return "";
    int e = (int)s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

static bool onlyLetters(const string& s) {
    for (int i = 0; i < (int)s.size(); i = i + 1)
        if (s[i] < 'A' || s[i] > 'Z') return false;
    return true;
}

int main(int argc, char** argv) {
    istringstream in(readInput(argc, argv));
    string first, second;
    getline(in, first);
    getline(in, second);
    first = trimmed(first);
    second = trimmed(second);
    if (first.empty() || second.empty() || !onlyLetters(first) || !onlyLetters(second)) {
        cerr << "ERROR: нужны две непустые строки из заглавных латинских букв A-Z" << endl;
        return 1;
    }

    HashSet pairs;
    for (int i = 0; i + 1 < (int)second.size(); i = i + 1) pairs.add(second.substr(i, 2));

    int closeness = 0;
    for (int i = 0; i + 1 < (int)first.size(); i = i + 1)
        if (pairs.contains(first.substr(i, 2))) closeness = closeness + 1;
    cout << closeness << endl;
}
