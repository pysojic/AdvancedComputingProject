#ifndef MARKET_SNAPSHOT_H
#define MARKET_SNAPSHOT_H

#include <map>
#include <memory>
#include <functional>

struct PriceLevel {
    double price;
    int quantity;
    PriceLevel(double p, int q) : price(p), quantity(q) {}
};

class MarketSnapshot {
private:
    std::map<double, std::unique_ptr<PriceLevel>, std::greater<double>> bids;
    std::map<double, std::unique_ptr<PriceLevel>> asks;
public:
    void updateBid(double price, int quantity);
    void updateAsk(double price, int quantity);
    const PriceLevel* getBestBid() const;
    const PriceLevel* getBestAsk() const;
};

#endif // MARKET_SNAPSHOT_H
