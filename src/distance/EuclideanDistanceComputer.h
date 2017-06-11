//
// Created by dima on 03.04.17.
//

#ifndef EUCLIDEANDISTANCECOMPUTER_EUCLIDEANDISTANCECOMPUTER_H
#define EUCLIDEANDISTANCECOMPUTER_EUCLIDEANDISTANCECOMPUTER_H

#include "../util/cblas_wraps.h"
#include <algorithm>

template<typename FLOAT>
class EuclideanDistanceComputer {
private:
    const FLOAT *base_vectors;
    FLOAT *base_vectors_sqr_norms;
    int base_vectors_count;
    int base_vectors_dim;
public:
    EuclideanDistanceComputer(FLOAT *base_vectors, const int base_vectors_count, const int base_vectors_dim)
            : base_vectors(
            base_vectors), base_vectors_count(base_vectors_count), base_vectors_dim(base_vectors_dim) {
        base_vectors_sqr_norms = new FLOAT[base_vectors_count];
        sqr_euclidean_norms(base_vectors, base_vectors_count, base_vectors_dim, base_vectors_sqr_norms);
    }

    /**
     *
     * @param query_vector
     * @param out_distances - like ndarray[base_vectors_count]
     */
    void computeSquaredDistances(const FLOAT *query_vector, FLOAT *out_distances) const {
        FLOAT query_sqr_norm = sqr_euclidean_norm(query_vector, base_vectors_dim);
        std::fill(out_distances, out_distances + base_vectors_count, query_sqr_norm);
        saxpy(1.0, base_vectors_sqr_norms, base_vectors_count, out_distances);
        sgemv(-2.0, base_vectors, base_vectors_count, base_vectors_dim, 1, query_vector, out_distances);
    }

    /**
     *
     * @param query_vectors - like ndarray[n_queries, base_vectors_dim]
     * @param n_queries
     * @param out_distances - like ndarray[n_queries, base_vectors_count]
     */
    void computePairwiseSquaredDistances(const FLOAT *query_vectors, const size_t n_queries,
                                         FLOAT *out_distances_matrix) const {
        for (size_t i = 0; i < n_queries; i++) {
            const FLOAT *query_vector = &query_vectors[i * base_vectors_dim];
            FLOAT *out_distances = &out_distances_matrix[i * base_vectors_count];
            computeSquaredDistances(query_vector, out_distances);
        }
    }

    ~EuclideanDistanceComputer() {
        delete[] base_vectors_sqr_norms;
    }

};


#endif //EUCLIDEANDISTANCECOMPUTER_EUCLIDEANDISTANCECOMPUTER_H
