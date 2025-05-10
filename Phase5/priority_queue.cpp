
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <queue>
#include <random>
#include <vector>


struct Order
{
    double price;
    int32_t id;

    // Natural ordering: higher price = higher priority (max-heap)
    bool operator<(const Order &other) const noexcept
    {
        return price < other.price;
    }
};


template <typename T, typename Compare = std::less<T>>
class BinaryHeap
{
public:
    explicit BinaryHeap(std::size_t reserve_n = 0, Compare comp = Compare{})
        : cmp_(comp)
    {
        heap_.reserve(reserve_n + 1); // +1 because we push a dummy
        heap_.emplace_back();         // index 0 unused (simpler math)
    }

    [[nodiscard]] bool empty() const noexcept { return size() == 0; }
    [[nodiscard]] std::size_t size() const noexcept { return heap_.size() - 1; }

    const T &top() const noexcept { return heap_[1]; }

    void push(const T &value)
    {
        heap_.push_back(value);
        sift_up(size());
    }

    void pop()
    {
        heap_[1] = heap_.back();
        heap_.pop_back();
        if (!empty())
            sift_down(1);
    }

private:
    std::vector<T> heap_; // heap_[0] is dummy
    Compare cmp_;         // “returns true if a < b” for min-heap

    void sift_up(std::size_t idx)
    {
        while (idx > 1)
        {
            std::size_t parent = idx >> 1;
            if (cmp_(heap_[parent], heap_[idx]))
            { 
                std::swap(heap_[parent], heap_[idx]);
                idx = parent;
            }
            else
                break;
        }
    }

    void sift_down(std::size_t idx)
    {
        std::size_t n = size();
        while (true)
        {
            std::size_t left = idx << 1;
            std::size_t right = left + 1;
            std::size_t largest = idx;

            if (left <= n && cmp_(heap_[largest], heap_[left]))
                largest = left;
            if (right <= n && cmp_(heap_[largest], heap_[right]))
                largest = right;

            if (largest != idx)
            {
                std::swap(heap_[idx], heap_[largest]);
                idx = largest;
            }
            else
                break;
        }
    }
};


using hrc = std::chrono::high_resolution_clock;
template <typename F>
double time_ms(F &&f)
{
    auto t0 = hrc::now();
    f();
    auto t1 = hrc::now();
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}


int main()
{
    constexpr std::size_t N = 1'000'000;
    std::vector<Order> orders;
    orders.reserve(N);

    std::mt19937_64 rng(42);
    std::uniform_real_distribution<double> price_dist(10.0, 100.0);
    for (std::size_t i = 0; i < N; ++i)
        orders.push_back({price_dist(rng), static_cast<int32_t>(i)});

    BinaryHeap<Order> my_heap(N);

    double t_push_my = time_ms([&]
                               {
        for (const auto& o : orders) my_heap.push(o); });

    double t_pop_my = time_ms([&]
                              {
        while (!my_heap.empty()) my_heap.pop(); });

    std::priority_queue<Order> stdpq;
    stdpq = std::priority_queue<Order>();                       // ensure empty
    stdpq = std::priority_queue<Order>(std::less<Order>(), {}); // reserve hint not possible

    double t_push_std = time_ms([&]
                                {
        for (const auto& o : orders) stdpq.push(o); });

    double t_pop_std = time_ms([&]
                               {
        while (!stdpq.empty()) stdpq.pop(); });

    std::cout.setf(std::ios::fixed);
    std::cout.precision(3);
    std::cout << "\n=== Priority-Queue Benchmark (" << N << " orders) ===\n";
    std::cout << "Custom BinaryHeap push: " << t_push_my << " ms\n";
    std::cout << "std::priority_queue push: " << t_push_std << " ms  ("
              << t_push_std / t_push_my << "× slower)\n\n";
    std::cout << "Custom BinaryHeap pop : " << t_pop_my << " ms\n";
    std::cout << "std::priority_queue pop : " << t_pop_std << " ms  ("
              << t_pop_std / t_pop_my << "× slower)\n";
}
