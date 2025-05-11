#ifndef TIME_SERIES_H
#define TIME_SERIES_H

#include <cstddef>
#include <immintrin.h>

inline void moving_average_naive(const double* data, double* out, size_t n, size_t w){
    for(size_t i=0;i + w<=n;++i){
        double sum=0;
        for(size_t j=0;j<w;++j) sum+=data[i+j];
        out[i]=sum/w;
    }
}

inline void moving_average_simd(const double* data, double* out, size_t n, size_t w){
    size_t limit = n - w + 1;
    size_t w4 = w - (w % 4);
    for(size_t i=0;i<limit;++i){
        __m256d vsum = _mm256_setzero_pd();
        size_t j=0;
        for(;j<w4;j+=4)
            vsum = _mm256_add_pd(vsum, _mm256_loadu_pd(data + i + j));
        double tmp[4];
        _mm256_storeu_pd(tmp, vsum);
        double s = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        for(;j<w;++j) s += data[i+j];
        out[i] = s / w;
    }
}

#endif
