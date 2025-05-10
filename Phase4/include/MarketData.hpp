#pragma once
#include "config.h"
#include <string>
#include <chrono>

#if CFG_ENABLE_ALIGN64
struct alignas(64) MarketData {
#else
struct MarketData {
#endif
    std::string                                      symbol;
    double                                           bid_price;
    double                                           ask_price;
    std::chrono::high_resolution_clock::time_point   timestamp;
};

class MarketDataFeed {
public:
    MarketData generate_mock_data(const std::string& symbol);
};
