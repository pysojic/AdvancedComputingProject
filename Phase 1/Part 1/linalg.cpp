#include <iostream>
#include <cstdlib>

void multiply_mv_row_major(const double* matrix, int rows, int cols,
                           const double* vector, double* result) {
    if(!matrix || !vector || !result || rows <= 0 || cols <= 0) {
        std::cerr << "Error in multiply_mv_row_major: invalid arguments.\n";
        return;
    }
    for(int i = 0; i < rows; i++) {
        double sum = 0.0;
        for(int j = 0; j < cols; j++) {
            sum += matrix[i * cols + j] * vector[j];
        }
        result[i] = sum;
    }
}

void multiply_mv_col_major(const double* matrix, int rows, int cols,
                           const double* vector, double* result) {
    if(!matrix || !vector || !result || rows <= 0 || cols <= 0) {
        std::cerr << "Error in multiply_mv_col_major: invalid arguments.\n";
        return;
    }
    for(int i = 0; i < rows; i++) {
        double sum = 0.0;
        for(int j = 0; j < cols; j++) {
            sum += matrix[j * rows + i] * vector[j];
        }
        result[i] = sum;
    }
}

void multiply_mm_naive(const double* matrixA, int rowsA, int colsA,
                       const double* matrixB, int rowsB, int colsB,
                       double* result) {
    if(!matrixA || !matrixB || !result ||
       rowsA <= 0 || colsA <= 0 || rowsB <= 0 || colsB <= 0) {
        std::cerr << "Error in multiply_mm_naive: invalid arguments.\n";
        return;
    }
    if(colsA != rowsB) {
        std::cerr << "Error in multiply_mm_naive: dimension mismatch.\n";
        return;
    }
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

void multiply_mm_transposed_b(const double* matrixA, int rowsA, int colsA,
                              const double* matrixB_transposed, int rowsB, int colsB,
                              double* result) {
    if(!matrixA || !matrixB_transposed || !result ||
       rowsA <= 0 || colsA <= 0 || rowsB <= 0 || colsB <= 0) {
        std::cerr << "Error in multiply_mm_transposed_b: invalid arguments.\n";
        return;
    }
    if(colsA != rowsB) {
        std::cerr << "Error in multiply_mm_transposed_b: dimension mismatch.\n";
        return;
    }
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

#ifndef NO_MAIN_IN_LINALG
int main() {
    // Test multiply_mv_row_major
    {
        int rows = 2, cols = 3;
        double* mat = new double[rows * cols]{1,2,3,4,5,6};
        double* vec = new double[cols]{1,1,1};
        double* res = new double[rows]{0,0};

        multiply_mv_row_major(mat, rows, cols, vec, res);
        std::cout << "Result of multiply_mv_row_major:\n";
        for(int i = 0; i < rows; i++) {
            std::cout << res[i] << " ";
        }
        std::cout << "\n";

        delete[] mat;
        delete[] vec;
        delete[] res;
    }

    // Test multiply_mv_col_major
    {
        int rows = 2, cols = 3;
        double* mat_col = new double[rows * cols]{
            // Column-major layout for the same 2x3 matrix
            1,4, 2,5, 3,6
        };
        double* vec = new double[cols]{1,1,1};
        double* res = new double[rows]{0,0};

        multiply_mv_col_major(mat_col, rows, cols, vec, res);
        std::cout << "Result of multiply_mv_col_major:\n";
        for(int i = 0; i < rows; i++) {
            std::cout << res[i] << " ";
        }
        std::cout << "\n";

        delete[] mat_col;
        delete[] vec;
        delete[] res;
    }

    // Test multiply_mm_naive
    {
        int rowsA = 2, colsA = 3, rowsB = 3, colsB = 2;
        double* A = new double[rowsA * colsA]{1,2,3,4,5,6};
        double* B = new double[rowsB * colsB]{7,8,9,10,11,12};
        double* R = new double[rowsA * colsB]{0,0,0,0};

        multiply_mm_naive(A, rowsA, colsA, B, rowsB, colsB, R);
        std::cout << "Result of multiply_mm_naive:\n";
        for(int i = 0; i < rowsA; i++) {
            for(int j = 0; j < colsB; j++) {
                std::cout << R[i * colsB + j] << " ";
            }
            std::cout << "\n";
        }

        delete[] A;
        delete[] B;
        delete[] R;
    }

    // Test multiply_mm_transposed_b
    {
        int rowsA = 2, colsA = 3, rowsB = 3, colsB = 2;
        double* A = new double[rowsA * colsA]{1,2,3,4,5,6};
        double* B = new double[rowsB * colsB]{7,8,9,10,11,12};
        double* B_trans = new double[rowsB * colsB];
        for(int r = 0; r < rowsB; r++) {
            for(int c = 0; c < colsB; c++) {
                B_trans[c * rowsB + r] = B[r * colsB + c];
            }
        }
        double* R = new double[rowsA * colsB]{0,0,0,0};

        multiply_mm_transposed_b(A, rowsA, colsA, B_trans, rowsB, colsB, R);
        std::cout << "Result of multiply_mm_transposed_b:\n";
        for(int i = 0; i < rowsA; i++) {
            for(int j = 0; j < colsB; j++) {
                std::cout << R[i * colsB + j] << " ";
            }
            std::cout << "\n";
        }

        delete[] A;
        delete[] B;
        delete[] B_trans;
        delete[] R;
    }

    return 0;
}
#endif
