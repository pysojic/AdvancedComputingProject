#include "../include/MarketData.hpp"

#include <random>

MarketData MarketDataFeed::generate_mock_data(const std::string& symbol) {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<double> price_dist(100.0, 200.0);

    double mid_price = price_dist(rng);
    double spread = 0.01;

    MarketData data;
    data.symbol = symbol;
    data.bid_price = mid_price - spread;
    data.ask_price = mid_price + spread;
    data.timestamp = std::chrono::high_resolution_clock::now();
    
    return data;
}