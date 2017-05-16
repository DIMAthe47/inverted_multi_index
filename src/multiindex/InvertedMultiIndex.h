#ifndef INVERTEDMULTIINDEX_H
#define INVERTEDMULTIINDEX_H


/**
 *
 * @tparam IndexEntry - type of what would be stored(e.g int for vectorId, or some struct for vectorId+metaInfo
 */
template<class IndexEntry>
class InvertedMultiIndex {
public:
    const IndexEntry *entries;
    const int entries_count;
    const int *entries_list_starts;
    const int entries_list_starts_len;
    const int subspaces_count;
    const int centroids_count_in_each_subspace;
    bool manage_memory = true;

    /**
     *
     * @param entries
     * @param entries_list_starts - reference to pointer to show that InvertedMultiIndex takes responsibility to delete underlying array
     * @param subspaces_count
     * @param centroids_count_in_each_subspace
     */
    InvertedMultiIndex(IndexEntry *entries, int *entries_list_starts, int entries_list_starts_len,
                       const int subspaces_count,
                       const int centroids_count_in_each_subspace) :
            entries(entries),
            entries_count(entries_list_starts[entries_list_starts_len - 1]),
            entries_list_starts(entries_list_starts),
            entries_list_starts_len(entries_list_starts_len),
            subspaces_count(subspaces_count),
            centroids_count_in_each_subspace(centroids_count_in_each_subspace) {
//        int n_total_cells = 1;
//        for (int i = 0; i < subspaces_count; i++) {
//            n_total_cells *= centroids_count_in_each_subspace;
//        }
        //last element must be len(entries)
//        this->entries_count = entries_list_starts[entries_list_starts_len-1];
    }

    ~InvertedMultiIndex() {
        if (manage_memory) {
            delete[] entries;
            delete[] entries_list_starts;
        }
    }

};

#endif