// market_snapshot.h
#ifndef MARKET_SNAPSHOT_H
#define MARKET_SNAPSHOT_H

#include <map>
#include <memory>
#include <functional>

// Represents a single price level in the order book
struct PriceLevel {
    double price;
    int quantity;
    PriceLevel(double p, int q) : price(p), quantity(q) {}
};

// MarketSnapshot maintains current bid and ask price levels (the order book snapshot)
class MarketSnapshot {
private:
    // Bid side (price -> PriceLevel) sorted highest-to-lowest price using std::greater comparator
    std::map<double, std::unique_ptr<PriceLevel>, std::greater<double>> bids;
    // Ask side (price -> PriceLevel) sorted lowest-to-highest (default std::less<double>)
    std::map<double, std::unique_ptr<PriceLevel>> asks;
public:
    // Update the bid side for a given price. If quantity is 0, removes that price level.
    void updateBid(double price, int quantity);
    // Update the ask side for a given price. If quantity is 0, removes that price level.
    void updateAsk(double price, int quantity);
    // Get the best (highest) bid level. Returns nullptr if no bids present.
    const PriceLevel* getBestBid() const;
    // Get the best (lowest) ask level. Returns nullptr if no asks present.
    const PriceLevel* getBestAsk() const;
};

#endif // MARKET_SNAPSHOT_H
