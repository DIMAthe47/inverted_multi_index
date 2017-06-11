//
// Created by dima on 05.04.17.
//

#ifndef EUCLIDEANDISTANCECOMPUTER_NEARESTINDICESSEARCHER_H
#define EUCLIDEANDISTANCECOMPUTER_NEARESTINDICESSEARCHER_H


#include <cstdlib>
#include <algorithm>
#include "../util/array_utils.h"
#include "../util/common_defs.h"
#include <assert.h>

template<typename FLOAT>
class NearestIndicesSearcher {

private:
    int *const indices_;
    const size_t max_distances_count_;
public:
    NearestIndicesSearcher(const size_t max_n_nearest_) : indices_(new int[max_n_nearest_]),
                                                          max_distances_count_(max_n_nearest_) {

    }

    void findNearestIndices(const FLOAT *distances, const size_t distances_count, int *out_nearest_indices,
                            const size_t n_nearest) const {
        for (size_t i = 0; i < distances_count; i++) {
            indices_[i] = (int) i;
        }
        std::partial_sort(indices_, indices_ + n_nearest, indices_ + distances_count,
                          [&distances](const int &index1, const int &index2) -> bool {
                              return distances[index1] < distances[index2];
                          });
#ifdef MY_DEBUG
        printDebugInfo(distances, distances_count, out_nearest_indices, n_nearest);
#endif

        for (size_t i = 0; i < n_nearest; i++) {
            out_nearest_indices[i] = indices_[i];
        }
    }

    /**
     *
     * @param distances_matrix - like ndarray[height_distances_matrix, width_distances_matrix]. distances_matrix[0,:] - distances from vector q to <width_distances_matrix> vectors
     * @param width_distances_matrix
     * @param height_distances_matrix
     * @param out_nearest_indices_matrix - like ndarray[height_distances_matrix, n_nearest].
     * @param n_nearest
     */
    void findNearestIndices(const FLOAT *distances_matrix, const size_t width_distances_matrix,
                            const size_t height_distances_matrix, int *out_nearest_indices_matrix,
                            const size_t n_nearest) const {
        for (size_t row_num = 0; row_num < height_distances_matrix; row_num++) {

#ifdef MY_DEBUG
            printDebugInfo(distances_matrix, width_distances_matrix,
                           height_distances_matrix, out_nearest_indices_matrix, n_nearest);
#endif
            const FLOAT *distances = &distances_matrix[row_num * width_distances_matrix];
            int *out_nearest_indices = &out_nearest_indices_matrix[row_num * n_nearest];
            findNearestIndices(distances, width_distances_matrix, out_nearest_indices, n_nearest);
        }
    }

    void printDebugInfo(const FLOAT *distances, const size_t distances_count, int *out_nearest_indices,
                        const size_t n_nearest) const {
        printf("NearestIndicesSearcher.findNearestIndices\n");
        printf("    max_distances_count_: %zu\n", max_distances_count_);
        printf("    distances_count: %zu\n", distances_count);
        printf("    n_nearest: %zu\n", n_nearest);
        printf("    distances: ");
        print_array(distances, distances_count, "%f ", false);
        printf("    indices_: ");
        print_array(indices_, max_distances_count_, "%d ", false);
    }

    void printDebugInfo(const FLOAT *distances_matrix, const size_t width_distances_matrix,
                        const size_t height_distances_matrix, int *out_nearest_indices_matrix,
                        const size_t n_nearest) const {
        printf("NearestIndicesSearcher.findNearestIndices\n");
        printf("    max_distances_count_: %zu\n", max_distances_count_);
        printf("    width_distances_matrix: %zu\n", width_distances_matrix);
        printf("    height_distances_matrix: %zu\n", height_distances_matrix);
        printf("    n_nearest: %zu\n", n_nearest);
    }


    ~NearestIndicesSearcher() {
        delete[] indices_;
    }
};


#endif //EUCLIDEANDISTANCECOMPUTER_NEARESTINDICESSEARCHER_H
