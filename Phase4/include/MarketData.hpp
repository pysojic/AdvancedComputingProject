#include <iostream>
#include <map>

struct alignas(64) MarketData {
    std::string symbol;
    double bid_price;
    int bid_size;
    double ask_price;
    int ask_size;
    std::chrono::high_resolution_clock::time_point timestamp;
};

class MarketDataHandler {
    public:
        void handleTick(MarketData tick);
    
    private:

};