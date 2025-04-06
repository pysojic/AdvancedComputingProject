#define NO_MAIN_IN_LINALG
#define NO_MAIN_IN_BENCH
#define NO_MAIN_IN_CACHE

#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <cmath>
#include "../../Part 2/memory/linalg_memory.cpp" 

// Create unaligned array (standard new)
double* create_unaligned_doubles(size_t n) {
    return new double[n]; 
}

// Fill array with random data
void fill_random_data(double* arr, size_t n) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dist(0.0, 1.0);

    for(size_t i = 0; i < n; i++) {
        arr[i] = dist(gen);
    }
}

// Measure MV row-major performance
long long time_mv_row_major(double* matrix, double* vector, double* result,
                            int rows, int cols) {
    auto start = std::chrono::high_resolution_clock::now();
    multiply_mv_row_major(matrix, rows, cols, vector, result);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

// Measure MM naive performance (row-major)
long long time_mm_naive(double* A, double* B, double* R,
                        int rowsA, int colsA, int rowsB, int colsB) {
    auto start = std::chrono::high_resolution_clock::now();
    multiply_mm_naive(A, rowsA, colsA, B, rowsB, colsB, R);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

// Helper to compute average from collected times
double average_time(const std::vector<long long>& times) {
    long long sum = 0;
    for (auto t : times) sum += t;
    return static_cast<double>(sum) / times.size();
}

// Helper to compute sample standard deviation
double stdev_time(const std::vector<long long>& times, double mean) {
    double var = 0.0;
    for (auto t : times) {
        double diff = t - mean;
        var += diff * diff;
    }
    // sample stdev
    var /= (times.size() - 1);
    return std::sqrt(var);
}

#ifndef NO_MAIN_IN_MEM
int main() {


    std::vector<int> testSizes = {256, 512, 1024}; 
    int runs = 5; 

    // --- Test: Matrix-Vector Multiplication (Row-Major) ---
    std::cout << "=== Matrix-Vector Multiplication (Row-Major): Aligned vs. Unaligned ===\n";
    for (auto size : testSizes) {
        int rows = size, cols = size;
        size_t matSize = rows * cols;
        size_t vecSize = cols;
        size_t resSize = rows;

        std::vector<long long> unalignedTimes, alignedTimes;
        unalignedTimes.reserve(runs);
        alignedTimes.reserve(runs);

        for(int r = 0; r < runs; r++) {
            // Unaligned
            double* mat_unaligned = create_unaligned_doubles(matSize);
            double* vec_unaligned = create_unaligned_doubles(vecSize);
            double* res_unaligned = create_unaligned_doubles(resSize);
            fill_random_data(mat_unaligned, matSize);
            fill_random_data(vec_unaligned, vecSize);
            unalignedTimes.push_back(
                time_mv_row_major(mat_unaligned, vec_unaligned, res_unaligned, rows, cols)
            );
            delete[] mat_unaligned;
            delete[] vec_unaligned;
            delete[] res_unaligned;

            // Aligned
            double* mat_aligned = allocate_aligned_doubles(matSize, 64);
            double* vec_aligned = allocate_aligned_doubles(vecSize, 64);
            double* res_aligned = allocate_aligned_doubles(resSize, 64);
            fill_random_data(mat_aligned, matSize);
            fill_random_data(vec_aligned, vecSize);
            alignedTimes.push_back(
                time_mv_row_major(mat_aligned, vec_aligned, res_aligned, rows, cols)
            );
            deallocate_aligned_doubles(mat_aligned);
            deallocate_aligned_doubles(vec_aligned);
            deallocate_aligned_doubles(res_aligned);
        }

        double avgUnaligned = average_time(unalignedTimes);
        double stdUnaligned = stdev_time(unalignedTimes, avgUnaligned);
        double avgAligned   = average_time(alignedTimes);
        double stdAligned   = stdev_time(alignedTimes, avgAligned);

        std::cout << "Size: " << size << "x" << size << "  (Runs: " << runs << ")\n"
                  << "  Unaligned MV: " << avgUnaligned << " us +/- " << stdUnaligned << "\n"
                  << "  Aligned MV:   " << avgAligned   << " us +/- " << stdAligned   << "\n";
    }

    // --- Test: Matrix-Matrix Multiplication (Naive) ---
    std::cout << "\n=== Matrix-Matrix Multiplication (Naive): Aligned vs. Unaligned ===\n";
    for (auto size : testSizes) {
        int rowsA = size, colsA = size;
        int rowsB = size, colsB = size;
        size_t matASize = rowsA * colsA;
        size_t matBSize = rowsB * colsB;
        size_t matRSize = rowsA * colsB;

        std::vector<long long> unalignedTimes, alignedTimes;
        unalignedTimes.reserve(runs);
        alignedTimes.reserve(runs);

        for(int r = 0; r < runs; r++) {
            // Unaligned
            double* A_unaligned = create_unaligned_doubles(matASize);
            double* B_unaligned = create_unaligned_doubles(matBSize);
            double* R_unaligned = create_unaligned_doubles(matRSize);
            fill_random_data(A_unaligned, matASize);
            fill_random_data(B_unaligned, matBSize);
            unalignedTimes.push_back(
                time_mm_naive(A_unaligned, B_unaligned, R_unaligned,
                              rowsA, colsA, rowsB, colsB)
            );
            delete[] A_unaligned;
            delete[] B_unaligned;
            delete[] R_unaligned;

            // Aligned
            double* A_aligned = allocate_aligned_doubles(matASize, 64);
            double* B_aligned = allocate_aligned_doubles(matBSize, 64);
            double* R_aligned = allocate_aligned_doubles(matRSize, 64);
            fill_random_data(A_aligned, matASize);
            fill_random_data(B_aligned, matBSize);
            alignedTimes.push_back(
                time_mm_naive(A_aligned, B_aligned, R_aligned,
                              rowsA, colsA, rowsB, colsB)
            );
            deallocate_aligned_doubles(A_aligned);
            deallocate_aligned_doubles(B_aligned);
            deallocate_aligned_doubles(R_aligned);
        }

        double avgUnaligned = average_time(unalignedTimes);
        double stdUnaligned = stdev_time(unalignedTimes, avgUnaligned);
        double avgAligned   = average_time(alignedTimes);
        double stdAligned   = stdev_time(alignedTimes, avgAligned);

        std::cout << "Size: " << size << "x" << size << "  (Runs: " << runs << ")\n"
                  << "  Unaligned MM: " << avgUnaligned << " us +/- " << stdUnaligned << "\n"
                  << "  Aligned MM:   " << avgAligned   << " us +/- " << stdAligned   << "\n";
    }

     
    return 0;
}
#endif
