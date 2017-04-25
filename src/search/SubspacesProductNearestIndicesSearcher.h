//
// Created by dima on 06.04.17.
//

#ifndef EUCLIDEANDISTANCECOMPUTER_SUBSPACESPRODUCTNEARESTINDICESSEARCHER_H
#define EUCLIDEANDISTANCECOMPUTER_SUBSPACESPRODUCTNEARESTINDICESSEARCHER_H

#include <vector>
#include "NearestIndicesSearcher.h"
#include "../util/SubspacedScalars.h"

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
    findNearestIndices(const SubspacedScalars<float> &subspacedDistances, SubspacedScalars<int> &subspacedNearestIndices) const {
        for (size_t i = 0; i < subspaceNearestIndicesSearchers.size(); i++) {
            subspaceNearestIndicesSearchers[i].findNearestIndices(subspacedDistances.getPointerToSubspace(i),
                                                                  subspacedDistances.vectors_count_in_each_subspace,
                                                                  subspacedNearestIndices.getPointerToSubspace(
                                                                          i),
                                                                  subspacedNearestIndices.vectors_count_in_each_subspace);
        }
    }

};


#endif //EUCLIDEANDISTANCECOMPUTER_SUBSPACESPRODUCTNEARESTINDICESSEARCHER_H
