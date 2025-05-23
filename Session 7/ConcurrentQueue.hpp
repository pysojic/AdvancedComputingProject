#pragma once
#include <array>
#include <atomic>
#include <cstddef>

template <typename T, std::size_t Capacity>
class ConcurrentQueue {
public:
    ConcurrentQueue() : head_(0), tail_(0) {}

    bool push(const T& item) {
        auto tail = tail_.load(std::memory_order_relaxed);
        auto next = (tail + 1) % Capacity;
        if (next == head_.load(std::memory_order_acquire)) return false;      // full
        buffer_[tail] = item;
        std::atomic_thread_fence(std::memory_order_release);                  // publish data
        tail_.store(next, std::memory_order_release);
        return true;
    }

    bool pop(T& item) {
        auto head = head_.load(std::memory_order_relaxed);
        if (head == tail_.load(std::memory_order_acquire)) return false;      // empty
        item = buffer_[head];
        std::atomic_thread_fence(std::memory_order_release);
        head_.store((head + 1) % Capacity, std::memory_order_release);
        return true;
    }
private:
    std::array<T, Capacity>              buffer_;
    std::atomic<std::size_t>             head_, tail_;
};
