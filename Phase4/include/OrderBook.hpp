#pragma once

#include <map>
#include <memory>
#include <iostream>
#include "../include/Order.hpp"

// Allocation Policies

// HeapAllocator: allocates dynamically from heap
template<typename T>
struct HeapAllocator {
    HeapAllocator(std::size_t /*capacity*/) {
        // HeapAllocator doesn't need to preallocate
    }

    T* allocate(std::size_t n) {
        return static_cast<T*>(::operator new(sizeof(T) * n));
    }

    void deallocate(T* ptr) {
        ::operator delete(ptr);
    }
};

// StackAllocator: allocates from a preallocated memory buffer
template<typename T>
struct StackAllocator {
    T* buffer;
    std::size_t capacity;
    std::size_t index;

    explicit StackAllocator(std::size_t n)
        : buffer(reinterpret_cast<T*>(::operator new(sizeof(T) * n))),
          capacity(n),
          index(0) {}

    ~StackAllocator() {
        ::operator delete(buffer);
    }

    T* allocate(std::size_t n) {
        if (index + n > capacity) {
            throw std::bad_alloc();
        }
        T* ptr = buffer + index;
        index += n;
        return ptr;
    }

    void deallocate(T* /*ptr*/) {
        // StackAllocator does not deallocate single objects
    }
};


template<typename T, typename AllocatorPolicy>
class OrderBook {
public:
    explicit OrderBook(std::size_t capacity)
        : allocator_(capacity), capacity_(capacity), size_(0) {
        data_ = allocator_.allocate(capacity_);
    }

    ~OrderBook() {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        allocator_.deallocate(data_);
    }

    void add_order(const T& order) {
        thread_.lock();
        if (size_ >= capacity_) {
            std::cerr << "OrderBookBuffer is full!\n";
            thread_.unlock();
            return;
        }
        new (data_ + size_) T(order);
        ++size_;
        thread_.unlock();
    }

    void print_orders() const {
        for (std::size_t i = 0; i < size_; ++i) {
            std::cout << data_[i] << "\n";
        }
    }

private:
    AllocatorPolicy allocator_;
    ThreadingPolicy thread_;
    T* data_;
    std::size_t capacity_;
    std::size_t size_;
};