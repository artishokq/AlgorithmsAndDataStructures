#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

class HashTableQuadratic {
public:
    int tableSize;
    std::vector<int> table;
    int collisions;
    int c1;
    int c2;

    HashTableQuadratic(int size, int c1 = 1, int c2 = 1): tableSize(size), collisions(0), c1(c1), c2(c2) {
        table.assign(tableSize, -1);
    }

    bool insert(int key) {
        int initialIndex = key % tableSize;
        for (int i = 0; i < tableSize; ++i) {
            int index = (initialIndex + c1 * i + c2 * i * i) % tableSize;
            if (table[index] == -1) {
                table[index] = key;
                return true;
            } else {
                ++collisions;
            }
        }
        return false;
    }
};

class HashTableCubic {
public:
    int tableSize;
    std::vector<int> table;
    int collisions;
    int c1;
    int c2;
    int c3;

    HashTableCubic(int size, int c1 = 1, int c2 = 1, int c3 = 1) : tableSize(size), collisions(0), c1(c1), c2(c2), c3(c3) {
        table.assign(tableSize, -1);
    }

    bool insert(int key) {
        int initialIndex = key % tableSize;
        for (int i = 0; i < tableSize; ++i) {
            int index = (initialIndex + c1 * i + c2 * i * i + c3 * i * i * i) % tableSize;
            if (table[index] == -1) {
                table[index] = key;
                return true;
            } else {
                ++collisions;
            }
        }
        return false;
    }
};

template <typename HashTable>
int runExperiment(int tableSize, int numKeys) {
    HashTable table(tableSize);
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, 100000);

    for (int i = 0; i < numKeys; ++i) {
        int key = dist(rng);
        bool inserted = table.insert(key);
        if (!inserted) {
            break;
        }
    }
    return table.collisions;
}

int main() {
    std::vector<int> tableSizes = {100, 200, 500};
    std::vector<int> keyCounts   = {80, 160, 400};
    int repeats = 50;

    std::cout << "Cреднее число коллизий при вставке ключей\n\n";

    for (size_t i = 0; i < tableSizes.size() && i < keyCounts.size(); ++i) {
        int tableSize = tableSizes[i];
        int numKeys   = keyCounts[i];

        long long totalCollisionsQuad = 0;
        long long totalCollisionsCubic = 0;

        for (int t = 0; t < repeats; ++t) {
            totalCollisionsQuad += runExperiment<HashTableQuadratic>(tableSize, numKeys);
            totalCollisionsCubic += runExperiment<HashTableCubic>(tableSize, numKeys);
        }

        double avgQuad = static_cast<double>(totalCollisionsQuad) / repeats;
        double avgCubic = static_cast<double>(totalCollisionsCubic) / repeats;

        std::cout << "Эксперимент для размера таблицы " << tableSize
                  << " и количества ключей " << numKeys << ":\n";
        std::cout << "Квадратичное пробирование: среднее число коллизий = "
                  << std::fixed << std::setprecision(2) << avgQuad << "\n";
        std::cout << "Кубическое пробирование:  среднее число коллизий = "
                  << std::fixed << std::setprecision(2) << avgCubic << "\n\n";
    }
    return 0;
}