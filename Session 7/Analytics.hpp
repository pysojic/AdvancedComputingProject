#pragma once
#include <algorithm>
#include <chrono>
#include <execution>
#include <numeric>
#include <vector>
#include <iostream>
#include "OrderProcessing.hpp"

inline void runAnalytics()
{
    std::vector<double> lat(completed.size());
    std::transform(std::execution::par, completed.begin(), completed.end(),
                   lat.begin(), [](const Order& o){
        return std::chrono::duration<double,std::micro>(o.deq-o.enq).count();
    });
    double tot = std::reduce(std::execution::par, lat.begin(), lat.end(), 0.0);
    std::cout<<"Avg latency: "<< (lat.empty()?0:tot/lat.size()) <<" µs\n";
}
