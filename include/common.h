// Общие вспомогательные функции: ошибки, разбор запроса, строки, числа.
#pragma once
#include <string>
#include <vector>

using Strs = std::vector<std::string>;

void fail(const std::string& msg);

std::string join(const Strs& v, const std::string& sep);
std::string quote(const std::string& s);
std::string spaced(const Strs& v);
std::string upper(std::string s);
bool endsWith(const std::string& s, const std::string& suf);

Strs tokenize(const std::string& line);

const std::string& arg(const Strs& a, int i);
int toIndex(const std::string& s);

bool isInteger(const std::string& s);
std::string normInt(const std::string& s);
