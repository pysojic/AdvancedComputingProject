#define NO_MAIN_IN_LINALG
#define NO_MAIN_IN_BENCH
#define NO_MAIN_IN_CACHE



#include <iostream>
#include <cstdlib>
#include <cstring>


#if defined(_MSC_VER)
    #include <malloc.h>
    void* aligned_malloc(size_t alignment, size_t size) {
        return _aligned_malloc(size, alignment);
    }
    void aligned_free(void* ptr) {
        _aligned_free(ptr);
    }
#else
    // For POSIX systems (Linux, macOS, etc.)
    void* aligned_malloc(size_t alignment, size_t size) {
        void* ptr = nullptr;
        if(posix_memalign(&ptr, alignment, size) != 0) {
            ptr = nullptr;
        }
        return ptr;
    }
    void aligned_free(void* ptr) {
        free(ptr);
    }
#endif

// Allocate aligned array of doubles
double* allocate_aligned_doubles(size_t n, size_t alignment = 64) {
    size_t sizeInBytes = n * sizeof(double);
    void* p = aligned_malloc(alignment, sizeInBytes);
    if (!p) {
        std::cerr << "Error: aligned_malloc failed.\n";
        return nullptr;
    }
    // Zero-initialize or leave uninitialized as needed
    std::memset(p, 0, sizeInBytes);
    return static_cast<double*>(p);
}

// Deallocate aligned array
void deallocate_aligned_doubles(double* ptr) {
    aligned_free(ptr);
}

// Matrix/Vector Multiplication (Row-Major)
void multiply_mv_row_major(const double* matrix, int rows, int cols,
                           const double* vector, double* result) {
    if(!matrix || !vector || !result || rows <= 0 || cols <= 0) return;
    for(int i = 0; i < rows; i++) {
        double sum = 0.0;
        for(int j = 0; j < cols; j++) {
            sum += matrix[i * cols + j] * vector[j];
        }
        result[i] = sum;
    }
}

// Matrix/Vector Multiplication (Column-Major)
void multiply_mv_col_major(const double* matrix, int rows, int cols,
                           const double* vector, double* result) {
    if(!matrix || !vector || !result || rows <= 0 || cols <= 0) return;
    for(int i = 0; i < rows; i++) {
        double sum = 0.0;
        for(int j = 0; j < cols; j++) {
            sum += matrix[j * rows + i] * vector[j];
        }
        result[i] = sum;
    }
}

// Matrix/Matrix Multiplication (Naive)
void multiply_mm_naive(const double* matrixA, int rowsA, int colsA,
                       const double* matrixB, int rowsB, int colsB,
                       double* result) {
    if(!matrixA || !matrixB || !result) return;
    if(colsA != rowsB) return;
    for(int i = 0; i < rowsA; i++) {
        for(int j = 0; j < colsB; j++) {
            double sum = 0.0;
            for(int k = 0; k < colsA; k++) {
                sum += matrixA[i * colsA + k] * matrixB[k * colsB + j];
            }
            result[i * colsB + j] = sum;
        }
    }
}

// Matrix/Matrix Multiplication (Transposed B)
void multiply_mm_transposed_b(const double* matrixA, int rowsA, int colsA,
                              const double* matrixB_transposed, int rowsB, int colsB,
                              double* result) {
    if(!matrixA || !matrixB_transposed || !result) return;
    if(colsA != rowsB) return;
    for(int i = 0; i < rowsA; i++) {
        for(int j = 0; j < colsB; j++) {
            double sum = 0.0;
            for(int k = 0; k < colsA; k++) {
                sum += matrixA[i * colsA + k] * matrixB_transposed[j * rowsB + k];
            }
            result[i * colsB + j] = sum;
        }
    }
}
