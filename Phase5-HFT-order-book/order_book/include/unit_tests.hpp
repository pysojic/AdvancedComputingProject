#pragma once

#include <iostream>
#include <cassert>
#include "orderbook.hpp"
#include "optimized_orderbook.hpp"

class OrderBookTests {
    public:
        template<typename OrderBookType>
        static void testAddOrder(OrderBookType& book) {
            book.addOrder("ORD001", 50.10, 100, true);
            assert(book.queryPrice(50.10).count("ORD001") == 1);
        }
    
        static void testModifyOrder(OrderBook& book) {
            book.addOrder("ORD002", 55.00, 200, false);
            book.modifyOrder("ORD002", 56.00, 150);
            auto result = book.queryPrice(56.00);
            assert(result.count("ORD002") == 1);
            assert(result["ORD002"].quantity == 150);
        }

        static void testModifyOrder(OptimizedOrderBook& book) {
            book.addOrder("ORD002", 55.00, 200, false);
            book.modifyOrder("ORD002", 56.00, 150);
            auto result = book.queryPrice(56.00);
            assert(result.count("ORD002") == 1);
            assert(result["ORD002"]->quantity == 150);
        }
    
        template<typename OrderBookType>
        static void testDeleteOrder(OrderBookType& book) {
            book.addOrder("ORD003", 48.50, 300, true);
            book.deleteOrder("ORD003");
            assert(book.queryPrice(48.50).count("ORD003") == 0);
        }
    
        template<typename OrderBookType>
        static void testQueryPrice(OrderBookType& book) {
            book.addOrder("ORD004", 49.75, 120, false);
            book.addOrder("ORD005", 49.75, 80, false);
            auto result = book.queryPrice(49.75);
            assert(result.size() == 2);
            assert(result.count("ORD004") == 1);
            assert(result.count("ORD005") == 1);
        }
    
        template<typename OrderBookType>
        static void runTests() {
            OrderBookType book;
            testAddOrder(book);
            testModifyOrder(book);
            testDeleteOrder(book);
            testQueryPrice(book);
        }
    };
