//
// Created by dima on 05.04.17.
//

#ifndef EUCLIDEANDISTANCECOMPUTER_NEARESTINDICESSEARCHER_H
#define EUCLIDEANDISTANCECOMPUTER_NEARESTINDICESSEARCHER_H


#include <cstdlib>
#include <algorithm>
#include "../util/array_utils.h"

template<typename FLOAT>
class NearestIndicesSearcher {

private:
    int *const indices_;
    const size_t max_n_nearest_;
public:
    NearestIndicesSearcher(size_t max_n_nearest_) : indices_(new int[max_n_nearest_]), max_n_nearest_(max_n_nearest_) {

    }

    void findNearestIndices(const FLOAT *distances, const size_t distances_count, int *out_nearest_indices,
                            const size_t n_nearest) const {
        for (size_t i = 0; i < max_n_nearest_; i++) {
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
            out_nearest_indices[i] = (int) indices_[i];
        }
    }

    void printDebugInfo(const FLOAT *distances, const size_t distances_count, int *out_nearest_indices,
                        const size_t n_nearest) const {
        printf("NearestIndicesSearcher.findNearestIndices\n");
        printf("    max_n_nearest_: %d\n", max_n_nearest_);
        printf("    distances_count: %d\n", distances_count);
        printf("    n_nearest: %d\n", n_nearest);
        printf("    distances: ");
        print_array(distances, distances_count, "%f ", false);
        printf("    indices_: ");
        print_array(indices_, max_n_nearest_, "%d ", false);
    }


    ~NearestIndicesSearcher() {
        delete[] indices_;
    }
};


#endif //EUCLIDEANDISTANCECOMPUTER_NEARESTINDICESSEARCHER_H
