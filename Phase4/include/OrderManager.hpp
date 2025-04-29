#pragma once
#include "OrderBook.hpp"
#include <memory>
#include <type_traits>
#include "config.h"

template<typename PriceType, typename OrderIdType>
class OrderManager {
    static_assert(std::is_arithmetic<PriceType>::value, "PriceType must be numeric");
    static_assert(std::is_integral<OrderIdType>::value, "OrderIdType must be integral");    
public:
    explicit OrderManager(OrderBook<PriceType, OrderIdType>& book);

    void submit_order(std::shared_ptr<Order<PriceType, OrderIdType>> order);
    void cancel_order(OrderIdType id);

private:
    OrderBook<PriceType, OrderIdType>& order_book;
};

