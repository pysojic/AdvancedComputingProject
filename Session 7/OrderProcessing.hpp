#pragma once
#include "ConcurrentQueue.hpp"
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

struct Order {
    int  id;
    std::string symbol;
    int  qty;
    double price;
    bool isBuy;
    std::chrono::high_resolution_clock::time_point enq, deq;
};

// ---------- globals ----------
constexpr std::size_t QUEUE_CAP = 1024;
inline ConcurrentQueue<Order, QUEUE_CAP> gQ;

inline std::unordered_map<std::string,std::pair<int,int>> orderBook;
inline std::shared_mutex  obMtx;

inline std::atomic<int>   processed{0};
inline std::counting_semaphore<3> sem(3);
inline std::barrier       syncBarrier(4);
inline std::latch         startLatch(1);
inline std::vector<Order> completed;                // for analytics
// ------------------------------

inline void producer(int pid, int N)
{
    startLatch.wait();
    for (int i=0;i<N;++i) {
        Order o{pid*1000+i, (i%2? "GOOGL":"AAPL"), 100+i,
                (i%2? 2800.5+i*0.05 : 146.23+i*0.01), !(i%2),
                std::chrono::high_resolution_clock::now()};
        while(!gQ.push(o)) std::this_thread::yield();
    }
}

inline void consumer()
{
    startLatch.wait();
    for (;;) {
        Order o;
        if (!gQ.pop(o)) {
            if (processed.load(std::memory_order_relaxed)>=50) break;
            std::this_thread::yield();
            continue;
        }
        o.deq = std::chrono::high_resolution_clock::now();
        sem.acquire();
        std::this_thread::sleep_for(std::chrono::microseconds(100));          // “processing”
        {
            std::unique_lock ul{obMtx};
            auto& [buy,sell]=orderBook[o.symbol];
            (o.isBuy? buy : sell) += o.qty;
        }
        processed.fetch_add(1,std::memory_order_relaxed);
        sem.release();
        completed.push_back(o);                                               // not thread‑safe in prod
        syncBarrier.arrive_and_wait();
    }
}
