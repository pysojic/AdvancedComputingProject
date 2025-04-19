#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "market_snapshot.h"
#include "order_manager.h"

int main(int argc, char* argv[]) {
    // Set up the market data snapshot and order manager
    MarketSnapshot market;
    OrderManager orderManager;

    // Strategy thresholds for demonstration
    const double buyThreshold = 100.0;
    const double sellThreshold = 110.0;

    // Open the feed file if provided as argument or default to sample_feed.txt
    std::istream* input = nullptr;
    std::ifstream file;
    if (argc > 1) {
        file.open(argv[1]);
        if (!file) {
            std::cerr << "Failed to open file: " << argv[1] << std::endl;
            return 1;
        }
        input = &file;
    } else {
        // Try default sample_feed.txt
        file.open("sample_feed.txt");
        if (file) {
            input = &file;
        } else {
            // Fallback to standard input
            input = &std::cin;
            if (!std::cin.good()) {
                std::cerr << "No input feed provided." << std::endl;
                return 1;
            }
        }
    }

    std::string line;
    // Read the feed line by line
    while (std::getline(*input, line)) {
        if (line.empty()) continue;
        // Print the feed line for logging
        std::cout << ">> " << line << std::endl;
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "BID" || type == "ASK") {
            double price;
            int quantity;
            iss >> price >> quantity;
            if (type == "BID") {
                market.updateBid(price, quantity);
                if (quantity == 0) {
                    std::cout << "Removed BID level " << price;
                } else {
                    std::cout << "Updated BID " << price << " -> qty " << quantity;
                }
            } else { // "ASK"
                market.updateAsk(price, quantity);
                if (quantity == 0) {
                    std::cout << "Removed ASK level " << price;
                } else {
                    std::cout << "Updated ASK " << price << " -> qty " << quantity;
                }
            }
            // After updating, log the current best bid and ask
            const PriceLevel* bestBid = market.getBestBid();
            const PriceLevel* bestAsk = market.getBestAsk();
            std::cout << ". Best Bid=";
            if (bestBid) {
                std::cout << bestBid->price << " (qty " << bestBid->quantity << ")";
            } else {
                std::cout << "N/A";
            }
            std::cout << ", Best Ask=";
            if (bestAsk) {
                std::cout << bestAsk->price << " (qty " << bestAsk->quantity << ")";
            } else {
                std::cout << "N/A";
            }
            std::cout << std::endl;
            // Strategy: place orders based on thresholds if no order is currently open
            if (bestAsk && orderManager.empty() && bestAsk->price <= buyThreshold) {
                // Best ask is at or below threshold -> place a buy order
                orderManager.placeOrder(OrderManager::Side::Buy, bestAsk->price, 10);
            } else if (bestBid && orderManager.empty() && bestBid->price >= sellThreshold) {
                // Best bid is at or above threshold -> place a sell order
                orderManager.placeOrder(OrderManager::Side::Sell, bestBid->price, 5);
            }
        } else if (type == "EXECUTION") {
            int orderId;
            int fillQty;
            iss >> orderId >> fillQty;
            // Handle an execution fill event for an order
            orderManager.handleExecution(orderId, fillQty);
        } else {
            // Unknown event type (skip or log)
            std::cerr << "Unknown feed event type: " << type << std::endl;
        }
    }

    // After processing all feed events, cancel any remaining open orders (cleanup)
    if (!orderManager.empty()) {
        std::cout << "End of feed. Canceling any open orders." << std::endl;
        orderManager.cancelAll();
    }

    return 0;
}
