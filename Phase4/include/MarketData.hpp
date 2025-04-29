#pragma once
#include <string>
#include <chrono>

struct alignas(64) MarketData {
    std::string symbol;
    double bid_price;
    double ask_price;
    std::chrono::high_resolution_clock::time_point timestamp;
};

class MarketDataFeed {
public:
    MarketData generate_mock_data(const std::string& symbol);
};