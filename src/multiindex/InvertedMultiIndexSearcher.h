//
// Created by dima on 07.04.17.
//

#ifndef IMAGE_PROCESSING_INVERTEDMULTIINDEXSEARCHER_H
#define IMAGE_PROCESSING_INVERTEDMULTIINDEXSEARCHER_H

#include <assert.h>
#include "MultiSequenceAlgorithm.h"
#include "../distance/SubspacesProductEuclideanDistanceComputer.h"
#include "../search/SubspacesProductNearestIndicesSearcher.h"
#include "../util/common_defs.h"

template<class IndexEntry, typename FLOAT>
class InvertedMultiIndexSearcher {
private:
    MultiSequenceAlgorithm<IndexEntry> multiSequenceAlgorithm;
    SubspacesProductEuclideanDistanceComputer<FLOAT> subspacesProductEuclideanDistanceComputer;
    SubspacesProductNearestIndicesSearcher<FLOAT> subspacesProductNearestIndicesSearcher;
    SubspacedScalars<FLOAT> *subspacedCentroidsDistances;
    SubspacedScalars<int> *subspacedNearestIndices;

public:
    InvertedMultiIndexSearcher(InvertedMultiIndex<IndexEntry> *invertedMultiIndex,
                               SubspacedVectors<FLOAT> &subspacedCentroids) :
            multiSequenceAlgorithm(invertedMultiIndex),
            subspacesProductEuclideanDistanceComputer(subspacedCentroids),
            subspacesProductNearestIndicesSearcher(subspacedCentroids.vectors_count_in_each_subspace,
                                                   invertedMultiIndex->subspaces_count),
            subspacedCentroidsDistances(SubspacedScalars<FLOAT>::allocateOneDimSubspacedVectors(subspacedCentroids)),
            subspacedNearestIndices(SubspacedScalars<int>::allocateOneDimSubspacedVectors(subspacedCentroids)) {

#ifdef MY_DEBUG
        printf("subspacedCount: %d, entriesCount: %d, centroidCountInEachSubspace: %d",
               invertedMultiIndex->subspaces_count, invertedMultiIndex->entries_count,
               invertedMultiIndex->centroids_count_in_each_subspace);
        printf(" \n");
#endif

        assert(invertedMultiIndex->subspaces_count == subspacedCentroids.subspaces_count);
        assert(invertedMultiIndex->centroids_count_in_each_subspace ==
               subspacedCentroids.vectors_count_in_each_subspace);
    }

    void findNearest(const FLOAT *query_vector, IndexEntry *out_nearest_entries, const int nearest_entries_count) {
#ifdef MY_DEBUG
        printf("findNearest \n");
        printf("computeDistances  \n");
#endif
        subspacesProductEuclideanDistanceComputer.computeSquaredDistances(query_vector,
                                                                          subspacedCentroidsDistances->subspaced_vectors);
#ifdef MY_DEBUG
        printf("findNearestIndices  \n");
#endif
        subspacesProductNearestIndicesSearcher.findNearestIndices(*subspacedCentroidsDistances,
                                                                  *subspacedNearestIndices);

#ifdef MY_DEBUG
        printf("subspacedCentroidsDistances \n");
        print_array(subspacedCentroidsDistances->subspaced_vectors,
                    subspacedCentroidsDistances->getTotalComponentsCount(), "%f ", false);
        printf("subspacedNearestIndices \n");
        print_array(subspacedNearestIndices->subspaced_vectors, subspacedNearestIndices->getTotalComponentsCount(),
                    "%d ", false);
        printf("find_and_write_candidates  \n");
#endif

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
