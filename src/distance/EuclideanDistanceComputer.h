//
// Created by dima on 03.04.17.
//

#ifndef EUCLIDEANDISTANCECOMPUTER_EUCLIDEANDISTANCECOMPUTER_H
#define EUCLIDEANDISTANCECOMPUTER_EUCLIDEANDISTANCECOMPUTER_H

#include "../util/cblas_wraps.h"
#include <algorithm>

class EuclideanDistanceComputer {
private:
    const float *base_vectors;
    float *base_vectors_sqr_norms;
    int base_vectors_count;
    int base_vectors_dim;
public:
    EuclideanDistanceComputer(float *base_vectors, const int base_vectors_count,const int base_vectors_dim) : base_vectors(
            base_vectors), base_vectors_count(base_vectors_count), base_vectors_dim(base_vectors_dim) {
        base_vectors_sqr_norms = new float[base_vectors_count];
        sqr_euclidean_norms(base_vectors, base_vectors_count, base_vectors_dim, base_vectors_sqr_norms);
    }

    /**
     *
     * @param query_vector
     * @param out_distances - like ndarray[base_vectors_count]
     */
    void computeDistances(const float *query_vector, float *out_distances) const {
        float query_sqr_norm = sqr_euclidean_norm(query_vector, base_vectors_dim);
        std::fill(out_distances, out_distances + base_vectors_count, query_sqr_norm);
        saxpy(1.0, base_vectors_sqr_norms, base_vectors_count, out_distances);
        sgemv(-2.0, base_vectors, base_vectors_count, base_vectors_dim, 1, query_vector, out_distances);
    }

    ~EuclideanDistanceComputer() {
        delete[] base_vectors_sqr_norms;
    }

};


#endif //EUCLIDEANDISTANCECOMPUTER_EUCLIDEANDISTANCECOMPUTER_H
