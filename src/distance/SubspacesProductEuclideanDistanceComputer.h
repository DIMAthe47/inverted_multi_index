//
// Created by dima on 05.04.17.
//

#ifndef EUCLIDEANDISTANCECOMPUTER_SUBSPACESPRODUCTEUCLIDEANDISTANCECOMPUTER_H
#define EUCLIDEANDISTANCECOMPUTER_SUBSPACESPRODUCTEUCLIDEANDISTANCECOMPUTER_H

#include "EuclideanDistanceComputer.h"
#include "../util/SubspacedVectors.h"
#include <algorithm>
#include <vector>

class SubspacesProductEuclideanDistanceComputer {
private:
    std::vector<EuclideanDistanceComputer> subspaceEuclideanDistanceComputers;
    size_t base_vectors_count_in_each_subspace;
    size_t subspace_dim;
public:
    /**
     *
     * @param subspace_base_vectors - like ndarray[\p subspaces_count, \p base_vectors_count_in_each_subspace , \p subspace_dim]
     * @param subspaces_count
     * @param base_vectors_count_in_each_subspace
     * @param subspace_dim
     */
    SubspacesProductEuclideanDistanceComputer(float *subspace_base_vectors, size_t subspaces_count,
                                              size_t base_vectors_count_in_each_subspace,
                                              size_t subspace_dim) :
            base_vectors_count_in_each_subspace(base_vectors_count_in_each_subspace),
            subspace_dim(subspace_dim) {

        subspaceEuclideanDistanceComputers.reserve(subspaces_count);

        size_t n_floats_in_subspace = (base_vectors_count_in_each_subspace * subspace_dim);
        for (size_t i = 0; i < subspaces_count; i++) {
            size_t subspace_base_vectors_from = i * n_floats_in_subspace;
            subspaceEuclideanDistanceComputers.emplace_back(&subspace_base_vectors[subspace_base_vectors_from],
                                                            base_vectors_count_in_each_subspace, subspace_dim);
        }
    }

//    SubspacedVectors<float> subspacedVectors;
//
//    SubspacesProductEuclideanDistanceComputer(SubspacedVectors<float> &subspacedVectors) :
//            subspacedVectors(subspacedVectors) {
//
//        subspaceEuclideanDistanceComputers.reserve(subspacedVectors.subspaces_count);
//
//        size_t n_floats_in_subspace = (subspacedVectors.vectors_count_in_each_subspace *
//                                       subspacedVectors.subspace_vector_dim);
//        for (size_t i = 0; i < subspacedVectors.subspaces_count; i++) {
//            size_t subspace_base_vectors_from = i * n_floats_in_subspace;
//            subspaceEuclideanDistanceComputers.emplace_back(
//                    &subspacedVectors.subspaced_vectors[subspace_base_vectors_from],
//                    subspacedVectors.vectors_count_in_each_subspace, subspacedVectors.subspace_vector_dim);
//        }
//    }

    /**
     *
     * @param query_vector
     * @param out_subspace_distances_arr - like ndarray[subspaces_count, base_vectors_count_in_each_subspace]
     */
    void computeDistances(const float *query_vector, float *out_subspace_distances_arr) const {
        for (size_t i = 0; i < subspaceEuclideanDistanceComputers.size(); i++) {
            size_t subspace_query_from = i * subspace_dim;
            size_t out_subspace_distances_arr_from = i * base_vectors_count_in_each_subspace;
            subspaceEuclideanDistanceComputers[i].computeDistances(&query_vector[subspace_query_from],
                                                                   &out_subspace_distances_arr[out_subspace_distances_arr_from]);
        }
    }

};


#endif //EUCLIDEANDISTANCECOMPUTER_SUBSPACESPRODUCTEUCLIDEANDISTANCECOMPUTER_H
