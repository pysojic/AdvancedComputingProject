#pragma once

extern double g_sum;
extern int BLOCK_SIZE;

void multiply_mv_row_major(const double *matrix, int rows, int cols,
                           const double *vector, double *result)
{
    for (int i = 0; i < rows; ++i)
    {
        double sum = 0.0;
        for (int j = 0; j < cols; ++j)
        {
            sum += matrix[i * cols + j] * vector[j];
        }
        result[i] = sum;
    }
}

void multiply_mv_col_major(const double *matrix, int rows, int cols,
                           const double *vector, double *result)
{
    for (int i = 0; i < rows; ++i)
    {
        result[i] = 0.0;
    }
    for (int j = 0; j < cols; ++j)
    {
        double v_j = vector[j];
        for (int i = 0; i < rows; ++i)
        {
            result[i] += matrix[j * rows + i] * v_j;
        }
    }
}

void multiply_mm_naive(const double *matrixA, int rowsA, int colsA,
                       const double *matrixB, int rowsB, int colsB,
                       double *result)
{
    for (int i = 0; i < rowsA; ++i)
    {
        for (int j = 0; j < colsB; ++j)
        {
            double sum = 0.0;
            for (int k = 0; k < colsA; ++k)
            {
                sum += matrixA[i * colsA + k] * matrixB[k * colsB + j];
            }
            result[i * colsB + j] = sum;
        }
    }
}

void multiply_mm_transposed_b(const double *matrixA, int rowsA, int colsA,
                              const double *matrixB_transposed, int rowsB, int colsB,
                              double *result)
{
    for (int i = 0; i < rowsA; ++i)
    {
        for (int j = 0; j < colsB; ++j)
        {
            double sum = 0.0;
            for (int k = 0; k < colsA; ++k)
            {
                sum += matrixA[i * colsA + k] * matrixB_transposed[j * rowsB + k];
            }
            result[i * colsB + j] = sum;
        }
    }
}

void multiply_mm_tiled(const double *matrixA, int rowsA, int colsA,
                       const double *matrixB, int rowsB, int colsB,
                       double *result)
{
    int total = rowsA * colsB;
    for (int idx = 0; idx < total; ++idx)
    {
        result[idx] = 0.0;
    }
    for (int i0 = 0; i0 < rowsA; i0 += BLOCK_SIZE)
    {
        for (int j0 = 0; j0 < colsB; j0 += BLOCK_SIZE)
        {
            for (int k0 = 0; k0 < colsA; k0 += BLOCK_SIZE)
            {
                int i_end = (i0 + BLOCK_SIZE < rowsA ? i0 + BLOCK_SIZE : rowsA);
                int j_end = (j0 + BLOCK_SIZE < colsB ? j0 + BLOCK_SIZE : colsB);
                int k_end = (k0 + BLOCK_SIZE < colsA ? k0 + BLOCK_SIZE : colsA);
                for (int i = i0; i < i_end; ++i)
                {
                    for (int k = k0; k < k_end; ++k)
                    {
                        double Aik = matrixA[i * colsA + k];
                        for (int j = j0; j < j_end; ++j)
                        {
                            result[i * colsB + j] += Aik * matrixB[k * colsB + j];
                        }
                    }
                }
            }
        }
    }
}