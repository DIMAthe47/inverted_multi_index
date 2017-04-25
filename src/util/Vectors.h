//
// Created by Dima on 25.04.2017.
//

#ifndef IMAGE_PROCESSING_VECTORS_H
#define IMAGE_PROCESSING_VECTORS_H

#include <cstdlib>

template<typename T>
class Vectors {
public:
    T *vectors;
    int vectors_count;
    int vector_dim;

    Vectors(T *vectors, int vectors_count, int vector_dim) : vectors(vectors), vectors_count(vectors_count),
                                                             vector_dim(vector_dim) {}


    T *getPointerToVector(int vector_num) const {
        size_t subspace_vectors_from = vector_num * vector_dim;
        T *vector = &vectors[subspace_vectors_from];
        return vector;
    }

};


#endif //IMAGE_PROCESSING_VECTORS_H
