from libcpp cimport bool
cimport cython

cdef extern from "util/array_utils.h":
    cdef void c_delete(void* c_arr)
    cdef void c_delete(const void* c_arr)

cdef extern from "util/SubspacedVectors.h":
    cdef cppclass SubspacedVectors[T]:
        SubspacedVectors(T *subspaced_vectors, int subspaces_count, int vectors_count_in_each_subspace,
                     int subspace_vector_dim)

cdef extern from "util/Vectors.h":
    cdef cppclass Vectors[T]:
        Vectors(T *vectors, int vectors_count, int vector_dim)

cdef extern from "multiindex/InvertedMultiIndex.h":
    cdef cppclass InvertedMultiIndex[IndexEntry]:
        const IndexEntry *entries
        const int entries_count
        const int *entries_list_starts
        const int entries_list_starts_len
        const int subspaces_count
        const int centroids_count_in_each_subspace
        bool manage_memory
        InvertedMultiIndex(IndexEntry *entries, int *entries_list_starts, int entries_list_starts_len,
                       const int subspaces_count,
                       const int centroids_count_in_each_subspace)

cdef extern from "multiindex/InvertedMultiIndexBuilder.h":
    cdef cppclass InvertedMultiIndexBuilder[IndexEntry]:
        InvertedMultiIndex[IndexEntry]* buildInvertedMultiIndex(IndexEntry *x, Vectors[float] xVectors, SubspacedVectors[float] subspacedCentroids)

cdef extern from "multiindex/InvertedMultiIndexSearcher.h":
    cdef cppclass InvertedMultiIndexSearcher[IndexEntry, FLOAT]:
        InvertedMultiIndexSearcher(InvertedMultiIndex[IndexEntry]* invertedMultiIndex, SubspacedVectors[FLOAT] &subspacedCentroids)
        void findNearest(const FLOAT *query_vector, IndexEntry *out_nearest_entries, const int nearest_entries_count)
