#ifndef INVERTEDMULTIINDEX_H
#define INVERTEDMULTIINDEX_H


/**
 *
 * @tparam IndexEntry - type of what would be stored(e.g int for vectorId, or some struct for vectorId+metaInfo
 */
template<class IndexEntry>
struct InvertedMultiIndex {
    IndexEntry *entries;
    int entries_count;
    int *entries_list_starts;
    const int subspaces_count;
    const int centroids_count_in_each_subspace;

    InvertedMultiIndex(IndexEntry *entries, int *entries_list_starts, const int subspaces_count,
                       const int centroids_count_in_each_subspace) :
            entries(entries), entries_list_starts(entries_list_starts), subspaces_count(subspaces_count),
            centroids_count_in_each_subspace(centroids_count_in_each_subspace) {
        int n_total_cells = 1;
        for (int i = 0; i < subspaces_count; i++) {
            n_total_cells *= centroids_count_in_each_subspace;
        }
        //last element must be len(entries)
        this->entries_count = entries_list_starts[n_total_cells];
    }

    ~InvertedMultiIndex() {
        delete[] entries;
        delete[] entries_list_starts;
    }
};

#endif