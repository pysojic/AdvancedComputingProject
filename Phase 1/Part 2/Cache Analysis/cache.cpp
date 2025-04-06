#define NO_MAIN_IN_LINALG
#define NO_MAIN_IN_BENCH


#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <cmath>



#include "../../Part 1/linalg.cpp" 

double* create_random_matrix(int rows, int cols) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double* mat = new double[rows * cols];
    for (int i = 0; i < rows * cols; ++i) {
        mat[i] = dist(gen);
    }
    return mat;
}

double* create_random_vector(int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double* vec = new double[size];
    for (int i = 0; i < size; ++i) {
        vec[i] = dist(gen);
    }
    return vec;
}

long long time_mv_row_major(int rows, int cols) {
    double* mat = create_random_matrix(rows, cols);
    double* vec = create_random_vector(cols);
    double* res = new double[rows];
    auto start = std::chrono::high_resolution_clock::now();
    multiply_mv_row_major(mat, rows, cols, vec, res);
    auto end = std::chrono::high_resolution_clock::now();
    delete[] mat;
    delete[] vec;
    delete[] res;
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

long long time_mv_col_major(int rows, int cols) {
    double* mat = new double[rows * cols];
    {
        double* tmp = create_random_matrix(rows, cols);
        for(int r = 0; r < rows; ++r) {
            for(int c = 0; c < cols; ++c) {
                mat[c * rows + r] = tmp[r * cols + c];
            }
        }
        delete[] tmp;
    }
    double* vec = create_random_vector(cols);
    double* res = new double[rows];
    auto start = std::chrono::high_resolution_clock::now();
    multiply_mv_col_major(mat, rows, cols, vec, res);
    auto end = std::chrono::high_resolution_clock::now();
    delete[] mat;
    delete[] vec;
    delete[] res;
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}


long long time_mm_naive(int size) {
    double* A = create_random_matrix(size, size);
    double* B = create_random_matrix(size, size);
    double* R = new double[size * size];
    auto start = std::chrono::high_resolution_clock::now();
    multiply_mm_naive(A, size, size, B, size, size, R);
    auto end = std::chrono::high_resolution_clock::now();
    delete[] A;
    delete[] B;
    delete[] R;
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

long long time_mm_transposed_b(int size) {
    double* A = create_random_matrix(size, size);
    double* B = create_random_matrix(size, size);
    double* B_t = new double[size * size];
    for(int r = 0; r < size; ++r) {
        for(int c = 0; c < size; ++c) {
            B_t[c * size + r] = B[r * size + c];
        }
    }
    double* R = new double[size * size];
    auto start = std::chrono::high_resolution_clock::now();
    multiply_mm_transposed_b(A, size, size, B_t, size, size, R);
    auto end = std::chrono::high_resolution_clock::now();
    delete[] A;
    delete[] B;
    delete[] B_t;
    delete[] R;
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}


#ifndef NO_MAIN_IN_CACHE
int main() {
    // Example sizes to show cache behavior
    std::vector<int> sizes = {128, 512, 1024};

    // Matrix-Vector tests
    std::cout << "Cache Analysis: Row-major vs. Col-major (MV)\n";
    for (auto sz : sizes) {
        long long rm_time = time_mv_row_major(sz, sz);
        long long cm_time = time_mv_col_major(sz, sz);
        std::cout << "Size " << sz << "x" << sz
                  << " -> Row-major: " << rm_time << " us, "
                  << "Col-major: " << cm_time << " us\n";
    }

    std::cout << "\nCache Analysis: Naive vs. Transposed-B (MM)\n";
    for (auto sz : sizes) {
        long long naive_time = time_mm_naive(sz);
        long long trans_time = time_mm_transposed_b(sz);
        std::cout << "Size " << sz << "x" << sz
                  << " -> Naive: " << naive_time << " us, "
                  << "Transposed-B: " << trans_time << " us\n";
    }

    return 0;
}
#endif
