#pragma once
#include "Order.hpp"
#include <map>
#include <memory>
#include <iostream>

template<typename PriceType, typename OrderIdType>
class OrderBook {
    public:
        OrderBook();

        void add_order(std::shared_ptr<Order<PriceType, OrderIdType>> order);
        void remove_order(OrderIdType id);
        void print_book() const;

        std::multimap<PriceType, std::shared_ptr<Order<PriceType, OrderIdType>>> buy_orders;
        std::multimap<PriceType, std::shared_ptr<Order<PriceType, OrderIdType>>> sell_orders;
};