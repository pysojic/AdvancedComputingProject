#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>

#include "MatrixFunctions.hpp"

double g_sum = 0.0; 
int BLOCK_SIZE = 32;

int main()
{
    using namespace std;
    srand(static_cast<unsigned>(time(0)));

    // Test 1: 2x3 matrix * 3x1 vector
    {
        int rows = 2, cols = 3;
        double matrixRowMajor[] = {1, 2, 3, 4, 5, 6};
        double matrixColMajor[] = {1, 4, 2, 5, 3, 6};
        double vector[] = {1, 1, 1};
        double resultRow[2], resultCol[2];
        multiply_mv_row_major(matrixRowMajor, rows, cols, vector, resultRow);
        multiply_mv_col_major(matrixColMajor, rows, cols, vector, resultCol);
        cout << "Matrix-Vector multiplication (2x3 * 3x1) test:\n";
        cout << " Result (row-major) = [" << resultRow[0] << ", " << resultRow[1] << "]\n";
        cout << " Result (col-major) = [" << resultCol[0] << ", " << resultCol[1] << "]\n";
        cout << " Expected           = [6, 15]\n\n";
    }

    // Test 2: 2x2 matrix * 2x2 matrix
    {
        int n = 2;
        double A[] = {1, 2, 3, 4};
        double B[] = {5, 6, 7, 8};
        double B_trans[4];
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                B_trans[j * n + i] = B[i * n + j];
            }
        }
        double result_naive[4], result_trans[4], result_tiled[4];
        multiply_mm_naive(A, n, n, B, n, n, result_naive);
        multiply_mm_transposed_b(A, n, n, B_trans, n, n, result_trans);
        multiply_mm_tiled(A, n, n, B, n, n, result_tiled);
        cout << "Matrix-Matrix multiplication (2x2 * 2x2) test:\n";
        cout << " Result (naive)      = ["
             << result_naive[0] << ", " << result_naive[1] << ", "
             << result_naive[2] << ", " << result_naive[3] << "]\n";
        cout << " Result (transposed) = ["
             << result_trans[0] << ", " << result_trans[1] << ", "
             << result_trans[2] << ", " << result_trans[3] << "]\n";
        cout << " Result (tiled)      = ["
             << result_tiled[0] << ", " << result_tiled[1] << ", "
             << result_tiled[2] << ", " << result_tiled[3] << "]\n";
        cout << " Expected            = [19, 22, 43, 50]\n\n";
    }

    int runs = 5;
    // Define problem sizes for small, medium, large
    int N_small = 100, N_medium = 500, N_large = 1000;

    // Benchmark: Small matrix (100x100)
    {
        int N = N_small;
        cout << "Benchmarking with small matrix (N=" << N << "):\n";
        double *A = new double[N * N];
        double *A_col = new double[N * N];
        double *B = new double[N * N];
        double *B_trans = new double[N * N];
        double *vec = new double[N];
        double *resVec = new double[N];
        double *resVecC = new double[N];
        double *resM_naive = new double[N * N];
        double *resM_trans = new double[N * N];
        double *resM_tiled = new double[N * N];

        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                double valA = (double)rand() / RAND_MAX;
                double valB = (double)rand() / RAND_MAX;
                A[i * N + j] = valA;
                B[i * N + j] = valB;
                A_col[j * N + i] = valA;
                B_trans[j * N + i] = valB;
            }
            vec[i] = (double)rand() / RAND_MAX;
        }

        // 1. MV row-major
        auto start = chrono::high_resolution_clock::now();
        for (int r = 0; r < runs; ++r)
        {
            multiply_mv_row_major(A, N, N, vec, resVec);
        }
        auto end = chrono::high_resolution_clock::now();
        double avg_us = chrono::duration_cast<chrono::microseconds>(end - start).count() / (double)runs;
        g_sum += resVec[0]; // prevent optimization
        cout << " multiply_mv_row_major: " << avg_us << " microseconds (avg over " << runs << " runs)\n";

        // 2. MV column-major
        start = chrono::high_resolution_clock::now();
        for (int r = 0; r < runs; ++r)
        {
            multiply_mv_col_major(A_col, N, N, vec, resVecC);
        }
        end = chrono::high_resolution_clock::now();
        avg_us = chrono::duration_cast<chrono::microseconds>(end - start).count() / (double)runs;
        g_sum += resVecC[0];
        cout << " multiply_mv_col_major: " << avg_us << " microseconds (avg over " << runs << " runs)\n";

        // 3. MM naive
        start = chrono::high_resolution_clock::now();
        for (int r = 0; r < runs; ++r)
        {
            multiply_mm_naive(A, N, N, B, N, N, resM_naive);
        }
        end = chrono::high_resolution_clock::now();
        avg_us = chrono::duration_cast<chrono::microseconds>(end - start).count() / (double)runs;
        g_sum += resM_naive[0];
        cout << " multiply_mm_naive: " << avg_us << " microseconds (avg over " << runs << " runs)\n";

        // 4. MM with B transposed
        start = chrono::high_resolution_clock::now();
        for (int r = 0; r < runs; ++r)
        {
            multiply_mm_transposed_b(A, N, N, B_trans, N, N, resM_trans);
        }
        end = chrono::high_resolution_clock::now();
        avg_us = chrono::duration_cast<chrono::microseconds>(end - start).count() / (double)runs;
        g_sum += resM_trans[0];
        cout << " multiply_mm_transposed_b: " << avg_us << " microseconds (avg over " << runs << " runs)\n";

        // 5. MM tiled
        start = chrono::high_resolution_clock::now();
        for (int r = 0; r < runs; ++r)
        {
            multiply_mm_tiled(A, N, N, B, N, N, resM_tiled);
        }
        end = chrono::high_resolution_clock::now();
        avg_us = chrono::duration_cast<chrono::microseconds>(end - start).count() / (double)runs;
        g_sum += resM_tiled[0];
        cout << " multiply_mm_tiled: " << avg_us << " microseconds (avg over " << runs << " runs)\n\n";

        delete[] A;
        delete[] A_col;
        delete[] B;
        delete[] B_trans;
        delete[] vec;
        delete[] resVec;
        delete[] resVecC;
        delete[] resM_naive;
        delete[] resM_trans;
        delete[] resM_tiled;
    }

    // Benchmark: Medium matrix (500x500)
    {
        int N = N_medium;
        cout << "Benchmarking with medium matrix (N=" << N << "):\n";

        double *A = new double[N * N];
        double *A_col = new double[N * N];
        double *B = new double[N * N];
        double *B_trans = new double[N * N];
        double *vec = new double[N];
        double *resVec = new double[N];
        double *resVecC = new double[N];
        double *resM_naive = new double[N * N];
        double *resM_trans = new double[N * N];
        double *resM_tiled = new double[N * N];

        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                double valA = (double)rand() / RAND_MAX;
                double valB = (double)rand() / RAND_MAX;
                A[i * N + j] = valA;
                B[i * N + j] = valB;
                A_col[j * N + i] = valA;
                B_trans[j * N + i] = valB;
            }
            vec[i] = (double)rand() / RAND_MAX;
        }
        auto start = chrono::high_resolution_clock::now();
        for (int r = 0; r < runs; ++r)
        {
            multiply_mv_row_major(A, N, N, vec, resVec);
        }
        auto end = chrono::high_resolution_clock::now();
        double avg_us = chrono::duration_cast<chrono::microseconds>(end - start).count() / (double)runs;
        g_sum += resVec[0];
        cout << " multiply_mv_row_major: " << avg_us << " microseconds (avg over " << runs << " runs)\n";

        start = chrono::high_resolution_clock::now();
        for (int r = 0; r < runs; ++r)
        {
            multiply_mv_col_major(A_col, N, N, vec, resVecC);
        }
        end = chrono::high_resolution_clock::now();
        avg_us = chrono::duration_cast<chrono::microseconds>(end - start).count() / (double)runs;
        g_sum += resVecC[0];
        cout << " multiply_mv_col_major: " << avg_us << " microseconds (avg over " << runs << " runs)\n";

        start = chrono::high_resolution_clock::now();
        for (int r = 0; r < runs; ++r)
        {
            multiply_mm_naive(A, N, N, B, N, N, resM_naive);
        }
        end = chrono::high_resolution_clock::now();
        avg_us = chrono::duration_cast<chrono::microseconds>(end - start).count() / (double)runs;
        g_sum += resM_naive[0];
        cout << " multiply_mm_naive: " << avg_us << " microseconds (avg over " << runs << " runs)\n";

        start = chrono::high_resolution_clock::now();
        for (int r = 0; r < runs; ++r)
        {
            multiply_mm_transposed_b(A, N, N, B_trans, N, N, resM_trans);
        }
        end = chrono::high_resolution_clock::now();
        avg_us = chrono::duration_cast<chrono::microseconds>(end - start).count() / (double)runs;
        g_sum += resM_trans[0];
        cout << " multiply_mm_transposed_b: " << avg_us << " microseconds (avg over " << runs << " runs)\n";

        start = chrono::high_resolution_clock::now();
        for (int r = 0; r < runs; ++r)
        {
            multiply_mm_tiled(A, N, N, B, N, N, resM_tiled);
        }
        end = chrono::high_resolution_clock::now();
        avg_us = chrono::duration_cast<chrono::microseconds>(end - start).count() / (double)runs;
        g_sum += resM_tiled[0];
        cout << " multiply_mm_tiled: " << avg_us << " microseconds (avg over " << runs << " runs)\n\n";

        delete[] A;
        delete[] A_col;
        delete[] B;
        delete[] B_trans;
        delete[] vec;
        delete[] resVec;
        delete[] resVecC;
        delete[] resM_naive;
        delete[] resM_trans;
        delete[] resM_tiled;
    }

    // Benchmark: Large matrix (1000x1000)
    {
        int N = N_large;
        cout << "Benchmarking with large matrix (N=" << N << "):\n";
        double *A = new double[N * N];
        double *A_col = new double[N * N];
        double *B = new double[N * N];
        double *B_trans = new double[N * N];
        double *vec = new double[N];
        double *resVec = new double[N];
        double *resVecC = new double[N];
        double *resM_naive = new double[N * N];
        double *resM_trans = new double[N * N];
        double *resM_tiled = new double[N * N];
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                double valA = (double)rand() / RAND_MAX;
                double valB = (double)rand() / RAND_MAX;
                A[i * N + j] = valA;
                B[i * N + j] = valB;
                A_col[j * N + i] = valA;
                B_trans[j * N + i] = valB;
            }
            vec[i] = (double)rand() / RAND_MAX;
        }
        auto start = chrono::high_resolution_clock::now();
        for (int r = 0; r < runs; ++r)
        {
            multiply_mv_row_major(A, N, N, vec, resVec);
        }
        auto end = chrono::high_resolution_clock::now();
        double avg_us = chrono::duration_cast<chrono::microseconds>(end - start).count() / (double)runs;
        g_sum += resVec[0];
        cout << " multiply_mv_row_major: " << avg_us << " microseconds (avg over " << runs << " runs)\n";

        start = chrono::high_resolution_clock::now();
        for (int r = 0; r < runs; ++r)
        {
            multiply_mv_col_major(A_col, N, N, vec, resVecC);
        }
        end = chrono::high_resolution_clock::now();
        avg_us = chrono::duration_cast<chrono::microseconds>(end - start).count() / (double)runs;
        g_sum += resVecC[0];
        cout << " multiply_mv_col_major: " << avg_us << " microseconds (avg over " << runs << " runs)\n";

        start = chrono::high_resolution_clock::now();
        for (int r = 0; r < runs; ++r)
        {
            multiply_mm_naive(A, N, N, B, N, N, resM_naive);
        }
        end = chrono::high_resolution_clock::now();
        avg_us = chrono::duration_cast<chrono::microseconds>(end - start).count() / (double)runs;
        g_sum += resM_naive[0];
        cout << " multiply_mm_naive: " << avg_us << " microseconds (avg over " << runs << " runs)\n";

        start = chrono::high_resolution_clock::now();
        for (int r = 0; r < runs; ++r)
        {
            multiply_mm_transposed_b(A, N, N, B_trans, N, N, resM_trans);
        }
        end = chrono::high_resolution_clock::now();
        avg_us = chrono::duration_cast<chrono::microseconds>(end - start).count() / (double)runs;
        g_sum += resM_trans[0];
        cout << " multiply_mm_transposed_b: " << avg_us << " microseconds (avg over " << runs << " runs)\n";

        start = chrono::high_resolution_clock::now();
        for (int r = 0; r < runs; ++r)
        {
            multiply_mm_tiled(A, N, N, B, N, N, resM_tiled);
        }
        end = chrono::high_resolution_clock::now();
        avg_us = chrono::duration_cast<chrono::microseconds>(end - start).count() / (double)runs;
        g_sum += resM_tiled[0];
        cout << " multiply_mm_tiled: " << avg_us << " microseconds (avg over " << runs << " runs)\n\n";

        delete[] A;
        delete[] A_col;
        delete[] B;
        delete[] B_trans;
        delete[] vec;
        delete[] resVec;
        delete[] resVecC;
        delete[] resM_naive;
        delete[] resM_trans;
        delete[] resM_tiled;
    }

    return 0;
}