#pragma once
#include "OrderBook.hpp"
#include "TradeLogger.hpp"
#include "config.h"

struct PnLTrade {
    bool   is_buy;
    double price;
    int    quantity;
  };

template<typename PriceType, typename OrderIdType>
class MatchingEngine {
    static_assert(std::is_arithmetic<PriceType>::value, "PriceType must be numeric");
    static_assert(std::is_integral<OrderIdType>::value, "OrderIdType must be integral");
public:
    explicit MatchingEngine(OrderBook<PriceType, OrderIdType>& book);

    std::vector<PnLTrade> match_orders(TradeLogger& logger);

private:
    OrderBook<PriceType, OrderIdType>& order_book;
};

