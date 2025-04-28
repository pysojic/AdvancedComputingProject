#pragma once
#include <string>
#include <memory>

enum class Side { Buy, Sell };

template <typename PriceType, typename OrderIdType>
struct Order {
    OrderIdType id;
    std::string symbol;
    PriceType price;
    int quantity;
    Side side;

    Order(OrderIdType id, std::string sym, PriceType pr, int qty, Side side)
        : id(id), symbol(std::move(sym)), price(pr), quantity(qty), side(side) {}
};