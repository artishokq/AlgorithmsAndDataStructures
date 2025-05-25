#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <random>
#include <utility>
#include "StringGenerator.cpp"
#include "Algos.cpp"

class StringSortTester {
private:
    static const int MIN_ARRAY_SIZE = 100;
    static const int MAX_ARRAY_SIZE = 3000;
    static const int STEP_SIZE = 100;
    static const int NUM_MEASUREMENTS = 5;

    template<typename SortFunc>
    std::pair<double, long long> measureAlgorithm(SortFunc sortFunction, std::vector<std::string> data) {
        comparison_count = 0;
        auto start = std::chrono::high_resolution_clock::now();
        sortFunction(data);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        double time_ms = duration.count() / 1000.0;

        return {time_ms, comparison_count};
    }

public:
    template<typename SortFunc>
    void testAlgorithm(StringGenerator& generator, SortFunc sortFunction,
                       const std::string& algorithm_name, const std::string& array_type) {

        std::string filename = algorithm_name + "_" + array_type + ".txt";
        std::ofstream file(filename);

        if (!file.is_open()) {
            return;
        }

        for (int size = MIN_ARRAY_SIZE; size <= MAX_ARRAY_SIZE; size += STEP_SIZE) {
            std::vector<double> times;
            std::vector<long long> comparisons;

            for (int i = 0; i < NUM_MEASUREMENTS; ++i) {
                std::vector<std::string> test_data = generator.getSubarray(size, array_type);
                auto [time, comp] = measureAlgorithm(sortFunction, test_data);
                times.push_back(time);
                comparisons.push_back(comp);
            }

            double avg_time = std::accumulate(times.begin(), times.end(), 0.0) / NUM_MEASUREMENTS;
            long long avg_comp = std::accumulate(comparisons.begin(), comparisons.end(), 0LL) / NUM_MEASUREMENTS;

            file << size << ";" << avg_comp << ";" << std::fixed << std::setprecision(3) << avg_time << std::endl;
        }

        file.close();
    }

    void runFullTest(StringGenerator& generator) {
        std::vector<std::string> array_types = {"random", "reverse", "nearly_sorted"};

        generator.initializeMasterArrays();

        for (const auto& array_type : array_types) {
            testAlgorithm(generator, [](std::vector<std::string> data) { standardQuickSort(data); },
                          "standard_quicksort", array_type);

            testAlgorithm(generator, [](std::vector<std::string> data) { standardMergeSort(data); },
                          "standard_mergesort", array_type);

            testAlgorithm(generator, [](std::vector<std::string> data) { ternaryStringQuickSort(data); },
                          "ternary_quicksort", array_type);

            testAlgorithm(generator, [](std::vector<std::string> data) { stringMergeSortWithLCP(data); },
                          "string_mergesort_lcp", array_type);

            testAlgorithm(generator, [](std::vector<std::string> data) { MSDRadixSort(data); },
                          "msd_radix_sort", array_type);

            testAlgorithm(generator, [](std::vector<std::string> data) { MSDRadixSortWithSwitch(data); },
                          "msd_radix_sort_switch", array_type);
        }
    }
};