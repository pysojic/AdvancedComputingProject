#include "../include/LatencyAnalyzer.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <numeric>

void analyzeLatencies(const std::vector<long long>& latencies) {
    if(latencies.empty()) return;
    auto minv = *std::min_element(latencies.begin(), latencies.end());
    auto maxv = *std::max_element(latencies.begin(), latencies.end());
    double mean = std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();

    double var = 0;
    for(auto l: latencies) var += (l-mean)*(l-mean);
    double stddev = std::sqrt(var/latencies.size());

    size_t idx_p99 = static_cast<size_t>(latencies.size()*0.99);
    std::vector<long long> sorted = latencies;
    std::nth_element(sorted.begin(), sorted.begin()+idx_p99, sorted.end());
    long long p99 = sorted[idx_p99];

    std::cout << "\nTick-to-Trade Latency (ns):\n"
              << "Min: "   << minv  << "\n"
              << "Max: "   << maxv  << "\n"
              << "Mean: "  << mean  << "\n"
              << "StdDev: "<< stddev<< "\n"
              << "P99: "   << p99   << "\n";
}
