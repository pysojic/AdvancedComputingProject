#include "../include/MatchingEngine.hpp"
#include <iostream>
#include "config.h"

template<typename PriceType, typename OrderIdType>
MatchingEngine<PriceType, OrderIdType>::MatchingEngine(OrderBook<PriceType, OrderIdType>& book)
    : order_book(book) {}

template<typename P, typename I>
std::vector<PnLTrade> MatchingEngine<P,I>::match_orders(TradeLogger& logger) {
    std::vector<PnLTrade> trades;
    while (!order_book.buy_orders.empty() && !order_book.sell_orders.empty()) {
        auto best_buy  = order_book.buy_orders.rbegin()->second;
        auto best_sell = order_book.sell_orders.begin()->second;
        if (best_buy->price >= best_sell->price) {
            int qty = std::min(best_buy->quantity, best_sell->quantity);
            double price = best_sell->price;
            logger.log_trade(best_buy->symbol, price, qty);
            // our side: best_buy was a BUY, best_sell was a SELL
            trades.push_back({ true,  price, qty });   // we bought
            trades.push_back({ false, price, qty });   // we sold

            best_buy->quantity  -= qty;
            best_sell->quantity -= qty;
            if (best_buy->quantity  == 0) order_book.buy_orders .erase(std::prev(order_book.buy_orders.end()));
            if (best_sell->quantity == 0) order_book.sell_orders.erase(order_book.sell_orders.begin());
        } else break;
    }
    return trades;
}

// Explicitly instantiate
template class MatchingEngine<double, int>;