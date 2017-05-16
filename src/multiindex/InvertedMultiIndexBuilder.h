//
// Created by dima on 09.04.17.
//

#ifndef INVERTEDMULTIINDEXBUILDER_H
#define INVERTEDMULTIINDEXBUILDER_H

#include <cstring>
#include "InvertedMultiIndex.h"
#include "../../src/multiindex/MultiIndexUtil.h"
#include "../../src/multiindex/MultiIndexUtil2.h"
#include "../../src/util/array_utils.h"
#include "../util/SubspacedVectors.h"
#include "../util/SubspacedScalars.h"
#include "../distance/SubspacesProductEuclideanDistanceComputer.h"
#include "../search/SubspacesProductNearestIndicesSearcher.h"
#include "../util/Vectors.h"

template<class IndexEntry>
class InvertedMultiIndexBuilder {
public:

    InvertedMultiIndex<IndexEntry>*
    buildInvertedMultiIndex(IndexEntry *x, int x_len, int *X_centroid_indices, int subspaces_count,
                            int centroids_count_in_each_subspace) {
        //printf("buildInvertedMultiIndex: %d %d %d\n", x_len, subspaces_count, centroids_count_in_each_subspace);
        //fflush(stdout);
        MultiIndexUtil2 multiIndexUtil(subspaces_count, centroids_count_in_each_subspace);

        int m = subspaces_count;

        int n_total_cells = 1;
        for (int i = 0; i < subspaces_count; i++) {
            n_total_cells *= centroids_count_in_each_subspace;
        }
        int *counts = new int[n_total_cells + 1];//+1 border cell
        memset(counts, 0, sizeof(int) * n_total_cells);

        for (int i = 0; i < x_len; i++) {
            int row_offset = i * subspaces_count;
            copy_array(&X_centroid_indices[row_offset], multiIndexUtil.multi_index_ptr(), subspaces_count);
            int flatindex = multiIndexUtil.flat_index();
            counts[flatindex]++;
        }
        //print_array(counts, n_total_cells, "%d ");
        for (int i = 1; i < n_total_cells; i++) {
            counts[i] = counts[i - 1] + counts[i];
        }
        //print_array(counts, n_total_cells, "%d ");

        int *entries_list_ends = counts;
        IndexEntry *entries = new IndexEntry[x_len];
        for (int i = 0; i < x_len; i++) {
            int row_offset = i * subspaces_count;
            copy_array(&X_centroid_indices[row_offset], multiIndexUtil.multi_index_ptr(), subspaces_count);
            int flatindex = multiIndexUtil.flat_index();
            entries_list_ends[flatindex]--;

            entries[entries_list_ends[flatindex]] = x[i];
        }
        int *entries_list_starts = entries_list_ends;
        entries_list_starts[n_total_cells] = x_len;

        //print_array(entries, x_len, "%d ", false);


        InvertedMultiIndex<IndexEntry>* invertedMultiIndex=new InvertedMultiIndex<IndexEntry>(entries,
                                                          entries_list_starts,
                                                          n_total_cells + 1,
                                                          subspaces_count,
                                                          centroids_count_in_each_subspace);
        return invertedMultiIndex;
    }


    InvertedMultiIndex<IndexEntry>*
    buildInvertedMultiIndex(IndexEntry *x, Vectors<float> xVectors, SubspacedVectors<float> subspacedCentroids) {
        SubspacesProductEuclideanDistanceComputer<float> subspacesProductEuclideanDistanceComputer(subspacedCentroids);
        SubspacesProductNearestIndicesSearcher<float> subspacesProductNearestIndicesSearcher(
                subspacedCentroids.vectors_count_in_each_subspace, subspacedCentroids.subspaces_count);

        SubspacedScalars<float> *subspacedCentroidsDistances(
                SubspacedScalars<float>::allocateOneDimSubspacedVectors(subspacedCentroids));
//        SubspacedScalars<int> *subspacedNearestIndices(
//                SubspacedScalars<int>::allocateOneDimSubspacedVectors(subspacedCentroids));

        int *X_centroid_indices = new int[xVectors.vectors_count * subspacedCentroids.subspaces_count];

        SubspacedScalars<int> nearestIndices(subspacedCentroids.subspaces_count, 1);
        for (int i = 0; i < xVectors.vectors_count; i++) {
            SubspacedVector<float> vector(xVectors.getPointerToVector(i), subspacedCentroids.subspaces_count,
                                          subspacedCentroids.subspace_vector_dim);
            subspacesProductEuclideanDistanceComputer.computeDistances(vector,
                                                                       *subspacedCentroidsDistances);

            int row_offset = i * subspacedCentroids.subspaces_count;
            nearestIndices.setSubspacedVectors(&X_centroid_indices[row_offset]);

            subspacesProductNearestIndicesSearcher.findNearestIndices(*subspacedCentroidsDistances,
                                                                      nearestIndices);
        }

        InvertedMultiIndex<IndexEntry>* invertedMultiIndex = buildInvertedMultiIndex(x, xVectors.vectors_count,
                                                                                     X_centroid_indices,
                                                                                     subspacedCentroids.subspaces_count,
                                                                                     subspacedCentroids.vectors_count_in_each_subspace);
        return invertedMultiIndex;
    }
};


#endif //INVERTEDMULTIINDEXBUILDER_H
