#define _CRT_SECURE_NO_WARNINGS
#include "config.h"
#include <iostream>
#include <memory>
#include <vector>
#include <numeric>
#include <algorithm>
#include <string>
#include <cstring>
#include <cstdio>

#include "../include/Order.hpp"
#include "../include/OrderBook.hpp"
#include "../include/OrderManager.hpp"
#include "../include/MatchingEngine.hpp"
#include "../include/TradeLogger.hpp"
#include "../include/LatencyAnalyzer.hpp"
#include "../include/MarketData.hpp"
#include "../include/MemoryPool.hpp"
#include "../include/Timer.hpp"

using PriceT  = double;
using OrderID = int;
using OrderT  = Order<PriceT,OrderID>;

int main(int argc, char** argv) {
    // 1) configure‐time defaults
    int         num_ticks = CFG_DEFAULT_TICKS;
    std::string ptrmode   = CFG_DEFAULT_PTRMODE;
    bool        usePool   = (CFG_ENABLE_POOL != 0);
    std::string layout    = CFG_DEFAULT_LAYOUT;

    // 2) command‐line overrides
    for(int i = 1; i < argc; ++i) {
        if (std::sscanf(argv[i], "--ticks=%d", &num_ticks) == 1)     continue;
        if (std::strncmp(argv[i], "--ptrmode=", 10) == 0) {
            ptrmode = argv[i] + 10;     continue;
        }
        if (std::strcmp(argv[i], "--pool=on") == 0) {
            usePool = true;  continue;
        }
        if (std::strcmp(argv[i], "--pool=off") == 0) {
            usePool = false; continue;
        }
        if (std::strncmp(argv[i], "--layout=", 9) == 0) {
            layout = argv[i] + 9;       continue;
        }
    }

    std::cout << "CONFIG: ticks=" << num_ticks
              << " ptrmode="   << ptrmode
              << " pool="      << (usePool ? "on" : "off")
              << " layout="    << layout << "\n";

    // core modules
    OrderBook<PriceT,OrderID>     book;
    OrderManager<PriceT,OrderID>  mgr(book);
    MatchingEngine<PriceT,OrderID> engine(book);
    TradeLogger                   logger;
    MarketDataFeed                feed;

    std::vector<long long> latencies;
    latencies.reserve(num_ticks);

    // declare pnl outside loop
    double pnl = 0.0;

    // pool for orders
    MemoryPool pool(sizeof(OrderT), num_ticks);

    for(int i = 0; i < num_ticks; ++i) {
        Timer timer; timer.start();

        auto md = feed.generate_mock_data("AAPL");

        std::shared_ptr<OrderT> orderPtr;
        if (ptrmode == "shared") {
            orderPtr = std::make_shared<OrderT>(
                i, "AAPL", md.bid_price, 100, (i % 2) == 0
            );
        }
        else if (ptrmode == "pool" && usePool) {
            void* raw = pool.allocate();
            orderPtr = std::shared_ptr<OrderT>(
              new(raw) OrderT(i, "AAPL", md.bid_price, 100, (i % 2) == 0),
              [&](OrderT* p){ p->~OrderT(); pool.deallocate(p); }
            );
        }
        else if (ptrmode == "raw") {
            orderPtr = std::shared_ptr<OrderT>(
              new OrderT(i, "AAPL", md.bid_price, 100, (i % 2) == 0),
              [](OrderT* p){ delete p; }
            );
        } else {
            orderPtr = std::make_shared<OrderT>(
                i, "AAPL", md.bid_price, 100, (i % 2) == 0
            );
        }

        mgr.submit_order(orderPtr);
        engine.match_orders(logger);
        // now get trades back so we can log and PnL
        // auto trades = engine.match_orders(logger);
        // for (auto &t : trades) {
        //     if (t.is_buy) {
        //         std::cout << "[BUY ] " << t.quantity << " @ " << t.price << "\n";
        //         pnl -= t.price * t.quantity;
        //     } else {
        //         std::cout << "[SELL] " << t.quantity << " @ " << t.price << "\n";
        //         pnl += t.price * t.quantity;
        //     }
        // }

        latencies.push_back(timer.stop());
    }

    analyzeLatencies(latencies);
    std::cout << "\n=== Realized PnL: " << pnl << " ===\n";
    return 0;
}
