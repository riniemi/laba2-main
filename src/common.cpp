#include "common.h"

#include <cctype>
#include <stdexcept>

using namespace std;

void fail(const string& msg) { throw runtime_error(msg); }

string join(const Strs& v, const string& sep) {
    string r;
    for (int i = 0; i < (int)v.size(); i = i + 1) {
        if (i > 0) r += sep;
        r += v[i];
    }
    return r;
}

string quote(const string& s) {
    bool need = s.empty();
    for (int i = 0; i < (int)s.size(); i = i + 1) {
        char c = s[i];
        if (c == ' ' || c == '\t' || c == '"' || c == '\\') need = true;
    }
    if (!need) return s;
    string r = "\"";
    for (int i = 0; i < (int)s.size(); i = i + 1) {
        char c = s[i];
        if (c == '"' || c == '\\') r += '\\';
        r += c;
    }
    return r + "\"";
}

string spaced(const Strs& v) {
    if (v.empty()) return "(empty)";
    Strs q;
    for (int i = 0; i < (int)v.size(); i = i + 1) q.push_back(quote(v[i]));
    return join(q, " ");
}

string upper(string s) {
    for (int i = 0; i < (int)s.size(); i = i + 1) {
        char c = s[i];
        if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
        s[i] = c;
    }
    return s;
}

bool endsWith(const string& s, const string& suf) {
    if (s.size() < suf.size()) return false;
    for (int i = 0; i < (int)suf.size(); i = i + 1)
        if (s[s.size() - suf.size() + i] != suf[i]) return false;
    return true;
}

Strs tokenize(const string& s) {
    Strs r;
    int i = 0, n = (int)s.size();
    while (i < n) {
        while (i < n && (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n')) i = i + 1;
        if (i >= n) break;
        string tok;
        if (s[i] == '"') {
            i = i + 1;
            bool closed = false;
            while (i < n) {
                char c = s[i];
                i = i + 1;
                if (c == '\\' && i < n) { tok += s[i]; i = i + 1; }
                else if (c == '"') { closed = true; break; }
                else tok += c;
            }
            if (!closed) fail("unterminated quote in: " + s);
        } else {
            while (i < n && s[i] != ' ' && s[i] != '\t' && s[i] != '\r' && s[i] != '\n') { tok += s[i]; i = i + 1; }
        }
        r.push_back(tok);
    }
    return r;
}

const string& arg(const Strs& a, int i) {
    if (i < 0 || i >= (int)a.size()) fail("missing argument");
    return a[i];
}

int toIndex(const string& s) {
    if (s.empty()) fail("bad index: " + s);
    for (int i = 0; i < (int)s.size(); i = i + 1)
        if (s[i] < '0' || s[i] > '9') fail("bad index: " + s);
    int v = 0;
    for (int i = 0; i < (int)s.size(); i = i + 1) v = v * 10 + (s[i] - '0');
    return v;
}

bool isInteger(const string& s) {
    int i = 0;
    if (!s.empty() && s[0] == '-') i = 1;
    if (i >= (int)s.size()) return false;
    for (; i < (int)s.size(); i = i + 1)
        if (s[i] < '0' || s[i] > '9') return false;
    return true;
}

string normInt(const string& s) {
    if (!isInteger(s)) fail("tree stores integers only, got: " + s);
    int v = 0;
    int neg = 0;
    int i = 0;
    if (!s.empty() && s[0] == '-') { neg = 1; i = 1; }
    for (; i < (int)s.size(); i = i + 1) v = v * 10 + (s[i] - '0');
    if (neg) return "-" + to_string(v);
    return to_string(v);
}
