#include <iostream>
#include <memory>
#include "../include/Order.hpp"
#include "../include/OrderBook.hpp"
#include "../include/OrderManager.hpp"
#include "../include/MatchingEngine.hpp"
#include "../include/TradeLogger.hpp"

using PriceT = double;
using OrderID = int;
using OrderType = Order<PriceT, OrderID>;

int main() {
    OrderBook<PriceT, OrderID> book;
    OrderManager<PriceT, OrderID> manager(book);
    MatchingEngine<PriceT, OrderID> engine(book);
    TradeLogger logger;

    const std::string symbol = "AAPL";

    // Submit some buy orders
    manager.submit_order(std::make_shared<OrderType>(1, symbol, 150.0, 100, true)); // Buy 100 @ 150
    manager.submit_order(std::make_shared<OrderType>(2, symbol, 151.0, 50, true));  // Buy 50 @ 151

    // Submit some sell orders
    manager.submit_order(std::make_shared<OrderType>(3, symbol, 149.0, 70, false)); // Sell 70 @ 149
    manager.submit_order(std::make_shared<OrderType>(4, symbol, 152.0, 30, false)); // Sell 30 @ 152

    std::cout << "\n--- Matching Phase 1 ---\n";
    engine.match_orders(logger);
    book.print_book();

    // Submit more orders to see partial fills
    manager.submit_order(std::make_shared<OrderType>(5, symbol, 150.0, 100, false)); // Sell 100 @ 151

    std::cout << "\n--- Matching Phase 2 ---\n";
    engine.match_orders(logger);
    book.print_book();

    return 0;
}