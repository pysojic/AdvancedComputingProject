// market_snapshot.cpp
#include "market_snapshot.h"
#include <iostream>

// Update the bid side of the order book at the given price.
// If quantity is non-zero, set or update that price level's quantity.
// If quantity is zero, remove the price level from the bid map.
void MarketSnapshot::updateBid(double price, int quantity) {
    if (quantity == 0) {
        // Remove price level if it exists
        auto it = bids.find(price);
        if (it != bids.end()) {
            bids.erase(it);
        }
        return;
    }
    // If quantity > 0, update or insert the price level
    auto it = bids.find(price);
    if (it != bids.end()) {
        // Update existing level's quantity
        it->second->quantity = quantity;
    } else {
        // Insert new price level with given price and quantity
        bids[price] = std::make_unique<PriceLevel>(price, quantity);
    }
}

// Update the ask side of the order book at the given price.
// If quantity is non-zero, set or update that price level's quantity.
// If quantity is zero, remove the price level from the ask map.
void MarketSnapshot::updateAsk(double price, int quantity) {
    if (quantity == 0) {
        // Remove price level if it exists
        auto it = asks.find(price);
        if (it != asks.end()) {
            asks.erase(it);
        }
        return;
    }
    // If quantity > 0, update or insert the price level
    auto it = asks.find(price);
    if (it != asks.end()) {
        // Update existing level's quantity
        it->second->quantity = quantity;
    } else {
        // Insert new price level with given price and quantity
        asks[price] = std::make_unique<PriceLevel>(price, quantity);
    }
}

// Get the best bid (highest price) in the order book.
// Returns a pointer to a PriceLevel struct or nullptr if no bids.
const PriceLevel* MarketSnapshot::getBestBid() const {
    if (bids.empty()) {
        return nullptr;
    }
    // Since bids are sorted in descending order by price, begin() gives highest price.
    return bids.begin()->second.get();
}

// Get the best ask (lowest price) in the order book.
// Returns a pointer to a PriceLevel struct or nullptr if no asks.
const PriceLevel* MarketSnapshot::getBestAsk() const {
    if (asks.empty()) {
        return nullptr;
    }
    // Asks are sorted in ascending order by price (default std::less), begin() gives lowest price.
    return asks.begin()->second.get();
}
