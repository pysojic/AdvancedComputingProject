#include <iostream>
#include <memory>
#include <vector>
#include <numeric>
#include <algorithm>
#include <string>                           // <— add
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

int main() {
    OrderBook<PriceT,OrderID> book;
    OrderManager<PriceT,OrderID> mgr(book);
    MatchingEngine<PriceT,OrderID> engine(book);
    TradeLogger logger;
    MarketDataFeed feed;

    std::vector<long long> latencies;
    const int num_ticks     = 10000;
    const std::string symbol = "AAPL";

    // memory-pool for Orders
    MemoryPool pool(sizeof(OrderT), num_ticks);

    for(int i = 0; i < num_ticks; ++i) {
        Timer timer;
        timer.start();

        // simulate tick
        auto md = feed.generate_mock_data(symbol);

        // allocate and construct an OrderT in our pool
        void* raw = pool.allocate();
        auto order = std::shared_ptr<OrderT>(
            new(raw) OrderT(
                i,
                symbol,
                md.bid_price,
                100,
                (i % 2) == 0
            ),
            [&](OrderT* p){
                p->~OrderT();
                pool.deallocate(p);
            }
        );
        mgr.submit_order(order);

        // match & log
        engine.match_orders(logger);
        latencies.push_back(timer.stop());
    }

    analyzeLatencies(latencies);
    return 0;
}
