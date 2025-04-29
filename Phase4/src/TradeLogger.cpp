#include "../include/TradeLogger.hpp"

#include <iostream>

void TradeLogger::log_trade(const std::string& symbol, double price, int quantity) {
    auto timestamp = std::chrono::high_resolution_clock::now();
    std::cout << "Trade Executed -> "
              << "Symbol: " << symbol
              << " | Price: " << price
              << " | Quantity: " << quantity
              << " | Time: " 
              << std::chrono::duration_cast<std::chrono::nanoseconds>(timestamp.time_since_epoch()).count()
              << " ns" << std::endl;
}