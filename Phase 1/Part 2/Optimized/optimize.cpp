#define NO_MAIN_IN_LINALG
#define NO_MAIN_IN_BENCH
#define NO_MAIN_IN_CACHE
#define NO_MAIN_IN_MEM
#define NO_MAIN_IN_INLINE

#include <iostream>
#include <chrono>
#include <cmath>
#include <random>
#include <vector>

void compute_stats(const std::vector<long long>& times, double& mean, double& stdev) {
    long long sum = 0;
    for (auto t : times) sum += t;
    mean = static_cast<double>(sum) / times.size();
    double variance = 0.0;
    for (auto t : times) {
        double diff = static_cast<double>(t) - mean;
        variance += diff * diff;
    }
    variance /= (times.size() > 1) ? (times.size() - 1) : 1;
    stdev = std::sqrt(variance);
}

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



void multiply_mm_naive(const double* A, int rowsA, int colsA,
                       const double* B, int rowsB, int colsB,
                       double* result) {
    if(colsA != rowsB) return; 
    for(int i = 0; i < rowsA; i++) {
        for(int j = 0; j < colsB; j++) {
            double sum = 0.0;
            for(int k = 0; k < colsA; k++) {
                sum += A[i * colsA + k] * B[k * colsB + j];
            }
            result[i * colsB + j] = sum;
        }
    }
}

void multiply_mm_transposed_b(const double* A, int rowsA, int colsA,
                              const double* B_transposed, int rowsB, int colsB,
                              double* result) {
    if(colsA != rowsB) return;
    for(int i = 0; i < rowsA; i++) {
        for(int j = 0; j < colsB; j++) {
            double sum = 0.0;
            for(int k = 0; k < colsA; k++) {
                sum += A[i * colsA + k] * B_transposed[j * rowsB + k];
            }
            result[i * colsB + j] = sum;
        }
    }
}



static const int BLOCK_SIZE = 64;

void multiply_mm_blocked(const double* A, int rowsA, int colsA,
                         const double* B, int rowsB, int colsB,
                         double* result) {
    if(colsA != rowsB) return;
    for(int i = 0; i < rowsA; i++) {
        for(int j = 0; j < colsB; j++) {
            result[i * colsB + j] = 0.0; 
        }
    }
    
    for(int iBlock = 0; iBlock < rowsA; iBlock += BLOCK_SIZE) {
        for(int kBlock = 0; kBlock < colsA; kBlock += BLOCK_SIZE) {
            for(int jBlock = 0; jBlock < colsB; jBlock += BLOCK_SIZE) {
                
                int iMax = std::min(iBlock + BLOCK_SIZE, rowsA);
                int kMax = std::min(kBlock + BLOCK_SIZE, colsA);
                int jMax = std::min(jBlock + BLOCK_SIZE, colsB);
                
                for(int i = iBlock; i < iMax; i++) {
                    for(int k = kBlock; k < kMax; k++) {
                        double aVal = A[i * colsA + k];
                        for(int j = jBlock; j < jMax; j++) {
                            result[i * colsB + j] += aVal * B[k * colsB + j];
                        }
                    }
                }
            }
        }
    }
}



long long time_mm_naive_run(int size) {
    double* A = create_random_matrix(size, size);
    double* B = create_random_matrix(size, size);
    double* C = new double[size * size];
    
    auto start = std::chrono::high_resolution_clock::now();
    multiply_mm_naive(A, size, size, B, size, size, C);
    auto end   = std::chrono::high_resolution_clock::now();
    
    delete[] A; delete[] B; delete[] C;
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

long long time_mm_transposed_b_run(int size) {
    double* A = create_random_matrix(size, size);
    double* B = create_random_matrix(size, size);
    double* B_trans = new double[size * size];
    for(int r = 0; r < size; r++) {
        for(int c = 0; c < size; c++) {
            B_trans[c * size + r] = B[r * size + c];
        }
    }
    double* C = new double[size * size];
    
    auto start = std::chrono::high_resolution_clock::now();
    multiply_mm_transposed_b(A, size, size, B_trans, size, size, C);
    auto end   = std::chrono::high_resolution_clock::now();
    
    delete[] A; delete[] B; delete[] B_trans; delete[] C;
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

long long time_mm_blocked_run(int size) {
    double* A = create_random_matrix(size, size);
    double* B = create_random_matrix(size, size);
    double* C = new double[size * size];
    
    auto start = std::chrono::high_resolution_clock::now();
    multiply_mm_blocked(A, size, size, B, size, size, C);
    auto end   = std::chrono::high_resolution_clock::now();
    
    delete[] A; delete[] B; delete[] C;
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

void benchmark_mm_function(const char* label, int size, int runs,
                           long long(*time_func)(int)) {
    std::vector<long long> durations;
    durations.reserve(runs);
    for(int i = 0; i < runs; i++) {
        durations.push_back(time_func(size));
    }
    double mean = 0.0, stdev = 0.0;
    compute_stats(durations, mean, stdev);
    
    std::cout << "[" << label << "] Size " << size << "x" << size
              << " | Avg: " << mean << " ns"
              << " | Stdev: " << stdev << " ns\n";
}


int main() {
    std::vector<int> sizes = {64, 256, 512}; 
    int runs = 5;  
    
    std::cout << "Benchmarking Matrix-Matrix (Naive, Transposed B, and Blocked):\n";
    for (auto sz : sizes) {
        benchmark_mm_function("MM Naive", sz, runs, time_mm_naive_run);
        benchmark_mm_function("MM Transposed B", sz, runs, time_mm_transposed_b_run);
        benchmark_mm_function("MM Blocked", sz, runs, time_mm_blocked_run);
        std::cout << "-------------------------------\n";
    }
    return 0;
}
