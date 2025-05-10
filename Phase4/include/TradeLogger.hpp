#pragma once
#include <string>
#include <vector>
#include <chrono>
#include "config.h"

class TradeLogger {
public:
    TradeLogger(size_t batchSize = 64);
    ~TradeLogger();

    void log_trade(const std::string& symbol, double price, int quantity);
    void flush();  // force‐write pending trades

private:
    struct Trade { std::string sym; double pr; int qty; long long ts; };
    std::vector<Trade> m_buffer;
    size_t             m_batchSize;
};
