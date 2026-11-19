// Фильтр Блума.
#pragma once
#include <string>
#include <vector>

struct BloomFilter {
    std::vector<unsigned int> words;
    int m;
    int k;

    BloomFilter(int bits = 64, int hashes = 4) {
        m = bits < 64 ? 64 : bits;
        k = hashes;
        words.assign((m + 31) / 32, 0);
    }

    void reset(int bits) {
        m = bits < 64 ? 64 : bits;
        words.assign((m + 31) / 32, 0);
    }

    void add(const std::string& key);
    bool mightContain(const std::string& key) const;
    int bits() const { return m; }
    int hashes() const { return k; }
    int setBits() const;

private:
    int bitIndex(const std::string& key, int i) const;
};
