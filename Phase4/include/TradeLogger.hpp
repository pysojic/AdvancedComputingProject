#pragma once
#include <string>

class TradeLogger {
public:
    void log_trade(const std::string& symbol, double price, int quantity);
};