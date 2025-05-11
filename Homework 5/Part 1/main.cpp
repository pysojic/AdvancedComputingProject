#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include "robinhood_hash_table.h"

int main() {
    const size_t n = 100000;
    std::vector<std::string> keys;
    keys.reserve(n);
    for (size_t i = 0; i < n; ++i)
        keys.push_back("SYM" + std::to_string(i));
    std::mt19937 rng(42);
    std::shuffle(keys.begin(), keys.end(), rng);

    RobinHoodHashTable<std::string, int> rh(n * 2);
    auto t0 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n; ++i) rh.insert(keys[i], i);
    auto t1 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n; ++i) rh.find(keys[i]);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "RobinHood insert ns/op: "
              << (t1 - t0).count() / n << "\n";
    std::cout << "RobinHood find ns/op: "
              << (t2 - t1).count() / n << "\n";

    std::unordered_map<std::string, int> um;
    t0 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n; ++i) um[keys[i]] = i;
    t1 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n; ++i) um.find(keys[i]);
    t2 = std::chrono::high_resolution_clock::now();

    std::cout << "std::unordered_map insert ns/op: "
              << (t1 - t0).count() / n << "\n";
    std::cout << "std::unordered_map find ns/op: "
              << (t2 - t1).count() / n << "\n";

    return 0;
}
