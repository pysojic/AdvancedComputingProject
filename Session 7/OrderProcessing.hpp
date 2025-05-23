// OrderProcessing.hpp
#pragma once
#include "Log.hpp"
#include "ConcurrentQueue.hpp"
#include <atomic>
#include <barrier>
#include <chrono>
#include <latch>
#include <mutex>
#include <semaphore>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

// ---- Order and globals ----
struct Order {
  int orderId;
  std::string symbol;
  int quantity;
  double price;
  bool isBuy;
  std::chrono::high_resolution_clock::time_point enqueueTime;
  std::chrono::high_resolution_clock::time_point dequeueTime;
};

constexpr size_t QUEUE_CAPACITY = 1024;
inline ConcurrentQueue<Order, QUEUE_CAPACITY> orderQueue;

// order book: symbol → (buyVol,sellVol)
inline std::unordered_map<std::string, std::pair<int,int>> orderBook;
inline std::shared_mutex orderBookMutex;

// coordination primitives
inline std::latch           startLatch(1);
inline std::counting_semaphore<3> processingSem(3);
// inline std::barrier*        syncBarrierPtr = nullptr;

// totals & analytics
inline int                  totalOrders = 0;
inline std::atomic<int>     ordersProcessed{0};
inline std::atomic<int>     progress{0};
inline std::vector<Order>   completedOrders;
inline std::mutex           completedMtx;

// ---- progress bar helper ----
inline void printProgress(int done, int total) {
  static constexpr int width = 40;
  int pos = done * width / total;
  std::cout << '\r' << "[";
  for(int i=0;i<width;i++) std::cout << (i<pos?'=':' ');
  std::cout << "] " << done << "/" << total << " orders";
  std::cout.flush();
}

// ---- producer ----
inline void orderProducer(int pid, int num) {
  startLatch.wait();
  for(int i=0;i<num;++i) {
    Order o;
    o.orderId     = pid*1000 + i;
    o.symbol      = (i%2==0?"AAPL":"GOOGL");
    o.quantity    = 100 + i;
    o.price       = (o.symbol=="AAPL"?146.23+i*0.01:2800.5+i*0.05);
    o.isBuy       = (i%2==0);
    o.enqueueTime = std::chrono::high_resolution_clock::now();
    while(!orderQueue.push(o)) std::this_thread::yield();
    Log::line(Log::DEBUG, "P" + std::to_string(pid) + " enq " + o.symbol);
  }
}

// ---- consumer ----
inline void orderConsumer() {
  startLatch.wait();
  while(true) {
    Order o;
    if(!orderQueue.pop(o)) {
      if(ordersProcessed.load()>=totalOrders) break;
      std::this_thread::yield();
      continue;
    }
    o.dequeueTime = std::chrono::high_resolution_clock::now();
    processingSem.acquire();
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    {
      std::unique_lock lock(orderBookMutex);
      auto& ent = orderBook[o.symbol];
      if(o.isBuy)  ent.first  += o.quantity;
      else          ent.second += o.quantity;
    }
    processingSem.release();
    int done = ++ordersProcessed;
    {
      std::lock_guard lk(completedMtx);
      completedOrders.push_back(o);
    }
    printProgress(done, totalOrders);
    // if(syncBarrierPtr)
    //   syncBarrierPtr->arrive_and_wait();
    if(done % 10 == 0) {
      Log::line(Log::INFO, "── Snapshot at " + std::to_string(done) + " orders ──");
      std::shared_lock sl(orderBookMutex);
      for(auto const& [sym,vol]: orderBook)
        std::cout<<"  "<<sym<<" buy="<<vol.first<<" sell="<<vol.second<<"\n";
    }
  }
}
