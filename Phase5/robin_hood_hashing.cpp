#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <random>

struct MarketData {
        std::string symbol;
        double price;
};

class RobinHoodHashTable {
    struct Entry {
        std::string key;
        double value;
        size_t probe_distance; // Distance from the original hash position

        Entry(std::string key, double val)
            : key(key), value(val), probe_distance(0) {}
    };

    std::vector<Entry> table;
    //std::vector<std::pair<std::string, double>> table;
    size_t capacity;
    size_t size;

    size_t hashFunction(std::string key) {
        return std::hash<std::string>{}(key) % capacity;
    }

public:
    RobinHoodHashTable(size_t initial_capacity = 1024)
        : capacity(initial_capacity), size(0) {
        table = std::vector<Entry>(capacity, Entry("", 0.0));
    }

    void insert(const std::string& key, double value) {
        Entry newEntry(key, value);
        size_t idx = hashFunction(key);

        while (table[idx].key != "" && table[idx].key != key) {
            if (table[idx].probe_distance < newEntry.probe_distance) {
                std::swap(newEntry, table[idx]);
            }
            newEntry.probe_distance++;
            idx = (idx + 1) % capacity;
        }

        table[idx] = newEntry;
        size++;
    }
        

    void remove(const std::string& key) {
        size_t idx = hashFunction(key);
        size_t offset = 0;

        while (offset < capacity) {
            if (table[idx].key == key) {
                table[idx].key = ""; // Mark as deleted
                size--;
                return;
            }
            offset++;
            idx = (idx + 1) % capacity;
        }
    }

    bool lookup(const std::string& key, double& value_out) {
        size_t idx = hashFunction(key);
        size_t offset = 0;

        while (offset < capacity) {
            if (table[idx].key == key) {
                value_out = table[idx].value;
                return true;
            }
            if (table[idx].key == "") {
                break; // Not found
            }
            offset++;
            idx = (idx + 1) % capacity;
        }
        return false; // Not found
    }

};

int main() {
    // benchmarking robin hood hashing against std::unordered_map
    const int num_elements = 100000;
    std::vector<std::string> symbols;
    RobinHoodHashTable rh_table(num_elements);
    std::unordered_map<std::string, double> std_map(num_elements);

    std::mt19937 rng(42); // Seed for reproducibility
    std::uniform_real_distribution<double> price_dist(10.0, 1000.0);

    // Generate fake stock symbols
    for (int i = 0; i < num_elements; ++i) {
        symbols.push_back("SYMB" + std::to_string(i));
    }

    // Robin Hood insertion
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& sym : symbols) {
        rh_table.insert(sym, price_dist(rng));
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Robin Hood Insert: " << duration.count() << "milliseconds\n";

    // std::unordered_map insertion
    start = std::chrono::high_resolution_clock::now();
    for (const auto& sym : symbols) {
        std_map[sym] = price_dist(rng);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "std::unordered_map Insert: " << duration.count() << "milliseconds\n";

    // Lookup test
    double price;
    int found = 0;
    start = std::chrono::high_resolution_clock::now();
    for (const auto& sym : symbols) {
        if (rh_table.lookup(sym, price)) found++;
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Robin Hood Lookup: " << duration.count() << "milliseconds\n";

    found = 0;
    start = std::chrono::high_resolution_clock::now();
    for (const auto& sym : symbols) {
        if (std_map.find(sym) != std_map.end()) found++;
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "std::unordered_map Lookup: " << duration.count() << "milliseconds\n";
}
