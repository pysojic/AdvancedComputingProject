// order_manager.h
#ifndef ORDER_MANAGER_H
#define ORDER_MANAGER_H

#include <map>
#include <memory>
#include <iostream>

class OrderManager {
public:
    // Side of order (Buy or Sell)
    enum class Side { Buy, Sell };

    // Represents an order placed by our trading strategy.
    struct MyOrder {
        int id;
        Side side;
        double price;
        int quantity;
        int filled;  // how much quantity has been executed so far
        MyOrder(int id_, Side side_, double price_, int quantity_)
            : id(id_), side(side_), price(price_), quantity(quantity_), filled(0) {}
    };

private:
    // Track active orders by ID using unique_ptr for memory safety.
    std::map<int, std::unique_ptr<MyOrder>> orders;
    int nextOrderId = 1;

public:
    // Place a new order and return its unique ID.
    // The order is stored and managed by the OrderManager.
    int placeOrder(Side side, double price, int quantity);

    // Cancel an existing order by ID. Returns true if found and canceled.
    // This removes the order from the manager (and frees its memory).
    bool cancelOrder(int orderId);

    // Handle an execution fill for a given order ID and quantity.
    // Updates the order's filled amount and removes the order if fully filled.
    void handleExecution(int orderId, int fillQuantity);

    // Check if an order with given ID is currently active (exists in the map).
    bool hasOrder(int orderId) const;

    // Check if no active orders remain.
    bool empty() const;

    // Cancel all active orders. This will remove all orders from the manager.
    void cancelAll();
};

#endif // ORDER_MANAGER_H
