//
// Created by dima on 09.04.17.
//

#ifndef IMAGE_PROCESSING_SUBSPACEDVECTORS_H
#define IMAGE_PROCESSING_SUBSPACEDVECTORS_H


template<typename T>
struct SubspacedVectors {
    T *subspaced_vectors;
    int subspaces_count;
    int vectors_count_in_each_subspace;
    int subspace_vector_dim;
};


#endif //IMAGE_PROCESSING_SUBSPACEDVECTORS_H
