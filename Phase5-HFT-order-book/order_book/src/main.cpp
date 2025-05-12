#include "../include/orderbook.hpp"
#include "../include/optimized_orderbook.hpp"
#include "../include/benchmark.hpp"
#include "../include/unit_tests.hpp"

#include <random>

template <typename T>
void stressTest(T& book, int numOrders) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> priceDist(50.0, 100.0);
    std::uniform_int_distribution<int> quantityDist(1, 500);

    for (int i = 0; i < numOrders; ++i) {
        std::string id = "ORD" + std::to_string(i);
        double price = priceDist(rng);
        int quantity = quantityDist(rng);

        book.addOrder(id, price, quantity, true);
    }
}

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