//
// Created by dima on 03.04.17.
//

#ifndef SYMMETRICDISTANCECOMPUTER_H
#define SYMMETRICDISTANCECOMPUTER_H

#include <algorithm>

template<typename CODE, typename FLOAT>
class SymmetricDistanceComputer {
private:
    const CODE *base_vectors_codes;
    const size_t n_base_vectors;
    const FLOAT *centroids_pairwise_squared_distances;
    const size_t n_subspaces;
    const size_t n_centroids_in_each_subspace;
public:
    /**
     *
     * @param base_vectors_codes - like ndarray[n_base_vectors, n_subspaces]
     * @param n_base_vectors
     * @param centroids_pairwise_squared_distances - like ndarray[n_subspaces, n_centroids_in_each_subspace, n_centroids_in_each_subspace]
     * @param n_subspaces
     * @param n_centroids_in_each_subspace
     */
    SymmetricDistanceComputer(const CODE *base_vectors_codes, const size_t n_base_vectors,
                              const FLOAT *centroids_pairwise_squared_distances, const size_t n_subspaces,
                              const size_t n_centroids_in_each_subspace)
            : base_vectors_codes(base_vectors_codes), n_base_vectors(n_base_vectors),
              centroids_pairwise_squared_distances(
                      centroids_pairwise_squared_distances), n_subspaces(n_subspaces),
              n_centroids_in_each_subspace(n_centroids_in_each_subspace) {
    }

    /**
     *
     * @param query_vector_codes - like ndarray[n_subspaces]
     * @param out_distances - like ndarray[n_base_vectors]
     */
    void computeSquaredDistances(const CODE *query_vector_codes,
                                 FLOAT *out_distances) const {
        std::fill(out_distances, out_distances + n_base_vectors, 0);
        for (size_t subspace_num = 0; subspace_num < n_subspaces; subspace_num++) {
            const FLOAT *centroids_pairwise_squared_distances_matrix = &centroids_pairwise_squared_distances[
                    subspace_num *
                    n_centroids_in_each_subspace *
                    n_centroids_in_each_subspace];
            const CODE q_code = query_vector_codes[subspace_num];
            const CODE row_num = q_code * n_centroids_in_each_subspace;
            for (size_t base_vector_num = 0; base_vector_num < n_base_vectors; base_vector_num++) {
                const CODE b_code = base_vectors_codes[base_vector_num * n_subspaces + subspace_num];
                const float dist = centroids_pairwise_squared_distances_matrix[row_num +
                                                                               b_code];
                out_distances[base_vector_num] += dist;
            }
        }
    }

    /**
     *
     * @param query_vectors_codes - like ndarray[n_queries, n_subspaces]
     * @param n_queries
     * @param out_distances - like ndarray[n_queries, n_base_vectors]
     */
    void computePairwiseSquaredDistances(const CODE *query_vectors_codes, const size_t n_queries,
                                         FLOAT *out_distances_matrix) {
        for (size_t i = 0; i < n_queries; i++) {
            const CODE *query_vector_codes = &query_vectors_codes[i * n_subspaces];
            FLOAT *out_distances = &out_distances_matrix[i * n_base_vectors];
            computeSquaredDistances(query_vector_codes, out_distances);
        }
    }

};


#endif //SYMMETRICDISTANCECOMPUTER_H
