#ifndef ORDER_PRIORITY_QUEUE_H
#define ORDER_PRIORITY_QUEUE_H

#include <vector>

struct Order {
    double price;
    int timestamp;
    bool operator<(const Order& other) const {
        return price < other.price;
    }
};

class OrderPriorityQueue {
public:
    void push(const Order& o) {
        data.push_back(o);
        siftUp(data.size() - 1);
    }
    Order top() const {
        return data.front();
    }
    void pop() {
        data[0] = data.back();
        data.pop_back();
        if (!data.empty()) siftDown(0);
    }
    bool empty() const {
        return data.empty();
    }
private:
    std::vector<Order> data;
    void siftUp(size_t idx) {
        while (idx > 0) {
            size_t parent = (idx - 1) / 2;
            if (!(data[parent] < data[idx])) break;
            std::swap(data[parent], data[idx]);
            idx = parent;
        }
    }
    void siftDown(size_t idx) {
        size_t n = data.size();
        while (true) {
            size_t left = 2 * idx + 1;
            size_t right = 2 * idx + 2;
            size_t largest = idx;
            if (left < n && data[largest] < data[left]) largest = left;
            if (right < n && data[largest] < data[right]) largest = right;
            if (largest == idx) break;
            std::swap(data[idx], data[largest]);
            idx = largest;
        }
    }
};

#endif

