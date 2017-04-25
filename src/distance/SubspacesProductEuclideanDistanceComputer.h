//
// Created by dima on 05.04.17.
//

#ifndef EUCLIDEANDISTANCECOMPUTER_SUBSPACESPRODUCTEUCLIDEANDISTANCECOMPUTER_H
#define EUCLIDEANDISTANCECOMPUTER_SUBSPACESPRODUCTEUCLIDEANDISTANCECOMPUTER_H

#include "EuclideanDistanceComputer.h"
#include "../util/SubspacedVectors.h"
#include "../util/SubspacedScalars.h"
#include "../util/SubspacedVector.h"
#include <algorithm>
#include <vector>

class SubspacesProductEuclideanDistanceComputer {
private:
    std::vector<EuclideanDistanceComputer> subspaceEuclideanDistanceComputers;
    SubspacedVectors<float> subspacedVectors;
public:

    SubspacesProductEuclideanDistanceComputer(SubspacedVectors<float> &subspacedVectors) :
            subspacedVectors(subspacedVectors) {

        subspaceEuclideanDistanceComputers.reserve(subspacedVectors.subspaces_count);
        for (size_t i = 0; i < subspacedVectors.subspaces_count; i++) {
            subspaceEuclideanDistanceComputers.emplace_back(
                    subspacedVectors.getPointerToSubspace(i),
                    subspacedVectors.vectors_count_in_each_subspace, subspacedVectors.subspace_vector_dim);
        }
    }

    /**
     *
     * @param query_vector - like ndarray[subspaces_count, subspace_vector_dim]
     * @param out_subspace_distances_arr - like ndarray[subspaces_count, base_vectors_count_in_each_subspace]
     */
    void computeDistances(const float *query_vector, float *out_subspace_distances_arr) const {
        for (size_t i = 0; i < subspaceEuclideanDistanceComputers.size(); i++) {
            size_t subspace_query_from = i * subspacedVectors.subspace_vector_dim;
            size_t out_subspace_distances_arr_from = i * subspacedVectors.vectors_count_in_each_subspace;
            subspaceEuclideanDistanceComputers[i].computeDistances(&query_vector[subspace_query_from],
                                                                   &out_subspace_distances_arr[out_subspace_distances_arr_from]);
        }
    }

    /**
     *
     * @param subspacedQueryVector - like ndarray[subspaces_count, subspace_vector_dim]
     * @param subspacedDistances - like ndarray[subspaces_count, base_vectors_count_in_each_subspace]
     */
    void computeDistances(const SubspacedVector<float> &subspacedQueryVector,
                          const SubspacedScalars<float> &subspacedDistances) const {
        for (size_t i = 0; i < subspaceEuclideanDistanceComputers.size(); i++) {
            subspaceEuclideanDistanceComputers[i].computeDistances(subspacedQueryVector.getPointerToSubspace(i),
                                                                   subspacedDistances.getPointerToSubspace(i));
        }
    }

};


#endif //EUCLIDEANDISTANCECOMPUTER_SUBSPACESPRODUCTEUCLIDEANDISTANCECOMPUTER_H
