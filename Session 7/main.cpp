#include "Log.hpp"
#include "OrderProcessing.hpp"
#include "Analytics.hpp"
#include <iostream>
#include <shared_mutex>
#include <thread>
#include <vector>

int main(int argc, char* argv[]) {
  int P=5, C=4, N=10;
  if(argc>=4){
    P = std::stoi(argv[1]);
    C = std::stoi(argv[2]);
    N = std::stoi(argv[3]);
  }
  totalOrders = P * N;
  Log::line(Log::INFO,
    "Launching " + std::to_string(P) + " producers, "
    + std::to_string(C) + " consumers, "
    + std::to_string(N) + " orders each ("
    + std::to_string(totalOrders) + " total)");

//   std::barrier sync(C);
//   syncBarrierPtr = &sync;

  // spawn threads
  std::vector<std::thread> prods, cons;
  for(int i=0;i<P;i++)
    prods.emplace_back(orderProducer, i+1, N);
  for(int i=0;i<C;i++)
    cons.emplace_back(orderConsumer);

  // go!
  startLatch.count_down();

  for(auto& t: prods) t.join();
  for(auto& t: cons) t.join();

  std::cout<<"\n\n⏹ All done! Processed: "
           << ordersProcessed.load() << " orders\n";

  runAnalytics();

  // final book
  {
    std::shared_lock sl(orderBookMutex);
    std::cout<<"\n--- Final Order Book ---\n";
    for(auto const& [sym,vol]: orderBook)
      std::cout<<sym
               <<" | buy="<<vol.first
               <<" | sell="<<vol.second<<"\n";
  }
  return 0;
}
