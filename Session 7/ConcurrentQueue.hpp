// ConcurrentQueue.hpp
#pragma once
#include <array>
#include <atomic>
#include <cstddef>

template <typename T, std::size_t Capacity>
class ConcurrentQueue {
public:
  ConcurrentQueue() : head(0), tail(0) {}

  bool push(const T& item) {
    size_t curTail = tail.load(std::memory_order_relaxed);
    size_t next    = (curTail + 1) % Capacity;
    if (next == head.load(std::memory_order_acquire))
      return false; // full
    buffer[curTail] = item;
    std::atomic_thread_fence(std::memory_order_release);
    tail.store(next, std::memory_order_release);
    return true;
  }

  bool pop(T& item) {
    size_t curHead = head.load(std::memory_order_relaxed);
    if (curHead == tail.load(std::memory_order_acquire))
      return false; // empty
    item = buffer[curHead];
    std::atomic_thread_fence(std::memory_order_release);
    head.store((curHead + 1) % Capacity, std::memory_order_release);
    return true;
  }

private:
  std::array<T, Capacity> buffer;
  std::atomic<size_t>     head;
  std::atomic<size_t>     tail;
};
