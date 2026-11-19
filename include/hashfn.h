// Две хеш-функции для строк. Метод деления применяется снаружи.
#pragma once
#include <string>

inline unsigned int hashDjb2(const std::string& s) {
    unsigned int h = 5381;
    for (int i = 0; i < (int)s.size(); i = i + 1) h = h * 33 + (unsigned char)s[i];
    return h;
}

inline unsigned int hashFnv1a(const std::string& s) {
    unsigned int h = 2166136261u;
    for (int i = 0; i < (int)s.size(); i = i + 1) { h = h ^ (unsigned char)s[i]; h = h * 16777619u; }
    return h;
}
