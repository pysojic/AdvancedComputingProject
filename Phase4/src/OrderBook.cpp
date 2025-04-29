#include "../include/OrderBook.hpp"
#include "config.h"

template<typename PriceType, typename OrderIdType>
OrderBook<PriceType, OrderIdType>::OrderBook() {}

template<typename PriceType, typename OrderIdType>
void OrderBook<PriceType, OrderIdType>::add_order(std::shared_ptr<Order<PriceType, OrderIdType>> order) {
    if (order->is_buy) {
        buy_orders.emplace(order->price, order);
    } else {
        sell_orders.emplace(order->price, order);
    }
}

template<typename PriceType, typename OrderIdType>
void OrderBook<PriceType, OrderIdType>::remove_order(OrderIdType id) {
    auto remove_from = [id](auto& orders) {
        for (auto it = orders.begin(); it != orders.end(); ++it) {
            if (it->second->id == id) {
                orders.erase(it);
                return;
            }
        }
    };
    remove_from(buy_orders);
    remove_from(sell_orders);
}

template<typename PriceType, typename OrderIdType>
void OrderBook<PriceType, OrderIdType>::print_book() const {
    std::cout << "\n=== ORDER BOOK ===\n";

    std::cout << "\n--- Buy Orders (Highest first) ---\n";
    for (auto it = buy_orders.rbegin(); it != buy_orders.rend(); ++it) {
        auto& order = it->second;
        std::cout << "ID: " << order->id
                  << " | Price: " << order->price
                  << " | Qty: " << order->quantity << '\n';
    }

    std::cout << "\n--- Sell Orders (Lowest first) ---\n";
    for (const auto& [price, order] : sell_orders) {
        std::cout << "ID: " << order->id
                  << " | Price: " << order->price
                  << " | Qty: " << order->quantity << '\n';
    }
    std::cout << "===================\n";
}

// Explicitly instantiate templates for common types
template class OrderBook<double, int>;