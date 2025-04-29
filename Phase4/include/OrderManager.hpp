#pragma once
#include "OrderBook.hpp"
#include <memory>
#include <type_traits>

template<typename PriceType, typename OrderIdType>
class OrderManager {
public:
    explicit OrderManager(OrderBook<PriceType, OrderIdType>& book);

    void submit_order(std::shared_ptr<Order<PriceType, OrderIdType>> order);
    void cancel_order(OrderIdType id);

private:
    OrderBook<PriceType, OrderIdType>& order_book;
};