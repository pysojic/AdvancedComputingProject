#include "../include/orderbook.hpp"
#include "../include/optimized_orderbook.hpp"
#include "../include/benchmark.hpp"
#include "../include/unit_tests.hpp"

int main()
{   
    // Run unit tests
    OrderBook classic_ob;
    OptimizedOrderBook optimized_ob;
    OrderBookTests::runTests<OrderBook>();
    OrderBookTests::runTests<OptimizedOrderBook>();
    std::cout << "All tests passed!" << std::endl;

    stressTest(optimized_ob, 100);

    // Run benchmark
    std::cout << "Running benchmark..." << std::endl;

    benchmark(1000);
    benchmark(5000);
    benchmark(10000);
    benchmark(50000);
    benchmark(100000);

    return 0;
}