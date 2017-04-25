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
    SubspacedScalars<float> *subspacedCentroidsDistances;
    SubspacedScalars<int> *subspacedNearestIndices;

public:
    InvertedMultiIndexSearcher(InvertedMultiIndex<IndexEntry> *invertedMultiIndex,
                               SubspacedVectors<float> &subspacedCentroids) :
            multiSequenceAlgorithm(invertedMultiIndex),
            subspacesProductEuclideanDistanceComputer(subspacedCentroids),
            subspacesProductNearestIndicesSearcher(invertedMultiIndex->entries_count,
                                                   invertedMultiIndex->subspaces_count),
            subspacedCentroidsDistances(SubspacedScalars<float>::allocateOneDimSubspacedVectors(subspacedCentroids)),
            subspacedNearestIndices(SubspacedScalars<int>::allocateOneDimSubspacedVectors(subspacedCentroids)) {

        assert(invertedMultiIndex->subspaces_count == subspacedCentroids.subspaces_count);
        assert(invertedMultiIndex->centroids_count_in_each_subspace ==
               subspacedCentroids.vectors_count_in_each_subspace);
    }

    void findNearest(const float *query_vector, IndexEntry *out_nearest_entries, const int nearest_entries_count) {
        subspacesProductEuclideanDistanceComputer.computeDistances(query_vector,
                                                                   subspacedCentroidsDistances->subspaced_vectors);
        subspacesProductNearestIndicesSearcher.findNearestIndices(*subspacedCentroidsDistances,
                                                                  *subspacedNearestIndices);
        multiSequenceAlgorithm.find_and_write_candidates(subspacedCentroidsDistances->subspaced_vectors,
                                                         subspacedNearestIndices->subspaced_vectors,
                                                         out_nearest_entries, nearest_entries_count);

    }

    ~InvertedMultiIndexSearcher() {
        delete subspacedCentroidsDistances;
        delete subspacedNearestIndices;
    }

};


#endif //IMAGE_PROCESSING_INVERTEDMULTIINDEXSEARCHER_H
