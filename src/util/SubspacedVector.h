//
// Created by dima on 09.04.17.
//

#ifndef IMAGE_PROCESSING_SUBSPACEDVECTOR_H
#define IMAGE_PROCESSING_SUBSPACEDVECTOR_H

#include "SubspacedVectors.h"

/*
 * like ndarray[subspaces_count, subspace_vector_dim]
 */
template<typename T>
class SubspacedVector : public SubspacedVectors<T> {

public:

    SubspacedVector(int subspaces_count, int subspace_vector_dim) : SubspacedVectors<T>(subspaces_count,
                                                                                        1,
                                                                                        subspace_vector_dim) {}

    SubspacedVector(T *subspaced_vector, int subspaces_count, int subspace_vector_dim)
            : SubspacedVectors<T>(
            subspaced_vector, subspaces_count,
            1,
            subspace_vector_dim) {}
};


#endif //IMAGE_PROCESSING_SUBSPACEDVECTOR_H
