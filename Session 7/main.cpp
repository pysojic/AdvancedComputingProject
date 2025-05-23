// main.cpp
#include "Log.hpp"
#include "ConcurrentQueue.hpp"
#include "OrderProcessing.hpp"
#include "Analytics.hpp"

#include <barrier>
#include <iostream>
#include <shared_mutex>
#include <thread>
#include <vector>

int main(int argc, char* argv[]) {
  // optional: show DEBUG logs by uncommenting:
  // Log::setLevel(Log::DEBUG);

  int P = 5, C = 4, N = 10;
  if (argc >= 4) {
    P = std::stoi(argv[1]);
    C = std::stoi(argv[2]);
    N = std::stoi(argv[3]);
  }
  totalOrders = P * N;
  Log::line(Log::INFO,
    "Starting " + std::to_string(P) + " producers, "
    + std::to_string(C) + " consumers, "
    + std::to_string(N) + " orders each ("
    + std::to_string(totalOrders) + " total)"
  );

//   std::barrier syncBarrier(C);
//   syncBarrierPtr = &syncBarrier;

  std::vector<std::thread> producers, consumers;
  for (int i = 0; i < P; ++i)
    producers.emplace_back(orderProducer, i+1, N);
  for (int i = 0; i < C; ++i)
    consumers.emplace_back(orderConsumer);

  // start all threads
  startLatch.count_down();

  for (auto &t : producers) t.join();
  for (auto &t : consumers) t.join();

  std::cout << "\nAll done! Processed "
            << ordersProcessed.load()
            << " orders\n";

  runAnalytics();

  {
    std::shared_lock sl(orderBookMutex);
    std::cout << "\nFinal Order Book:\n";
    for (auto const& [sym,vol] : orderBook)
      std::cout << sym
                << " | buy="  << vol.first
                << " | sell=" << vol.second
                << "\n";
  }

  return 0;
}
