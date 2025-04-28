#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include "../include/Order.hpp"
#include "../include/Timer.hpp"

using OrderType = Order<double, int>;

int main() {
    std::vector<long long> latencies;
    const int num_ticks = 10000;

    for (int i = 0; i < num_ticks; ++i) {
        Timer timer;
        timer.start();

        // Simulated tick + order match (replace with real logic)
        OrderType order(i, "AAPL", 150.0 + (i % 5), 100, i % 2 == 0);
        // simulate match logic here

        latencies.push_back(timer.stop());
    }

    // Analyze latency
    auto min = *std::min_element(latencies.begin(), latencies.end());
    auto max = *std::max_element(latencies.begin(), latencies.end());
    double mean = std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();

    std::cout << "Tick-to-Trade Latency (nanoseconds):\n";
    std::cout << "Min: " << min << " | Max: " << max << " | Mean: " << mean << '\n';
}