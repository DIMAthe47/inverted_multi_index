//
// Created by dima on 05.04.17.
//

#ifndef EUCLIDEANDISTANCECOMPUTER_NEARESTINDICESSEARCHER_H
#define EUCLIDEANDISTANCECOMPUTER_NEARESTINDICESSEARCHER_H


#include <cstdlib>
#include <algorithm>


class NearestIndicesSearcher {

private:
    int *const indices_;
    const size_t max_n_nearest_;
public:
    NearestIndicesSearcher(size_t max_n_nearest_) : indices_(new int[max_n_nearest_]), max_n_nearest_(max_n_nearest_) {

    }

    void findNearestIndices(const float *distances, const size_t distances_count, int *out_nearest_indices,
                            const size_t n_nearest) const {
        for (size_t i = 0; i < max_n_nearest_; i++) {
            indices_[i] = i;
        }
        std::partial_sort(indices_, indices_ + n_nearest, indices_ + distances_count,
                          [&distances](const int &index1, const int &index2) -> bool {
                              return distances[index1] < distances[index2];
                          });

        for (size_t i = 0; i < n_nearest; i++) {
            out_nearest_indices[i] = indices_[i];
        }
    }

    ~NearestIndicesSearcher() {
        delete[] indices_;
    }
};


#endif //EUCLIDEANDISTANCECOMPUTER_NEARESTINDICESSEARCHER_H
