//
// Created by dima on 06.04.17.
//

#ifndef EUCLIDEANDISTANCECOMPUTER_SUBSPACESPRODUCTNEARESTINDICESSEARCHER_H
#define EUCLIDEANDISTANCECOMPUTER_SUBSPACESPRODUCTNEARESTINDICESSEARCHER_H

#include <vector>
#include "NearestIndicesSearcher.h"

class SubspacesProductNearestIndicesSearcher {
private:
    std::vector<NearestIndicesSearcher> subspaceNearestIndicesSearchers;
public:
    SubspacesProductNearestIndicesSearcher(size_t max_n_nearest, size_t subspaces_count) {
        subspaceNearestIndicesSearchers.reserve(subspaces_count);
        for (size_t i = 0; i < subspaces_count; i++) {
            subspaceNearestIndicesSearchers.emplace_back(max_n_nearest);
        }
    }

    void
    findNearestIndices(const float *subspace_distances, const size_t distances_count_in_each_subspace,
                       int *out_subspace_nearest_indices,
                       const size_t n_nearest_in_each_subspace) const {
        for (size_t i = 0; i < subspaceNearestIndicesSearchers.size(); i++) {
            size_t subspace_distances_from = i * distances_count_in_each_subspace;
            size_t out_nearest_indices_from = i * n_nearest_in_each_subspace;
            subspaceNearestIndicesSearchers[i].findNearestIndices(&subspace_distances[subspace_distances_from],
                                                                  distances_count_in_each_subspace,
                                                                  &out_subspace_nearest_indices[out_nearest_indices_from],
                                                                  n_nearest_in_each_subspace);
        }
    }

};


#endif //EUCLIDEANDISTANCECOMPUTER_SUBSPACESPRODUCTNEARESTINDICESSEARCHER_H
