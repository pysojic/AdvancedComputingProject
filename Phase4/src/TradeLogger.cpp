#include "../include/TradeLogger.hpp"
#include <iostream>
#include "config.h"

TradeLogger::TradeLogger(size_t batchSize)
 : m_batchSize(batchSize)
{ m_buffer.reserve(batchSize); }

TradeLogger::~TradeLogger() {
    flush();
}

void TradeLogger::log_trade(const std::string& symbol, double price, int quantity) {
    auto ts = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
    m_buffer.push_back({symbol, price, quantity, ts});

    if(m_buffer.size() >= m_batchSize) flush();
}

void TradeLogger::flush() {
    for(auto &t : m_buffer) {
        std::cout << "TRADE | "
                  << t.sym << " @ "<< t.pr
                  << " x"<< t.qty
                  << " | ts="<< t.ts << "\n";
    }
    m_buffer.clear();
}
