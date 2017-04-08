//
// Created by dima on 07.04.17.
//

#ifndef IMAGE_PROCESSING_INVERTEDMULTIINDEXSEARCHER_H
#define IMAGE_PROCESSING_INVERTEDMULTIINDEXSEARCHER_H

#include <assert.h>
#include "MultiSequenceAlgorithm.h"
#include "../distance/SubspacesProductEuclideanDistanceComputer.h"
#include "../search/SubspacesProductNearestIndicesSearcher.h"

template<class IndexEntry>
class InvertedMultiIndexSearcher {
private:
    MultiSequenceAlgorithm<IndexEntry> multiSequenceAlgorithm;
    SubspacesProductEuclideanDistanceComputer subspacesProductEuclideanDistanceComputer;
    SubspacesProductNearestIndicesSearcher subspacesProductNearestIndicesSearcher;
    float *subspace_centroids_distances_matrix_;
    int *nearest_subspace_cluster_indices_;
    int centroids_count_in_each_subspace;

public:
    InvertedMultiIndexSearcher(InvertedMultiIndex<IndexEntry> *invertedMultiIndex, float *subspaces_centroids,
                               size_t subspace_centroid_len) :
            centroids_count_in_each_subspace(centroids_count_in_each_subspace),
            multiSequenceAlgorithm(invertedMultiIndex),
            subspacesProductEuclideanDistanceComputer(subspaces_centroids, subspaces_count, centroids_count_in_each_subspace,
                                                      subspace_centroid_len),
            subspacesProductNearestIndicesSearcher(invertedMultiIndex->entries_count, subspaces_count) {

        assert(invertedMultiIndex->subspaces_count == subspaces_count);
        assert(invertedMultiIndex->centroids_count_in_each_subspace == centroids_count_in_each_subspace);

        subspace_centroids_distances_matrix_ = new float[subspaces_count * centroids_count_in_each_subspace];
        nearest_subspace_cluster_indices_ = new int[subspaces_count * centroids_count_in_each_subspace];
    }

    void findNearest(const float *query_vector, IndexEntry *out_nearest_entries, const int nearest_entries_count) {

        subspacesProductEuclideanDistanceComputer.computeDistances(query_vector, subspace_centroids_distances_matrix_);
        subspacesProductNearestIndicesSearcher.findNearestIndices(subspace_centroids_distances_matrix_,
                                                                  centroids_count_in_each_subspace,
                                                                  nearest_subspace_cluster_indices_,
                                                                  centroids_count_in_each_subspace);
        multiSequenceAlgorithm.find_and_write_candidates(subspace_centroids_distances_matrix_,
                                                         nearest_subspace_cluster_indices_,
                                                         out_nearest_entries, nearest_entries_count);

    }


};


#endif //IMAGE_PROCESSING_INVERTEDMULTIINDEXSEARCHER_H
