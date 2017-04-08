//
// Created by dima on 09.04.17.
//

#ifndef INVERTEDMULTIINDEXBUILDER_H
#define INVERTEDMULTIINDEXBUILDER_H


#include <cstring>
#include "InvertedMultiIndex.h"
#include "../../src/multiindex/MultiIndexUtil.h"
#include "../../src/util/array_utils.h"


class InvertedMultiIndexBuilder {

public:
    template<class IndexEntry>
    static InvertedMultiIndex<IndexEntry> *
    buildInvertedMultiIndex(IndexEntry *x, int x_len, int *X_centroid_indices, int subspaces_count,
                            int centroids_count_in_each_subspace) {
        //printf("buildInvertedMultiIndex: %d %d %d\n", x_len, subspaces_count, centroids_count_in_each_subspace);
        //fflush(stdout);
        MultiIndexUtil multiIndexUtil(subspaces_count, centroids_count_in_each_subspace);

        int m = subspaces_count;
        int *_multi_index = new int[m];

        int n_total_cells = 1;
        for (int i = 0; i < subspaces_count; i++) {
            n_total_cells *= centroids_count_in_each_subspace;
        }

        int *counts = new int[n_total_cells + 1];//+1 border cell
        memset(counts, 0, sizeof(int) * n_total_cells);

        for (int i = 0; i < x_len; i++) {
            int row_offset = i * subspaces_count;
            copy_array(&X_centroid_indices[row_offset], _multi_index, subspaces_count);
            int flatindex = multiIndexUtil.flat_index(_multi_index);
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
            copy_array(&X_centroid_indices[row_offset], _multi_index, subspaces_count);
            int flatindex = multiIndexUtil.flat_index(_multi_index);
            entries_list_ends[flatindex]--;

            entries[entries_list_ends[flatindex]] = x[i];
        }
        int *entries_list_starts = entries_list_ends;
        entries_list_starts[n_total_cells] = x_len;

        //print_array(entries, x_len, "%d ", false);

        delete[] _multi_index;

        InvertedMultiIndex<IndexEntry> *invertedMultiIndex = new InvertedMultiIndex<IndexEntry>(entries,
                                                                                                entries_list_starts,
                                                                                                subspaces_count,
                                                                                                centroids_count_in_each_subspace);

        return invertedMultiIndex;
    }
};


#endif //INVERTEDMULTIINDEXBUILDER_H
