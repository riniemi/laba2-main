#include "hashtable.h"

#include <cstdio>
#include <utility>
#include <vector>

#include "hashfn.h"

using namespace std;

static const char SEP = '\x1F';
static const string DELETED = "\x01";
static const int NONE = -1;

static bool isDeleted(const string& s) { return s == DELETED; }

static bool slotHasKey(const string& s, const string& key) {
    return s.size() > key.size() && s[(size_t)key.size()] == SEP && s.compare(0, key.size(), key) == 0;
}

static bool isPrime(int n) {
    if (n < 2) return false;
    for (int d = 2; d * d <= n; d = d + 1) if (n % d == 0) return false;
    return true;
}

static int nextPrime(int n) {
    while (!isPrime(n)) n = n + 1;
    return n;
}

void HashTable::init(Method m, int capacity) {
    method = m;
    cap = nextPrime(capacity < 3 ? 3 : capacity);
    while (slots.size() > 0) slots.erase(slots.size() - 1);
    for (int i = 0; i < cap; i = i + 1) slots.push("");
    count = 0;
    tombs = 0;
    bloom.reset(cap * 10);
}

HashTable::HashTable(Method m, int capacity, bool grow, bool bloomOn)
    : method(m), cap(0), autoGrow(grow), useBloom(bloomOn), bloom(64, 4) {
    init(m, capacity);
}

int HashTable::home(const string& key) const {
    return (int)(hashDjb2(key) % (unsigned int)cap);
}

int HashTable::stepOf(const string& key) const {
    if (method == LINEAR) return 1;
    return 1 + (int)(hashFnv1a(key) % (unsigned int)(cap - 1));
}

int HashTable::findIndex(const string& key) {
    int idx = home(key);
    int step = stepOf(key);
    for (int n = 0; n < cap; n = n + 1) {
        probes = probes + 1;
        const string& s = slots.get((size_t)idx);
        if (s.empty()) return NONE;
        if (!isDeleted(s) && slotHasKey(s, key)) return idx;
        idx = (idx + step) % cap;
    }
    return NONE;
}

void HashTable::insertFresh(const string& key, const string& value) {
    int idx = home(key);
    int step = stepOf(key);
    for (int n = 0; n < cap; n = n + 1) {
        probes = probes + 1;
        const string& s = slots.get((size_t)idx);
        if (s.empty() || isDeleted(s)) {
            if (isDeleted(s)) tombs = tombs - 1;
            slots.set((size_t)idx, key + SEP + value);
            count = count + 1;
            bloom.add(key);
            return;
        }
        idx = (idx + step) % cap;
    }
    fail("hash table is full");
}

void HashTable::rehash(int newCap) {
    vector<pair<string, string>> live;
    for (int i = 0; i < cap; i = i + 1) {
        const string& s = slots.get((size_t)i);
        if (s.empty() || isDeleted(s)) continue;
        size_t p = s.find(SEP);
        live.push_back(make_pair(s.substr(0, p), s.substr(p + 1)));
    }
    init(method, newCap);
    for (int i = 0; i < (int)live.size(); i = i + 1)
        insertFresh(live[i].first, live[i].second);
}

bool HashTable::put(const string& key, const string& value) {
    if (key.find(SEP) != string::npos) fail("bad key");
    if (autoGrow && (count + tombs + 1) * 10 > cap * 7)
        rehash((count + 1) * 2 > cap ? cap * 2 : cap);
    int idx = home(key);
    int step = stepOf(key);
    int firstFree = NONE;
    for (int n = 0; n < cap; n = n + 1) {
        probes = probes + 1;
        const string& s = slots.get((size_t)idx);
        if (s.empty()) { if (firstFree == NONE) firstFree = idx; break; }
        if (isDeleted(s)) {
            if (firstFree == NONE) firstFree = idx;
        } else if (slotHasKey(s, key)) {
            slots.set((size_t)idx, key + SEP + value);
            return false;
        }
        idx = (idx + step) % cap;
    }
    if (firstFree == NONE) fail("hash table is full");
    if (isDeleted(slots.get((size_t)firstFree))) tombs = tombs - 1;
    slots.set((size_t)firstFree, key + SEP + value);
    count = count + 1;
    bloom.add(key);
    return true;
}

bool HashTable::get(const string& key, string& value) {
    if (useBloom && !bloom.mightContain(key)) { bloomRejects = bloomRejects + 1; return false; }
    int idx = findIndex(key);
    if (idx == NONE) {
        if (useBloom) bloomFalse = bloomFalse + 1;
        return false;
    }
    const string& s = slots.get((size_t)idx);
    value = s.substr(s.find(SEP) + 1);
    return true;
}

bool HashTable::contains(const string& key) {
    string v;
    return get(key, v);
}

bool HashTable::erase(const string& key) {
    int idx = findIndex(key);
    if (idx == NONE) return false;
    slots.set((size_t)idx, DELETED);
    count = count - 1;
    tombs = tombs + 1;
    return true;
}

Strs HashTable::dump() const {
    Strs live, tomb;
    for (int i = 0; i < cap; i = i + 1) {
        const string& s = slots.get((size_t)i);
        if (s.empty()) continue;
        if (isDeleted(s)) { tomb.push_back(to_string(i)); continue; }
        size_t p = s.find(SEP);
        live.push_back(to_string(i));
        live.push_back(s.substr(0, p));
        live.push_back(s.substr(p + 1));
    }
    Strs r;
    r.push_back(method == LINEAR ? "linear" : "double");
    r.push_back(to_string(cap));
    r.push_back(to_string(count));
    r.push_back(to_string(tombs));
    for (int i = 0; i < (int)live.size(); i = i + 1) r.push_back(live[i]);
    for (int i = 0; i < (int)tomb.size(); i = i + 1) r.push_back(tomb[i]);
    return r;
}

void HashTable::load(const Strs& t) {
    if (t.size() < 4 || (t[0] != "linear" && t[0] != "double")) fail("corrupted data file");
    int c = toIndex(t[1]);
    int nLive = toIndex(t[2]);
    int nTomb = toIndex(t[3]);
    if ((int)t.size() != 4 + 3 * nLive + nTomb || !isPrime(c)) fail("corrupted data file");
    init(t[0] == "linear" ? LINEAR : DOUBLE, c);
    for (int k = 0; k < nLive; k = k + 1) {
        int idx = toIndex(t[4 + 3 * k]);
        if (idx >= cap) fail("corrupted data file");
        slots.set((size_t)idx, t[5 + 3 * k] + SEP + t[6 + 3 * k]);
        bloom.add(t[5 + 3 * k]);
        count = count + 1;
    }
    for (int k = 0; k < nTomb; k = k + 1) {
        int idx = toIndex(t[4 + 3 * nLive + k]);
        if (idx >= cap) fail("corrupted data file");
        slots.set((size_t)idx, DELETED);
        tombs = tombs + 1;
    }
}

string HashTable::show() const {
    string r = stats();
    for (int i = 0; i < cap; i = i + 1) {
        const string& s = slots.get((size_t)i);
        if (s.empty()) continue;
        if (isDeleted(s)) { r = r + "\n[" + to_string(i) + "] (deleted)"; continue; }
        size_t p = s.find(SEP);
        r = r + "\n[" + to_string(i) + "] " + quote(s.substr(0, p)) + " => " + quote(s.substr(p + 1));
    }
    return r;
}

string HashTable::stats() const {
    int set = (int)bloom.setBits();
    double fill = (double)set / (double)bloom.bits();
    double fp = 1;
    for (int i = 0; i < bloom.hashes(); i = i + 1) fp = fp * fill;
    auto pct = [](double x) {
        char buf[32];
        if (x < 0.1) snprintf(buf, sizeof buf, "%.2f%%", x * 100);
        else snprintf(buf, sizeof buf, "%.0f%%", x * 100);
        return string(buf);
    };
    return string("method: ") + (method == LINEAR ? "linear probing" : "double hashing") +
           ", capacity: " + to_string(cap) + ", size: " + to_string(count) + ", deleted: " + to_string(tombs) +
           ", load: " + pct((double)count / cap) + "\nbloom: " + to_string(bloom.bits()) + " bits, " +
           to_string(bloom.hashes()) + " hashes, bits set: " + pct(fill) + ", false positive estimate: " + pct(fp);
}
