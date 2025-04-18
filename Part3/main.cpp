#include <iostream>

#include "include/market_snapshot.h"

int main()
{
    MarketSnapshot ms;

    ms.update_ask(38, 90);
    ms.update_ask(50, 92);
    ms.update_ask(49, 10);
    ms.update_ask(40, 5);

    ms.update_bid(24, 4);
    ms.update_bid(25, 41);
    ms.update_bid(24, 5);
    ms.update_bid(35, 1);

    std::cout << ms.get_best_bid().m_Price << std::endl;
    std::cout << ms.get_best_ask().m_Price << std::endl;
}