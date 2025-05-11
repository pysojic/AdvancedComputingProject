# pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include "order.hpp"


class OrderBook {
private:
    std::unordered_map<std::string, Order> orderLookup;
    std::map<double, std::unordered_map<std::string, Order>> orderLevels[2]; // 0 for bids, 1 for asks

public:
    void addOrder(const std::string& id, double price, int quantity, bool isBuy) {
        Order order = {id, price, quantity, isBuy};
        int side = isBuy ? 0 : 1;
        orderLevels[side][price][id] = order;
        orderLookup[id] = order;
    }

    void modifyOrder(const std::string& id, double newPrice, int newQuantity) {
        auto try_find_order = orderLookup.find(id);
        if (try_find_order != orderLookup.end()) {
            Order oldOrder = orderLookup[id];
            int side = oldOrder.isBuy ? 0 : 1;
            orderLevels[side][oldOrder.price].erase(id);
            addOrder(id, newPrice, newQuantity, oldOrder.isBuy);
        }
    }
    
    void deleteOrder(const std::string& id) {
        auto try_find_order = orderLookup.find(id);
        if (try_find_order != orderLookup.end()) {
            Order oldOrder = orderLookup[id];
            int side = oldOrder.isBuy ? 0 : 1;
            orderLevels[side][oldOrder.price].erase(id);
        }
    }
    
    std::unordered_map<std::string, Order> queryPrice(double price) {
        std::unordered_map<std::string, Order> out;
        if (auto orders = orderLevels[0].find(price); orders != orderLevels[0].end()) {
            out.insert(orders->second.begin(), orders->second.end());
        }
        if (auto orders = orderLevels[1].find(price); orders != orderLevels[1].end()) {
            out.insert(orders->second.begin(), orders->second.end());
        }
        return out;
    }
};