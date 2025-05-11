# pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include "order.hpp"

class OrderPool {
    std::vector<Order> pool;
    // Using atomic to ensure thread safety when multiple threads are allocating orders
    std::atomic<size_t> index;

public:
    OrderPool(size_t capacity) : index(0) {
        pool.reserve(capacity);
        pool.resize(capacity);
    }

    Order* allocate() {
        size_t i = index.fetch_add(1, std::memory_order_relaxed);
        if (i < pool.size()) return &pool[i];
        throw std::bad_alloc();
    }
};


class OptimizedOrderBook {
private:
    // Preallocating memory via an OderPool to avoid frequent allocations
    OrderPool pool;
    // storing pointers to Orders instead of copies
    std::unordered_map<std::string, Order*> orderLookup;
    std::map<double, std::unordered_map<std::string, Order*>> orderLevels[2]; // 0 for bids, 1 for asks

public:
    OptimizedOrderBook(size_t poolSize = 100000) 
    : pool(poolSize) {
        // Reserving even more space for the order levels to avoid collisions and rehashing
        orderLookup.reserve(10*poolSize);
    }

    void addOrder(const std::string& id, double price, int quantity, bool isBuy) {
        Order* order = pool.allocate();
        *order = {id, price, quantity, isBuy};
        int side = isBuy ? 0 : 1;
        orderLevels[side][price][id] = order;
        orderLookup[id] = order;
    }

    void modifyOrder(const std::string& id, double newPrice, int newQuantity) {
        auto try_find_order = orderLookup.find(id);
        if (try_find_order != orderLookup.end()) {
            Order* oldOrder = orderLookup[id];
            int side = oldOrder->isBuy ? 0 : 1;
            orderLevels[side][oldOrder->price].erase(id);
            addOrder(id, newPrice, newQuantity, oldOrder->isBuy);
        }
    }
    
    void deleteOrder(const std::string& id) {
        auto try_find_order = orderLookup.find(id);
        if (try_find_order != orderLookup.end()) {
            Order* oldOrder = orderLookup[id];
            int side = oldOrder->isBuy ? 0 : 1;
            orderLevels[side][oldOrder->price].erase(id);
        }
    }
    
    std::unordered_map<std::string, Order*> queryPrice(double price) {
        std::unordered_map<std::string, Order*> out;
        if (auto orders = orderLevels[0].find(price); orders != orderLevels[0].end()) {
            out.insert(orders->second.begin(), orders->second.end());
        }
        if (auto orders = orderLevels[1].find(price); orders != orderLevels[1].end()) {
            out.insert(orders->second.begin(), orders->second.end());
        }
        return out;
    }
};