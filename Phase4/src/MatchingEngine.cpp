#include "../include/MatchingEngine.hpp"
#include <iostream>

template<typename PriceType, typename OrderIdType>
MatchingEngine<PriceType, OrderIdType>::MatchingEngine(OrderBook<PriceType, OrderIdType>& book)
    : order_book(book) {}

template<typename PriceType, typename OrderIdType>
void MatchingEngine<PriceType, OrderIdType>::match_orders(TradeLogger& logger) {
    while (!order_book.buy_orders.empty() && !order_book.sell_orders.empty()) {
        auto best_buy = order_book.buy_orders.rbegin()->second;
        auto best_sell = order_book.sell_orders.begin()->second;

        if (best_buy->price >= best_sell->price) {
            int trade_quantity = std::min(best_buy->quantity, best_sell->quantity);
            logger.log_trade(best_buy->symbol, best_sell->price, trade_quantity);

            best_buy->quantity -= trade_quantity;
            best_sell->quantity -= trade_quantity;

            if (best_buy->quantity == 0) {
                order_book.buy_orders.erase(std::prev(order_book.buy_orders.end()));
            }
            if (best_sell->quantity == 0) {
                order_book.sell_orders.erase(order_book.sell_orders.begin());
            }
        } else {
            break;
        }
    }
}

// Explicitly instantiate
template class MatchingEngine<double, int>;