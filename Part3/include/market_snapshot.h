#pragma once

#include <map>
#include <memory>

struct PriceLevel 
{
    PriceLevel(double p, int q) : m_Price(p), m_Quantity(q) {}

    double m_Price;
    int m_Quantity;
};

class MarketSnapshot 
{
public:
    void update_bid(double price, int qty) 
    { 
        if (m_Bids.contains(price))
            m_Bids[price]->m_Quantity += qty;
        m_Bids.try_emplace(price, std::make_unique<PriceLevel>(price, qty));
    }
    void update_ask(double price, int qty) 
    { 
        if (m_Asks.contains(price))
            m_Asks[price]->m_Quantity += qty;
        m_Asks.try_emplace(price, std::make_unique<PriceLevel>(price, qty));
    }
    const PriceLevel& get_best_ask() const { return *m_Asks.begin()->second; }
    const PriceLevel& get_best_bid() const { return *m_Bids.begin()->second; }

private:
std::map<double, std::unique_ptr<PriceLevel>> m_Asks; // sorted ascending
std::map<double, std::unique_ptr<PriceLevel>, std::greater<>> m_Bids; // sorted descending
};