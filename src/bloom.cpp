#include "bloom.h"

#include "hashfn.h"

using namespace std;

int BloomFilter::bitIndex(const string& key, int i) const {
    return (int)((hashDjb2(key) + (unsigned int)i * (hashFnv1a(key) | 1u)) % (unsigned int)m);
}

void BloomFilter::add(const string& key) {
    for (int i = 0; i < k; i = i + 1) {
        int b = bitIndex(key, i);
        words[b / 32] = words[b / 32] | (1u << (b % 32));
    }
}

bool BloomFilter::mightContain(const string& key) const {
    for (int i = 0; i < k; i = i + 1) {
        int b = bitIndex(key, i);
        if (!((words[b / 32] >> (b % 32)) & 1u)) return false;
    }
    return true;
}

int BloomFilter::setBits() const {
    int n = 0;
    for (int i = 0; i < (int)words.size(); i = i + 1) {
        unsigned int w = words[i];
        while (w != 0) {
            n = n + (w & 1u);
            w = w >> 1;
        }
    }
    return n;
}
