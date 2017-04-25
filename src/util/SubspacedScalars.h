//
// Created by dima on 09.04.17.
//

#ifndef IMAGE_PROCESSING_SUBSPACEDSCALARS_H
#define IMAGE_PROCESSING_SUBSPACEDSCALARS_H


#include <cstdio>
#include "SubspacedVectors.h"

/*
 * like ndarray[subspaced_vectors, vectors_count_in_each_subspace]
 */
template<typename T>
class SubspacedScalars : public SubspacedVectors<T> {

public:

    SubspacedScalars(int subspaces_count, int vectors_count_in_each_subspace) : SubspacedVectors<T>(subspaces_count,
                                                                                                    vectors_count_in_each_subspace,
                                                                                                    1) {}

    SubspacedScalars(T *subspaced_vectors, int subspaces_count, int vectors_count_in_each_subspace)
            : SubspacedVectors<T>(
            subspaced_vectors, subspaces_count,
            vectors_count_in_each_subspace,
            1) {}

    template<typename F>
    static SubspacedScalars<T> *allocateOneDimSubspacedVectors(SubspacedVectors<F> &subspacedVectors) {
        SubspacedScalars<T> *subspacedScalars = new SubspacedScalars(new T[subspacedVectors.getTotalVectorsCount()],
                                                                     subspacedVectors.subspaces_count,
                                                                     subspacedVectors.vectors_count_in_each_subspace);
        subspacedScalars->destructVectors = true;
        return subspacedScalars;
    }
};


#endif //IMAGE_PROCESSING_SUBSPACEDSCALARS_H
