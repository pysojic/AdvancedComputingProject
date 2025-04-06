#define NO_MAIN_IN_LINALG
#include <iostream>
#include <chrono>
#include <cmath>
#include <random>
#include <vector>

#include "../../Part 1/linalg.cpp"  

// Generate a random matrix of size rows x cols
double* create_random_matrix(int rows, int cols) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    double* mat = new double[rows * cols];
    for(int i = 0; i < rows * cols; ++i) {
        mat[i] = dist(gen);
    }
    return mat;
}

// Generate a random vector of size length
double* create_random_vector(int length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    double* vec = new double[length];
    for(int i = 0; i < length; ++i) {
        vec[i] = dist(gen);
    }
    return vec;
}

// Compute mean and sample standard deviation of a list of timings
void compute_stats(const std::vector<long long>& times, double& mean, double& stdev) {
    long long sum = 0;
    for(auto t : times) sum += t;
    mean = static_cast<double>(sum) / times.size();
    
    double variance = 0.0;
    for(auto t : times) {
        double diff = static_cast<double>(t) - mean;
        variance += diff * diff;
    }
    // Sample standard deviation
    variance /= (times.size() - 1);
    stdev = std::sqrt(variance);
}

// Benchmark matrix-vector multiplication (row-major)
void benchmark_mv_row_major(int rows, int cols, int runs) {
    double* mat = create_random_matrix(rows, cols);
    double* vec = create_random_vector(cols);
    double* res = new double[rows];

    std::vector<long long> durations;
    durations.reserve(runs);

    for(int i = 0; i < runs; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        multiply_mv_row_major(mat, rows, cols, vec, res);
        auto end = std::chrono::high_resolution_clock::now();
        durations.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    double mean = 0.0, stdev = 0.0;
    compute_stats(durations, mean, stdev);
    std::cout << "[MV Row-Major] Size " << rows << "x" << cols
              << " | Avg: " << mean << " ns"
              << " | Stdev: " << stdev << " ns\n";

    delete[] mat;
    delete[] vec;
    delete[] res;
}

// Benchmark matrix-vector multiplication (col-major)
void benchmark_mv_col_major(int rows, int cols, int runs) {
    // Create random matrix in "fake" column-major:
    double* mat = new double[rows * cols];
    {
        double* tmp = create_random_matrix(rows, cols);
        // Re-map row-major random data into col-major layout
        for(int r = 0; r < rows; ++r) {
            for(int c = 0; c < cols; ++c) {
                mat[c * rows + r] = tmp[r * cols + c];
            }
        }
        delete[] tmp;
    }
    double* vec = create_random_vector(cols);
    double* res = new double[rows];

    std::vector<long long> durations;
    durations.reserve(runs);

    for(int i = 0; i < runs; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        multiply_mv_col_major(mat, rows, cols, vec, res);
        auto end = std::chrono::high_resolution_clock::now();
        durations.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    double mean = 0.0, stdev = 0.0;
    compute_stats(durations, mean, stdev);
    std::cout << "[MV Col-Major] Size " << rows << "x" << cols
              << " | Avg: " << mean << " ns"
              << " | Stdev: " << stdev << " ns\n";

    delete[] mat;
    delete[] vec;
    delete[] res;
}

// Benchmark naive matrix-matrix multiplication (row-major)
void benchmark_mm_naive(int size, int runs) {
    // We'll do square matrices for demonstration
    int rowsA = size, colsA = size;
    int rowsB = size, colsB = size;

    double* A = create_random_matrix(rowsA, colsA);
    double* B = create_random_matrix(rowsB, colsB);
    double* R = new double[rowsA * colsB]{0};

    std::vector<long long> durations;
    durations.reserve(runs);

    for(int i = 0; i < runs; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        multiply_mm_naive(A, rowsA, colsA, B, rowsB, colsB, R);
        auto end = std::chrono::high_resolution_clock::now();
        durations.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    double mean = 0.0, stdev = 0.0;
    compute_stats(durations, mean, stdev);
    std::cout << "[MM Naive] Size " << size << "x" << size
              << " | Avg: " << mean << " ns"
              << " | Stdev: " << stdev << " ns\n";

    delete[] A;
    delete[] B;
    delete[] R;
}

// Benchmark matrix-matrix multiplication with transposed B
void benchmark_mm_transposed_b(int size, int runs) {
    int rowsA = size, colsA = size;
    int rowsB = size, colsB = size;

    double* A = create_random_matrix(rowsA, colsA);
    double* B = create_random_matrix(rowsB, colsB);
    // Transpose B
    double* B_trans = new double[rowsB * colsB];
    for(int r = 0; r < rowsB; ++r) {
        for(int c = 0; c < colsB; ++c) {
            B_trans[c * rowsB + r] = B[r * colsB + c];
        }
    }
    double* R = new double[rowsA * colsB]{0};

    std::vector<long long> durations;
    durations.reserve(runs);

    for(int i = 0; i < runs; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        multiply_mm_transposed_b(A, rowsA, colsA, B_trans, rowsB, colsB, R);
        auto end = std::chrono::high_resolution_clock::now();
        durations.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    double mean = 0.0, stdev = 0.0;
    compute_stats(durations, mean, stdev);
    std::cout << "[MM Transposed B] Size " << size << "x" << size
              << " | Avg: " << mean << " ns"
              << " | Stdev: " << stdev << " ns\n";

    delete[] A;
    delete[] B;
    delete[] B_trans;
    delete[] R;
}

int main() {
    // Example test sizes
    std::vector<int> vecSizes = {64, 512, 2048}; // for MV
    std::vector<int> matSizes = {64, 256, 512};  // for MM
    int runs = 5; // number of repetitions

    std::cout << "Benchmarking Matrix-Vector (Row-Major and Col-Major):\n";
    for(auto sz : vecSizes) {
        benchmark_mv_row_major(sz, sz, runs);
        benchmark_mv_col_major(sz, sz, runs);
        std::cout << "-------------------------------\n";
    }

    std::cout << "\nBenchmarking Matrix-Matrix (Naive and Transposed B):\n";
    for(auto sz : matSizes) {
        benchmark_mm_naive(sz, runs);
        benchmark_mm_transposed_b(sz, runs);
        std::cout << "-------------------------------\n";
    }

    return 0;
}
