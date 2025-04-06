#include <iostream>

//------------------------------------------------------------
// Matrix-Vector Multiplication (Row-Major)
// Assume matrix is stored in row-major order.
void multiply_mv_row_major(const double* matrix, int rows, int cols, 
                             const double* vector, double* result) {
    if (!matrix || !vector || !result) {
        std::cerr << "Null pointer error in multiply_mv_row_major.\n";
        return;
    }
    for (int i = 0; i < rows; i++) {
        double sum = 0.0;
        for (int j = 0; j < cols; j++) {
            sum += matrix[i * cols + j] * vector[j];
        }
        result[i] = sum;
    }
}

//------------------------------------------------------------
// Matrix-Vector Multiplication (Column-Major)
// Assume matrix is stored in column-major order.
void multiply_mv_col_major(const double* matrix, int rows, int cols, 
                             const double* vector, double* result) {
    if (!matrix || !vector || !result) {
        std::cerr << "Null pointer error in multiply_mv_col_major.\n";
        return;
    }
    for (int i = 0; i < rows; i++) {
        double sum = 0.0;
        // Note: In column-major, element (i,j) is at index j * rows + i.
        for (int j = 0; j < cols; j++) {
            sum += matrix[j * rows + i] * vector[j];
        }
        result[i] = sum;
    }
}

//------------------------------------------------------------
// Team Member 3: Matrix-Matrix Multiplication (Naive)
// Assume matrixA, matrixB, and result are stored in row-major order.
void multiply_mm_naive(const double* matrixA, int rowsA, int colsA, 
                         const double* matrixB, int rowsB, int colsB, 
                         double* result) {
    if (!matrixA || !matrixB || !result) {
        std::cerr << "Null pointer error in multiply_mm_naive.\n";
        return;
    }
    if (colsA != rowsB) {
        std::cerr << "Dimension mismatch: colsA must equal rowsB in multiply_mm_naive.\n";
        return;
    }
    // For each element in the result matrix, compute the dot product.
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            double sum = 0.0;
            for (int k = 0; k < colsA; k++) {
                sum += matrixA[i * colsA + k] * matrixB[k * colsB + j];
            }
            result[i * colsB + j] = sum;
        }
    }
}

//------------------------------------------------------------
// Team Member 4: Matrix-Matrix Multiplication (Transposed B)
// Assume matrixA and result are in row-major order, and
// matrixB_transposed is the transpose of matrixB (also in row-major order).
// If matrixA is m x k and B (non-transposed) is k x n, then B_transposed is n x k.
// For multiplication, colsA (== k) must equal colsB (== k).
// The resulting matrix will be of size m x n (n == rowsB).
void multiply_mm_transposed_b(const double* matrixA, int rowsA, int colsA, 
                                const double* matrixB_transposed, int rowsB, int colsB, 
                                double* result) {
    if (!matrixA || !matrixB_transposed || !result) {
        std::cerr << "Null pointer error in multiply_mm_transposed_b.\n";
        return;
    }
    if (colsA != colsB) {
        std::cerr << "Dimension mismatch: colsA must equal colsB in multiply_mm_transposed_b.\n";
        return;
    }
    // Here, matrixB_transposed is of size (rowsB x colsB) where colsB equals the shared dimension.
    // The result will be of size (rowsA x rowsB).
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < rowsB; j++) {
            double sum = 0.0;
            for (int k = 0; k < colsA; k++) {
                // Note: row j of matrixB_transposed corresponds to column j of original B.
                sum += matrixA[i * colsA + k] * matrixB_transposed[j * colsA + k];
            }
            result[i * rowsB + j] = sum;
        }
    }
}

//------------------------------------------------------------
// Main test program
int main() {
    //--- Test multiply_mv_row_major ---
    {
        std::cout << "Testing multiply_mv_row_major:\n";
        int rows = 2, cols = 3;
        // 2x3 matrix in row-major order: [1 2 3; 4 5 6]
        double* matrix = new double[rows * cols]{1, 2, 3, 4, 5, 6};
        double* vector = new double[cols]{1, 1, 1};  // vector [1,1,1]
        double* result = new double[rows]{0};

        multiply_mv_row_major(matrix, rows, cols, vector, result);

        std::cout << "Result: ";
        for (int i = 0; i < rows; i++) {
            std::cout << result[i] << " ";
        }
        std::cout << "\n\n";

        delete[] matrix;
        delete[] vector;
        delete[] result;
    }

    //--- Test multiply_mv_col_major ---
    {
        std::cout << "Testing multiply_mv_col_major:\n";
        int rows = 2, cols = 3;
        // 2x3 matrix in column-major order: {1,4,2,5,3,6} represents [1 2 3; 4 5 6]
        double* matrix = new double[rows * cols]{1, 4, 2, 5, 3, 6};
        double* vector = new double[cols]{1, 1, 1};  // vector [1,1,1]
        double* result = new double[rows]{0};

        multiply_mv_col_major(matrix, rows, cols, vector, result);

        std::cout << "Result: ";
        for (int i = 0; i < rows; i++) {
            std::cout << result[i] << " ";
        }
        std::cout << "\n\n";

        delete[] matrix;
        delete[] vector;
        delete[] result;
    }

    //--- Test multiply_mm_naive ---
    {
        std::cout << "Testing multiply_mm_naive:\n";
        // Multiply a 2x3 matrix with a 3x2 matrix.
        int rowsA = 2, colsA = 3, rowsB = 3, colsB = 2;
        // matrixA: 2x3 matrix: [1 2 3; 4 5 6]
        double* matrixA = new double[rowsA * colsA]{1, 2, 3, 4, 5, 6};
        // matrixB: 3x2 matrix: [7 8; 9 10; 11 12]
        double* matrixB = new double[rowsB * colsB]{7, 8, 9, 10, 11, 12};
        double* result = new double[rowsA * colsB]{0};

        multiply_mm_naive(matrixA, rowsA, colsA, matrixB, rowsB, colsB, result);

        std::cout << "Result:\n";
        for (int i = 0; i < rowsA; i++) {
            for (int j = 0; j < colsB; j++) {
                std::cout << result[i * colsB + j] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";

        delete[] matrixA;
        delete[] matrixB;
        delete[] result;
    }

    //--- Test multiply_mm_transposed_b ---
    {
        std::cout << "Testing multiply_mm_transposed_b:\n";
        // Multiply a 2x3 matrix A with a 3x2 matrix B.
        // Instead of passing B directly, we pass its transpose.
        // matrixA: 2x3: [1 2 3; 4 5 6]
        int rowsA = 2, colsA = 3;
        double* matrixA = new double[rowsA * colsA]{1, 2, 3, 4, 5, 6};
        // matrixB (original): 3x2: [7 8; 9 10; 11 12]
        // Its transpose (matrixB_transposed) is 2x3: [7 9 11; 8 10 12]
        int trans_rows = 2, trans_cols = 3;  // transposed dimensions: 2x3
        double* matrixB_transposed = new double[trans_rows * trans_cols]{7, 9, 11, 8, 10, 12};
        // For A (2x3) * B (3x2), the result will be a 2x2 matrix.
        // With transposed B provided, result dimensions are (rowsA x trans_rows) = 2 x 2.
        double* result = new double[rowsA * trans_rows]{0};

        multiply_mm_transposed_b(matrixA, rowsA, colsA, matrixB_transposed, trans_rows, trans_cols, result);

        std::cout << "Result:\n";
        for (int i = 0; i < rowsA; i++) {
            for (int j = 0; j < trans_rows; j++) {
                std::cout << result[i * trans_rows + j] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";

        delete[] matrixA;
        delete[] matrixB_transposed;
        delete[] result;
    }

    return 0;
}
