//
// Created by dima on 03.04.17.
//

#ifndef ASYMMETRICDISTANCECOMPUTER_H
#define ASYMMETRICDISTANCECOMPUTER_H

#include <algorithm>
#include "SubspacesProductEuclideanDistanceComputer.h"
#include "../util/SubspacedVectors.h"

template<typename CODE, typename FLOAT>
class AsymmetricDistanceComputer {
private:
    const CODE *base_vectors_codes;
    const size_t n_base_vectors;
    const FLOAT *centroids_pairwise_squared_distances;
    const size_t n_subspaces;
    const size_t n_centroids_in_each_subspace;
    const size_t subspace_dim;
    const SubspacesProductEuclideanDistanceComputer<FLOAT> subspacesProductEuclideanDistanceComputer;
    FLOAT *subspaced_squared_distances;//like ndarray[n_subspaces, n_centroids_in_each_subspace]
public:
    /**
     *
     * @param base_vectors_codes - like ndarray[n_base_vectors, n_subspaces]
     * @param n_base_vectors
     * @param subspaced_centroids - like ndarray[n_subspaces, n_centroids_in_each_subspace, subspace_dim]
     * @param n_subspaces
     * @param n_centroids_in_each_subspace
     */
    AsymmetricDistanceComputer(const FLOAT *subspaced_centroids, const size_t n_subspaces,
                               const size_t n_centroids_in_each_subspace, const size_t subspace_dim,
                               const CODE *base_vectors_codes, const size_t n_base_vectors)
            : subspacesProductEuclideanDistanceComputer(
            SubspacedVectors<FLOAT>((FLOAT *) subspaced_centroids, n_subspaces, n_centroids_in_each_subspace,
                                    subspace_dim)),
              n_subspaces(n_subspaces),
              n_centroids_in_each_subspace(n_centroids_in_each_subspace),
              subspace_dim(subspace_dim), base_vectors_codes(base_vectors_codes), n_base_vectors(n_base_vectors) {
        subspaced_squared_distances = new FLOAT[n_subspaces * n_centroids_in_each_subspace];

    }

    /**
     *
     * @param query_vector - like ndarray[n_subspaces*subspaces_dim]
     * @param out_distances - like ndarray[n_base_vectors]
     */
    void computeSquaredDistances(const FLOAT *query_vector,
                                 FLOAT *out_distances) const {
        subspacesProductEuclideanDistanceComputer.computeSquaredDistances(query_vector, subspaced_squared_distances);

        std::fill(out_distances, out_distances + n_base_vectors, 0);
        for (size_t subspace_num = 0; subspace_num < n_subspaces; subspace_num++) {
            const FLOAT *subspace_squared_distances = &subspaced_squared_distances[subspace_num *
                                                                                   n_centroids_in_each_subspace];
            for (size_t base_vector_num = 0; base_vector_num < n_base_vectors; base_vector_num++) {
                const CODE b_code = base_vectors_codes[base_vector_num * n_subspaces + subspace_num];
                const float dist = subspace_squared_distances[b_code];
                out_distances[base_vector_num] += dist;
            }
        }
    }

    /**
     *
     * @param query_vectors - like ndarray[n_queries, n_subspaces]
     * @param n_queries
     * @param out_distances_matrix - like ndarray[n_queries, n_base_vectors]
     */
    void computePairwiseSquaredDistances(const FLOAT *query_vectors, const size_t n_queries,
                                         FLOAT *out_distances_matrix) {
        for (size_t i = 0; i < n_queries; i++) {
            const FLOAT *query_vector = &query_vectors[i * n_subspaces * subspace_dim];
            FLOAT *out_distances = &out_distances_matrix[i * n_base_vectors];
            computeSquaredDistances(query_vector, out_distances);
        }
    }

    ~AsymmetricDistanceComputer() {
        delete[] subspaced_squared_distances;
    }
};


#endif //ASYMMETRICDISTANCECOMPUTER_H
