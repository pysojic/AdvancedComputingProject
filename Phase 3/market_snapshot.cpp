#include "market_snapshot.h"
#include <iostream>

void MarketSnapshot::updateBid(double price, int quantity) {
    if (quantity == 0) {
        auto it = bids.find(price);
        if (it != bids.end()) {
            bids.erase(it);
        }
        return;
    }
    auto it = bids.find(price);
    if (it != bids.end()) {
        it->second->quantity = quantity;
    } else {
        bids[price] = std::make_unique<PriceLevel>(price, quantity);
    }
}


void MarketSnapshot::updateAsk(double price, int quantity) {
    if (quantity == 0) {
        auto it = asks.find(price);
        if (it != asks.end()) {
            asks.erase(it);
        }
        return;
    }
    auto it = asks.find(price);
    if (it != asks.end()) {
        it->second->quantity = quantity;
    } else {
        asks[price] = std::make_unique<PriceLevel>(price, quantity);
    }
}

const PriceLevel* MarketSnapshot::getBestBid() const {
    if (bids.empty()) {
        return nullptr;
    }
    return bids.begin()->second.get();
}

const PriceLevel* MarketSnapshot::getBestAsk() const {
    if (asks.empty()) {
        return nullptr;
    }
    return asks.begin()->second.get();
}
