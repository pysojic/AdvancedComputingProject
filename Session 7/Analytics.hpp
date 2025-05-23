// Analytics.hpp
#pragma once
#include <algorithm>
#include <execution>
#include <iostream>
#include <numeric>
#include "OrderProcessing.hpp"

// ---- Latency histogram & average ----
inline void printLatencyHistogram(
  const std::vector<double>& lat)
{
  if (lat.empty()) return;
  auto [minIt,maxIt] = 
     std::minmax_element(lat.begin(), lat.end());
  double mn = *minIt, mx = *maxIt;
  int bins = 10;
  std::vector<int> counts(bins);
  for (double v : lat) {
    int idx = std::min(
      int((v - mn)/(mx - mn + 1e-12)*bins),
      bins-1);
    counts[idx]++;
  }
  std::cout << "\nLatency histogram (µs):\n";
  for (int i = 0; i < bins; ++i) {
    double lo = mn + (mx-mn)*i/bins;
    double hi = mn + (mx-mn)*(i+1)/bins;
    std::cout << "["<<int(lo)<<"–"<<int(hi)<<"] "
              << std::string(
                   counts[i]*50/lat.size(), '*')
              << " ("<<counts[i]<<")\n";
  }
}

inline void runAnalytics() {
  std::vector<double> lat;
  {
    std::lock_guard lk(completedMtx);
    lat.reserve(completedOrders.size());
    for (auto const& o : completedOrders) {
      double µ = std::chrono::duration_cast<
        std::chrono::microseconds>(
          o.dequeueTime - o.enqueueTime
      ).count();
      lat.push_back(µ);
    }
  }
  double total = std::reduce(
    std::execution::par,
    lat.begin(), lat.end(), 0.0
  );
  double avg = lat.empty() ? 0.0 : total/lat.size();
  std::cout << "\nAverage processing latency: "
            << avg << " µs\n";
  printLatencyHistogram(lat);
}
