#include "io.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>

using namespace std;

string readInput(int argc, char** argv) {
    for (int i = 1; i + 1 < argc; i = i + 1) {
        if (string(argv[i]) == "--file") {
            ifstream in(argv[i + 1]);
            if (!in) { cerr << "ERROR: cannot open file: " << argv[i + 1] << endl; exit(1); }
            string result;
            char c;
            while (in.get(c)) result += c;
            return result;
        }
    }
    string result;
    char c;
    while (cin.get(c)) result += c;
    return result;
}
