//
// Created by dima on 09.04.17.
//

#ifndef IMAGE_PROCESSING_SUBSPACEDVECTORS_H
#define IMAGE_PROCESSING_SUBSPACEDVECTORS_H

/*
 * like ndarray[subspaces_count, vectors_count_in_each_subspace, subspace_vector_dim]
 */
template<typename T>
class SubspacedVectors {
protected:
    bool destructVectors = false;
public:
    T *subspaced_vectors;
    int subspaces_count;
    int vectors_count_in_each_subspace;
    int subspace_vector_dim;

    SubspacedVectors(int subspaces_count, int vectors_count_in_each_subspace,
                     int subspace_vector_dim) : subspaces_count(subspaces_count),
                                                vectors_count_in_each_subspace(vectors_count_in_each_subspace),
                                                subspace_vector_dim(subspace_vector_dim) {}

    SubspacedVectors(T *subspaced_vectors, int subspaces_count, int vectors_count_in_each_subspace,
                     int subspace_vector_dim) : subspaced_vectors(subspaced_vectors), subspaces_count(subspaces_count),
                                                vectors_count_in_each_subspace(vectors_count_in_each_subspace),
                                                subspace_vector_dim(subspace_vector_dim) {}

    void setSubspacedVectors(T *subspaced_vectors) {
        this->subspaced_vectors = subspaced_vectors;
    }

    int getTotalVectorsCount() const {
        int total_vectors_count = subspaces_count * vectors_count_in_each_subspace;
        return total_vectors_count;
    }

    int getTotalComponentsCount() const {
        int total_components_count = subspaces_count * vectors_count_in_each_subspace * subspace_vector_dim;
        return total_components_count;
    }

    int getComponentsCountInSubspace() const {
        int components_count_in_subspace = vectors_count_in_each_subspace * subspace_vector_dim;
        return components_count_in_subspace;
    }

    T *getPointerToSubspace(size_t subspace_num) const {
        size_t subspace_vectors_from = subspace_num * getComponentsCountInSubspace();
        T *subspace_vectors = &subspaced_vectors[subspace_vectors_from];
        return subspace_vectors;
    }

    ~SubspacedVectors() {
        if (destructVectors) {
            delete[] subspaced_vectors;
        }
    }
};


#endif //IMAGE_PROCESSING_SUBSPACEDVECTORS_H
