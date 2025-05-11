#ifndef ROBINHOOD_HASH_TABLE_H
#define ROBINHOOD_HASH_TABLE_H

#include <vector>
#include <optional>
#include <functional>

template<typename K, typename V>
class RobinHoodHashTable {
public:
    RobinHoodHashTable(size_t capacity)
        : table(capacity), tableSize(capacity), numElements(0) {}
    bool insert(const K& key, const V& value) {
        if ((numElements + 1) * 2 > tableSize) resize();
        size_t idx = hashFunction(key);
        Entry entry{key, value, true, false, 0};
        for (size_t dist = 0; dist < tableSize; ++dist) {
            Entry& slot = table[idx];
            if (!slot.occupied || slot.deleted) {
                table[idx] = entry;
                ++numElements;
                return true;
            }
            size_t existingDist = (idx + tableSize - hashFunction(slot.key)) % tableSize;
            if (existingDist < dist) {
                std::swap(entry, slot);
                dist = entry.probe_len;
            }
            idx = (idx + 1) % tableSize;
        }
        return false;
    }
    bool erase(const K& key) {
        size_t idx = hashFunction(key);
        for (size_t dist = 0; dist < tableSize; ++dist) {
            Entry& slot = table[idx];
            if (!slot.occupied && !slot.deleted) return false;
            if (slot.occupied && slot.key == key) {
                slot.occupied = false;
                slot.deleted = true;
                --numElements;
                return true;
            }
            idx = (idx + 1) % tableSize;
        }
        return false;
    }
    std::optional<V> find(const K& key) const {
        size_t idx = hashFunction(key);
        for (size_t dist = 0; dist < tableSize; ++dist) {
            const Entry& slot = table[idx];
            if (!slot.occupied && !slot.deleted) return std::nullopt;
            if (slot.occupied && slot.key == key) return slot.value;
            idx = (idx + 1) % tableSize;
        }
        return std::nullopt;
    }
private:
    struct Entry {
        K key;
        V value;
        bool occupied = false;
        bool deleted = false;
        size_t probe_len = 0;
    };
    std::vector<Entry> table;
    size_t tableSize;
    size_t numElements;
    size_t hashFunction(const K& key) const {
        return std::hash<K>{}(key) % tableSize;
    }
    void resize() {
        size_t newSize = tableSize * 2;
        std::vector<Entry> old = std::move(table);
        table.assign(newSize, Entry());
        tableSize = newSize;
        numElements = 0;
        for (auto& e : old)
            if (e.occupied)
                insert(e.key, e.value);
    }
};

#endif
