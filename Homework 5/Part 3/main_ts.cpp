#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "time_series.h"

int main(){
    size_t n = 10000000;
    size_t window = 16;
    std::vector<double> data(n);
    std::mt19937_64 rng(42);
    std::uniform_real_distribution<double> dist(0.0,1.0);
    for(size_t i=0;i<n;++i) data[i] = dist(rng);

    size_t m = n - window + 1;
    std::vector<double> out1(m), out2(m);

    auto t0 = std::chrono::high_resolution_clock::now();
    moving_average_naive(data.data(), out1.data(), n, window);
    auto t1 = std::chrono::high_resolution_clock::now();
    moving_average_simd(data.data(), out2.data(), n, window);
    auto t2 = std::chrono::high_resolution_clock::now();

    auto ms1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "Naive ms: " << ms1 << "\n";
    std::cout << "SIMD  ms: " << ms2 << "\n";
    return 0;
}
