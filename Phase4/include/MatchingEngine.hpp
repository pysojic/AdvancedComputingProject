#pragma once
#include "OrderBook.hpp"
#include "TradeLogger.hpp"

template<typename PriceType, typename OrderIdType>
class MatchingEngine {
public:
    explicit MatchingEngine(OrderBook<PriceType, OrderIdType>& book);

    void match_orders(TradeLogger& logger);

private:
    OrderBook<PriceType, OrderIdType>& order_book;
};