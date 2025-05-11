#include <iostream>
#include <queue>
#include <vector>
#include <random>
#include <chrono>
#include "order_pq.h"

int main() {
    const size_t n = 100000;
    std::vector<Order> orders;
    orders.reserve(n);
    std::mt19937_64 rng(42);
    std::uniform_real_distribution<double> dist(1.0, 100.0);
    for (size_t i = 0; i < n; ++i)
        orders.push_back(Order{dist(rng), static_cast<int>(i)});

    OrderPriorityQueue custom;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (auto& o : orders) custom.push(o);
    auto t1 = std::chrono::high_resolution_clock::now();
    while (!custom.empty()) custom.pop();
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Custom PQ push ns/op: "
              << (t1 - t0).count() / n << "\n";
    std::cout << "Custom PQ pop ns/op: "
              << (t2 - t1).count() / n << "\n";

    std::priority_queue<Order> stdpq;
    t0 = std::chrono::high_resolution_clock::now();
    for (auto& o : orders) stdpq.push(o);
    t1 = std::chrono::high_resolution_clock::now();
    while (!stdpq.empty()) stdpq.pop();
    t2 = std::chrono::high_resolution_clock::now();

    std::cout << "std::priority_queue push ns/op: "
              << (t1 - t0).count() / n << "\n";
    std::cout << "std::priority_queue pop ns/op: "
              << (t2 - t1).count() / n << "\n";

    return 0;
}
