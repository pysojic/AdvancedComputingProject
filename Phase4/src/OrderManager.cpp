#include "../include/OrderManager.hpp"

int OrderManager::placeOrder(Side side, double price, int quantity) {
    int orderId = nextOrderId++;
    orders[orderId] = std::make_unique<MyOrder>(orderId, side, price, quantity);
    std::cout << "Placed order " << orderId << ": "
              << (side == Side::Buy ? "Buy" : "Sell")
              << " " << quantity << " @ " << price << std::endl;
    return orderId;
}

bool OrderManager::cancelOrder(int orderId) {
    auto it = orders.find(orderId);
    if (it == orders.end()) {
        return false; 
    }
    MyOrder* order = it->second.get();
    int remaining = order->quantity - order->filled;
    std::cout << "Canceled order " << orderId;
    if (order->filled > 0) {
        std::cout << " (remaining " << remaining << " canceled)";
    }
    std::cout << std::endl;
    orders.erase(it);
    return true;
}


void OrderManager::handleExecution(int orderId, int fillQuantity) {
    auto it = orders.find(orderId);
    if (it == orders.end()) {
        std::cout << "Warning: Execution for unknown order ID " << orderId << std::endl;
        return;
    }
    MyOrder* order = it->second.get();
    order->filled += fillQuantity;
    if (order->filled >= order->quantity) {
        std::cout << "Order " << orderId << " fully filled (total " << order->quantity << " executed)" << std::endl;
        orders.erase(it);
    } else {
        int remaining = order->quantity - order->filled;
        std::cout << "Order " << orderId << " partially filled: " 
                  << fillQuantity << " executed, " << remaining << " remaining" << std::endl;
    }
}

bool OrderManager::hasOrder(int orderId) const {
    return orders.find(orderId) != orders.end();
}

bool OrderManager::empty() const {
    return orders.empty();
}

void OrderManager::cancelAll() {
    auto it = orders.begin();
    while (it != orders.end()) {
        int id = it->first;
        it++;
        cancelOrder(id);
    }
}