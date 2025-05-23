# pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include <tuple>

struct Order {
    std::string id;  // String-based order ID
    double price;
    int quantity;
    bool isBuy;
};


class OrderBook {
private:
    std::unordered_map<std::string, std::tuple<double,double>> orderBook;

public:
    void addOrder(const std::string& sec_id, double bid, double ask) {
        orderBook[sec_id] = std::make_tuple(bid, ask);
    }
    
    
    std::tuple<double,double> queryTicker(const std::string& sec_id) {        
        return orderBook[sec_id];
    }

    void print_book() const {
        std::cout << "\n=== ORDER BOOK ===\n";
        for (const auto& [sec_id, prices] : orderBook) {
            std::cout << "Security ID: " << sec_id
                      << " | Bid: " << std::get<0>(prices)
                      << " | Ask: " << std::get<1>(prices) << "\n";
        }
        std::cout << "===================\n";
    }
};