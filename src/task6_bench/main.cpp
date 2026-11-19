// Задача 6 (анализ хеш-таблиц). Сравнение линейного апробирования и двойного хеширования.
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

#include "hashtable.h"

using namespace std;

static mt19937 rng;

static string randomKey(char prefix) {
    string s(1, prefix);
    for (int i = 0; i < 12; i = i + 1) s += (char)('a' + rng() % 26);
    return s;
}

static double theorySuccess(Method m, double a) {
    if (m == LINEAR) return 0.5 * (1 + 1 / (1 - a));
    return log(1 / (1 - a)) / a;
}
static double theoryFail(Method m, double a) {
    if (m == LINEAR) return 0.5 * (1 + 1 / ((1 - a) * (1 - a)));
    return 1 / (1 - a);
}

static void analyze(Method m, int cap, int samples) {
    cout << (m == LINEAR ? "Линейное апробирование" : "Двойное хеширование") << "  (ячеек: " << cap << ")\n";
    cout << " заполнение | вставка | успешный поиск (теория) | неуспешный поиск (теория)\n";
    cout << "------------+---------+-------------------------+--------------------------\n";
    HashTable t(m, cap, false, false);
    vector<string> keys;
    for (int pct = 10; pct <= 90; pct = pct + 10) {
        int target = cap * pct / 100;
        int inserted = 0;
        int before = t.probes;
        while ((int)keys.size() < target) {
            string k = randomKey('k');
            if (t.put(k, "v")) { keys.push_back(k); inserted = inserted + 1; }
        }
        double ins = (double)(t.probes - before) / (double)inserted;

        string v;
        before = t.probes;
        for (int i = 0; i < samples; i = i + 1) t.get(keys[(int)(rng() % (unsigned int)keys.size())], v);
        double succ = (double)(t.probes - before) / (double)samples;

        before = t.probes;
        for (int i = 0; i < samples; i = i + 1) t.get(randomKey('z'), v);
        double fail = (double)(t.probes - before) / (double)samples;

        double a = (double)keys.size() / (double)cap;
        cout << fixed << setprecision(2) << "    " << setw(3) << pct << "%    | " << setw(7) << ins << " | "
             << setw(9) << succ << " (" << setw(6) << theorySuccess(m, a) << ")      | "
             << setw(9) << fail << " (" << setw(7) << theoryFail(m, a) << ")\n";
    }
    cout << "\n";
}

static void bloomEffect(int cap, int samples) {
    cout << "Фильтр Блума в таблице с двойным хешированием (" << cap << " ячеек)\n";
    cout << " заполнение | проб на неуспешный поиск: без фильтра -> с фильтром | отсеяно фильтром | ложных срабатываний\n";
    cout << "------------+------------------------------------------------------+------------------+--------------------\n";
    int pcts[3] = {30, 50, 70};
    for (int pi = 0; pi < 3; pi = pi + 1) {
        int pct = pcts[pi];
        HashTable plain(DOUBLE, cap, false, false), withBloom(DOUBLE, cap, false, true);
        int n = cap * pct / 100;
        for (int i = 0; i < n;) {
            string k = randomKey('k');
            if (plain.put(k, "v")) { withBloom.put(k, "v"); i = i + 1; }
        }
        vector<string> absent;
        for (int i = 0; i < samples; i = i + 1) absent.push_back(randomKey('z'));
        string v;
        plain.resetCounters();
        withBloom.resetCounters();
        for (int i = 0; i < (int)absent.size(); i = i + 1) { plain.get(absent[i], v); withBloom.get(absent[i], v); }
        cout << fixed << setprecision(3) << "    " << setw(3) << pct << "%    | "
             << setw(22) << (double)plain.probes / (double)samples << "  ->  " << setw(10) << (double)withBloom.probes / (double)samples
             << "         | " << setw(8) << setprecision(2) << 100.0 * (double)withBloom.bloomRejects / (double)samples << "%        | "
             << setw(8) << setprecision(3) << 100.0 * (double)withBloom.bloomFalse / (double)samples << "%\n";
    }
    cout << "\n";
}

int main(int argc, char** argv) {
    int cap = 100003, samples = 20000;
    unsigned int seed = 12345;
    for (int i = 1; i + 1 < argc; i = i + 2) {
        string key = argv[i];
        if (key == "--cap") cap = atoi(argv[i + 1]);
        if (key == "--seed") seed = (unsigned int)atoi(argv[i + 1]);
        if (key == "--samples") samples = atoi(argv[i + 1]);
    }
    rng.seed(seed);
    cout << "Среднее число проб, " << samples << " запросов на каждый уровень заполнения\n\n";
    analyze(LINEAR, cap, samples);
    analyze(DOUBLE, cap, samples);
    bloomEffect(cap, samples);
}
