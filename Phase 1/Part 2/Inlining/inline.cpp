#define NO_MAIN_IN_LINALG
#define NO_MAIN_IN_BENCH
#define NO_MAIN_IN_CACHE
#define NO_MAIN_IN_MEM


#include <iostream>
#include <chrono>
#include <random>
#include <vector>

inline double multiply_add(double a, double b, double& acc) {
    acc += (a * b);
    return acc;
}

void multiply_mm_inline(const double* A, const double* B, double* C,
                        int rows, int cols, int common_dim) {
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            double sum = 0.0;
            for(int k = 0; k < common_dim; ++k) {
                multiply_add(A[i * common_dim + k], B[k * cols + j], sum);
            }
            C[i * cols + j] = sum;
        }
    }
}

double multiply_add_no_inline(double a, double b, double& acc) {
    acc += (a * b);
    return acc;
}
void multiply_mm_no_inline(const double* A, const double* B, double* C,
                           int rows, int cols, int common_dim) {
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            double sum = 0.0;
            for(int k = 0; k < common_dim; ++k) {
                multiply_add_no_inline(A[i * common_dim + k], B[k * cols + j], sum);
            }
            C[i * cols + j] = sum;
        }
    }
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

// Timing function for inline multiply
long long time_multiply_mm_inline(int N) {
    double* A = create_random_matrix(N, N);
    double* B = create_random_matrix(N, N);
    double* C = new double[N * N];
    auto start = std::chrono::high_resolution_clock::now();
    multiply_mm_inline(A, B, C, N, N, N);
    auto end = std::chrono::high_resolution_clock::now();
    delete[] A; delete[] B; delete[] C;
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

// Timing function for no-inline multiply
long long time_multiply_mm_no_inline(int N) {
    double* A = create_random_matrix(N, N);
    double* B = create_random_matrix(N, N);
    double* C = new double[N * N];
    auto start = std::chrono::high_resolution_clock::now();
    multiply_mm_no_inline(A, B, C, N, N, N);
    auto end = std::chrono::high_resolution_clock::now();
    delete[] A; delete[] B; delete[] C;
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

#ifndef NO_MAIN_IN_INLINE
int main() {
    std::vector<int> sizes = {64, 128, 256};
    for(auto s : sizes) {
        long long t_inline = time_multiply_mm_inline(s);
        long long t_no_inline = time_multiply_mm_no_inline(s);
        std::cout << "Matrix size: " << s << "x" << s << "\n"
                  << "  Inline:    " << t_inline << " us\n"
                  << "  No Inline: " << t_no_inline << " us\n\n";
    }
    return 0;
}
#endif
