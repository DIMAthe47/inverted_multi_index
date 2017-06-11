from libcpp cimport bool
cimport cython
from py_inverted_multi_index_common cimport LONGLONG

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
        InvertedMultiIndex[IndexEntry]* buildInvertedMultiIndex(IndexEntry *x, int x_len, int* X_centroid_indices, int subspaces_count, int centroids_count_in_each_subspace)
        InvertedMultiIndex[IndexEntry]* buildInvertedMultiIndex(IndexEntry *x, Vectors[float] xVectors, SubspacedVectors[float] subspacedCentroids)

cdef extern from "multiindex/InvertedMultiIndexSearcher.h":
    cdef cppclass InvertedMultiIndexSearcher[IndexEntry, FLOAT]:
        InvertedMultiIndexSearcher(InvertedMultiIndex[IndexEntry]* invertedMultiIndex, SubspacedVectors[FLOAT] &subspacedCentroids)
        void findNearest(const FLOAT *query_vector, IndexEntry *out_nearest_entries, const int nearest_entries_count)

cdef extern from "multiindex/MultiIndexUtil.h":
    cdef cppclass MultiIndexUtil[LONGLONG]:
          MultiIndexUtil(const int n_dims, const int dim_size)
          long long flat_index(const int *multi_index)
          long long* flat_indices(const int *multi_index, int multi_indices_len)