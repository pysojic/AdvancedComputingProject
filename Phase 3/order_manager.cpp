// order_manager.cpp
#include "order_manager.h"

// Place a new order with given side, price, and quantity.
// Uses a unique order ID and stores the order in the map with a unique_ptr.
int OrderManager::placeOrder(Side side, double price, int quantity) {
    int orderId = nextOrderId++;
    // Create a new order and store it in the map
    orders[orderId] = std::make_unique<MyOrder>(orderId, side, price, quantity);
    // Log the order placement
    std::cout << "Placed order " << orderId << ": "
              << (side == Side::Buy ? "Buy" : "Sell")
              << " " << quantity << " @ " << price << std::endl;
    return orderId;
}

// Cancel an active order by its ID. Returns true if the order was found and canceled.
bool OrderManager::cancelOrder(int orderId) {
    auto it = orders.find(orderId);
    if (it == orders.end()) {
        return false; // Order not found or already filled/canceled
    }
    // Log the cancellation (if partially filled, some quantity remains unfilled)
    MyOrder* order = it->second.get();
    int remaining = order->quantity - order->filled;
    std::cout << "Canceled order " << orderId;
    if (order->filled > 0) {
        std::cout << " (remaining " << remaining << " canceled)";
    }
    std::cout << std::endl;
    // Erase the order from map, unique_ptr will automatically free the memory.
    orders.erase(it);
    return true;
}

// Handle an execution event for a given order ID and filled quantity.
// Updates the order's filled amount. If the order is completely filled, it is removed.
void OrderManager::handleExecution(int orderId, int fillQuantity) {
    auto it = orders.find(orderId);
    if (it == orders.end()) {
        // Unknown order ID (could be already filled/canceled or invalid)
        std::cout << "Warning: Execution for unknown order ID " << orderId << std::endl;
        return;
    }
    MyOrder* order = it->second.get();
    // Update filled quantity
    order->filled += fillQuantity;
    if (order->filled >= order->quantity) {
        // Order fully filled (total executed equals original quantity)
        std::cout << "Order " << orderId << " fully filled (total " << order->quantity << " executed)" << std::endl;
        // Remove the order from active list, freeing the memory.
        orders.erase(it);
    } else {
        // Order partially filled, still has remaining quantity
        int remaining = order->quantity - order->filled;
        std::cout << "Order " << orderId << " partially filled: " 
                  << fillQuantity << " executed, " << remaining << " remaining" << std::endl;
    }
}

// Check if an order ID exists in the active orders map.
bool OrderManager::hasOrder(int orderId) const {
    return orders.find(orderId) != orders.end();
}

// Check if no active orders remain.
bool OrderManager::empty() const {
    return orders.empty();
}

// Cancel all active orders, freeing their memory.
void OrderManager::cancelAll() {
    // Iterate through all orders and cancel them.
    // Use a loop erasing one by one to safely invalidate iterators.
    auto it = orders.begin();
    while (it != orders.end()) {
        int id = it->first;
        // Move iterator to next before erasing current
        it++;
        cancelOrder(id);
    }
}
