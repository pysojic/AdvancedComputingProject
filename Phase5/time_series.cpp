#include <immintrin.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <random>

std::vector<double> moving_average_naive(const std::vector<double>& data, size_t window) {
    size_t n = data.size();
    std::vector<double> result(n - window + 1);
    for (size_t i = 0; i + window <= n; ++i) {
        double sum = 0.0;
        for (size_t j = 0; j < window; ++j) {
            sum += data[i + j];
        }
        result[i] = sum / window;
    }
    return result;
}

std::vector<double> moving_average_simd(const std::vector<double>& data, size_t window) {
    size_t n = data.size();
    size_t out_size = n - window + 1;
    std::vector<double> result(out_size);
    size_t i = 0;
    const size_t simd_width = 4; // AVX2 processes 4 doubles

    double init_sum = 0.0;
    for (size_t j = 0; j < window; ++j) init_sum += data[j];

    result[0] = init_sum / window;

    for (i = 1; i + simd_width - 1 < out_size; i += simd_width) {
        __m256d sum_vec = _mm256_setzero_pd();
        for (size_t j = 0; j < window; ++j) {
            __m256d prev = _mm256_loadu_pd(&result[i - 1]);
            __m256d add = _mm256_set1_pd(data[i + window - 1 - j]);
            __m256d sub = _mm256_set1_pd(data[i - 1 - j]);
            sum_vec = _mm256_add_pd(_mm256_sub_pd(prev, sub), add);
        }
        __m256d avg = _mm256_div_pd(sum_vec, _mm256_set1_pd(static_cast<double>(window)));
        _mm256_storeu_pd(&result[i], avg);
    }

    // Tail
    for (; i < out_size; ++i) {
        double sum = result[i - 1] * window;
        sum += data[i + window - 1] - data[i - 1];
        result[i] = sum / window;
    }
    return result;
}

int main() {
    const size_t N = 10000000;
    const size_t WINDOW = 16;
    std::vector<double> data(N);

    std::mt19937_64 rng(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (auto& x : data) x = dist(rng);

    auto res1 = moving_average_naive(data, WINDOW);
    auto res2 = moving_average_simd(data, WINDOW);

    // Benchmark naive
    auto t1 = std::chrono::high_resolution_clock::now();
    res1 = moving_average_naive(data, WINDOW);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dur_naive = t2 - t1;

    // Benchmark SIMD
    auto t3 = std::chrono::high_resolution_clock::now();
    res2 = moving_average_simd(data, WINDOW);
    auto t4 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dur_simd = t4 - t3;

    std::cout << "Naive duration: " << dur_naive.count() << " s\n";
    std::cout << "SIMD  duration: " << dur_simd.count() << " s\n";
    std::cout << "Speedup: " << dur_naive.count() / dur_simd.count() << "x\n";

    return 0;
}