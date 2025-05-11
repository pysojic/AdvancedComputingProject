# pragma once

#include <iostream>

struct Order {
    std::string id;  // String-based order ID
    double price;
    int quantity;
    bool isBuy;
};