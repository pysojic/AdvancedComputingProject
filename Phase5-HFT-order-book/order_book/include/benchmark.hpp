#pragma once

#include <iostream>
#include <unordered_map>
#include <map>
#include <list>
#include <vector>
#include <chrono>
#include <random>
#include <string>
#include "orderbook.hpp"
#include "optimized_orderbook.hpp"

void benchmark(size_t nOps=100000) {
    std::mt19937_64 rng(123);
    std::uniform_real_distribution<double> priceDist(90.0,110.0);
    std::uniform_int_distribution<int> qtyDist(1,1000);
    std::uniform_int_distribution<int> sideDist(0,1);

    OrderBook classic_ob;
    OptimizedOrderBook optimized_ob(2*nOps);

    std::vector<std::tuple<int,std::string,double,int,bool>> ops;
    ops.reserve(nOps);
    for (int i=1;i<=int(nOps);++i) {
        ops.emplace_back(i, std::to_string(i), priceDist(rng), qtyDist(rng), sideDist(rng)?true:false);
    }

    auto run = [&](auto& book, const char* name){
        using clk = std::chrono::high_resolution_clock;
        auto t0 = clk::now();
        for (auto& [id,id_str,p,q,s] : ops) book.addOrder(id_str,p,q,s);
        auto t1 = clk::now();
        for (auto& [id,id_str,p,q,s] : ops) book.modifyOrder(id_str, p+1, q+1);
        auto t2 = clk::now();
        for (auto& [id,id_str,p,q,s] : ops) book.queryPrice(std::get<2>(ops[id%ops.size()]));
        auto t3 = clk::now();
        for (auto& [id,id_str,p,q,s] : ops) {
            if (id % 3 == 0) book.deleteOrder(id_str);
        }
        auto t4 = clk::now();
        auto delta_add = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count();
        auto delta_modify = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
        auto delta_query = std::chrono::duration_cast<std::chrono::milliseconds>(t3-t2).count();
        auto delta_delete = std::chrono::duration_cast<std::chrono::milliseconds>(t4-t3).count();
        std::cout << name << " for " << nOps << " operation:\n";
        std::cout << "Execution time for addOrder operation: " << delta_add << " ms\n";
        std::cout << "Execution time for modifyOrder operation: " << delta_modify << " ms\n";
        std::cout << "Execution time for queryPrice operation: " << delta_query << " ms\n";
        std::cout << "Execution time for deleteOrder operation: " << delta_delete << " ms\n";
        std::cout << "Total exectution time: " << delta_add + delta_modify + delta_query + delta_delete << " ms\n\n";
    };

    run(classic_ob, "Classic order book");
    run(optimized_ob,  "Optimized order book");
}