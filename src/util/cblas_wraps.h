//
// Created by dima on 05.04.17.
//

#ifndef CBLASWRAPS_H
#define CBLASWRAPS_H

#include "cblas.h"

#ifdef _WIN32

#include <windows.h>

#endif

#include <stdio.h>

#include <stdlib.h>
#include <signal.h>

static inline void saxpy(const float alpha, const float *x, const int len, float *y) {
    cblas_saxpy(len, alpha, x, (OPENBLAS_CONST blasint) 1, y, (OPENBLAS_CONST blasint) 1);
}

static inline float dot_product(const float *x, const float *y, const int len) {
    float p = cblas_sdot(len, x, (OPENBLAS_CONST blasint) 1, y, (OPENBLAS_CONST blasint) 1);
    return p;
}

static inline float sqr_euclidean_norm(const float *x, const int len) {
    float n = cblas_sdot((OPENBLAS_CONST blasint) len, x, (OPENBLAS_CONST blasint) 1, x, (OPENBLAS_CONST blasint) 1);
    return n;
}

static inline void sqr_euclidean_norms(const float *a, const size_t rows, const size_t columns, float *out_sqr_norms) {
    for (size_t i = 0; i < rows; i++) {
        float sqr_norm = sqr_euclidean_norm(&a[i * columns], (const int) columns);
        out_sqr_norms[i] = sqr_norm;
    }
}

static inline void
sgemv(const float alpha, const float *a, const int rows, const int columns, const float beta, const float *x,
      float *y) {
    /*
    signal(SIGSEGV, posix_death_signal);
    float asdfg = 0;
    last_touched_arr = "a";
    for (size_t i = 0; i < columns * rows; i++) {
        last_touched_index = i;
        asdfg = a[i];
    }
   */
    cblas_sgemv(CBLAS_ORDER::CblasRowMajor, CBLAS_TRANSPOSE::CblasNoTrans, rows, columns, alpha, a, columns,
                x, (OPENBLAS_CONST blasint) 1, beta, y, (OPENBLAS_CONST blasint) 1);
}

#endif //EUCLIDEANDISTANCECOMPUTER_CBLAS_WRAPS_H
