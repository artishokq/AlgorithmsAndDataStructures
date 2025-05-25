#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>

class StringGenerator {
private:
    const std::string CHARSET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-.";

    std::mt19937 rng;
    const int MIN_LENGTH = 10;
    const int MAX_LENGTH = 200;
    const int MAX_ARRAY_SIZE = 3000;
    std::uniform_int_distribution<int> char_dist;
    std::uniform_int_distribution<int> length_dist;
    std::vector<std::string> master_array;

public:
    StringGenerator() :
            rng(std::chrono::steady_clock::now().time_since_epoch().count()),
            char_dist(0, CHARSET.length() - 1),
            length_dist(MIN_LENGTH, MAX_LENGTH) {}

    std::string generateRandomString() {
        int length = length_dist(rng);
        std::string result;
        result.reserve(length);

        for (int i = 0; i < length; ++i) {
            result += CHARSET[char_dist(rng)];
        }

        return result;
    }

    std::string generateStringWithPrefix(const std::string& prefix, int total_length = -1) {
        if (total_length == -1) {
            total_length = length_dist(rng);
        }

        total_length = std::max(total_length, static_cast<int>(prefix.length()));
        std::string result = prefix;

        for (int i = prefix.length(); i < total_length; ++i) {
            result += CHARSET[char_dist(rng)];
        }

        return result;
    }

    std::vector<std::string> generateRandomArray(int size) {
        std::vector<std::string> result;
        result.reserve(size);

        for (int i = 0; i < size; ++i) {
            result.push_back(generateRandomString());
        }

        return result;
    }

    std::vector<std::string> generateArrayWithPrefixes(int size, int prefix_groups = 5) {
        std::vector<std::string> result;
        result.reserve(size);

        std::vector<std::string> prefixes;
        for (int i = 0; i < prefix_groups; ++i) {
            int prefix_length = 5 + rng() % 15;
            std::string prefix;
            for (int j = 0; j < prefix_length; ++j) {
                prefix += CHARSET[char_dist(rng)];
            }
            prefixes.push_back(prefix);
        }

        for (int i = 0; i < size; ++i) {
            const std::string& prefix = prefixes[rng() % prefixes.size()];
            result.push_back(generateStringWithPrefix(prefix));
        }

        std::shuffle(result.begin(), result.end(), rng);

        return result;
    }

    std::vector<std::string> generateReverseSortedArray(int size) {
        std::vector<std::string> result = generateRandomArray(size);
        std::sort(result.begin(), result.end(), std::greater<std::string>());
        return result;
    }

    std::vector<std::string> generateNearlySortedArray(int size, double disorder_ratio = 0.05) {
        std::vector<std::string> result = generateRandomArray(size);
        std::sort(result.begin(), result.end());

        int swaps = static_cast<int>(size * disorder_ratio);

        std::uniform_int_distribution<int> index_dist(0, size - 1);

        for (int i = 0; i < swaps; ++i) {
            int idx1 = index_dist(rng);
            int idx2 = index_dist(rng);
            if (idx1 != idx2) {
                std::swap(result[idx1], result[idx2]);
            }
        }

        return result;
    }

    void initializeMasterArrays() {
        std::cout << "Инициализация основных массивов..." << std::endl;

        master_array = generateRandomArray(MAX_ARRAY_SIZE);

        std::cout << "Основной массив создан: " << master_array.size() << " строк" << std::endl;
    }

    std::vector<std::string> getSubarray(std::size_t count, const std::string& type = "random") {
        if (count > master_array.size()) {
            throw std::runtime_error("Запрошенный размер больше основного массива");
        }

        std::vector<std::string> result(master_array.begin(), master_array.begin() + count);

        if (type == "reverse") {
            std::sort(result.begin(), result.end(), std::greater<std::string>());
        } else if (type == "nearly_sorted") {
            std::sort(result.begin(), result.end());
            int swaps = static_cast<int>(count * 0.05);
            std::uniform_int_distribution<int> index_dist(0, count - 1);

            for (int i = 0; i < swaps; ++i) {
                int idx1 = index_dist(rng);
                int idx2 = index_dist(rng);
                if (idx1 != idx2) {
                    std::swap(result[idx1], result[idx2]);
                }
            }
        }
        return result;
    }
};