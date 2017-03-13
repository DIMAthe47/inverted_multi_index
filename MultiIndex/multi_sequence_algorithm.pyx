# distutils: language=c++
import numpy as np
# cimport numpy as np

cdef extern from "InvertedMultiIndex.h":
    cdef cppclass InvertedMultiIndex[T]:
        int entries_len
    cdef InvertedMultiIndex[T]* build_InvertedMultiIndex[T](T* x, int x_len, int* X_centroid_indices, int n_subquantizers, int n_clusters)

cdef extern from "MultiSequenceAlgorithm.h":
    cdef cppclass MultiSequenceAlgorithm[T]:
        MultiSequenceAlgorithm(InvertedMultiIndex[T]* invertedMultiIndex)
        void find_and_write_candidates(const float *cluster_distance_matrix, const int* nearest_cluster_index_matrix, T* out_candidate_list, const int out_candidate_list_len)

cdef class PyInvertedMultiIndex:
    cdef InvertedMultiIndex[int]* invertedMultiIndex

    def __cinit__(self, int[::1] x_ids, int[:,::1] X_centroid_indices, int n_clusters):
        cdef int x_len = len(x_ids)
        cdef int n_subquantizers = X_centroid_indices.shape[0]
        self.invertedMultiIndex = build_InvertedMultiIndex[int](&x_ids[0], x_len, &X_centroid_indices[0][0], n_subquantizers, n_clusters)

    def __dealloc__(self):
        del self.invertedMultiIndex

cdef class PyMultiSequenceAlgorithm:
    cdef MultiSequenceAlgorithm[int]* multiSequenceAlgorithm
    cdef int max_candidates_to_take

    def __cinit__(self, PyInvertedMultiIndex pyInvertedMultiIndex):
        self.multiSequenceAlgorithm = new MultiSequenceAlgorithm[int](pyInvertedMultiIndex.invertedMultiIndex)
        self.max_candidates_to_take = pyInvertedMultiIndex.invertedMultiIndex.entries_len

    def f1(self, int n):
        print(n)

    def f2(self, n):
        print(n)

    def find_candidates(self, cluster_distance_matrix, nearest_cluster_index_matrix, int n_candidates):
        cdef float[::1] _cluster_distance_matrix = cluster_distance_matrix.ravel().astype(np.float32)
        cdef int[::1] _nearest_cluster_index_matrix = nearest_cluster_index_matrix.ravel().astype(np.int32)
        cdef int[::1] candidates = self.__find_candidates(_cluster_distance_matrix, _nearest_cluster_index_matrix, n_candidates)
        print(candidates)
        return np.asarray(candidates)

    def __find_candidates(self, float[::1] cluster_distance_matrix, int[::1] nearest_cluster_index_matrix, int n_candidates):
        cdef int candidates_to_take = n_candidates if n_candidates < self.max_candidates_to_take else self.max_candidates_to_take
        cdef int[::1] candidates = np.empty((candidates_to_take,), dtype=np.int32)
        self.multiSequenceAlgorithm.find_and_write_candidates(&cluster_distance_matrix[0], &nearest_cluster_index_matrix[0], &candidates[0], candidates_to_take)
        print(candidates)
        return candidates

    def __dealloc__(self):
        del self.multiSequenceAlgorithm


"""
cdef class PyMultiSequenceAlgorithm:
    cdef max_candidates_to_take
    cdef MultiSequenceAlgorithm[int]* msa      # hold a C++ instance which we're wrapping
    def __cinit__(self, const int n_subquantizers, const int n_clusters, int[::1] list_start_ndarray, int[::1] lists):
        self.msa = new MultiSequenceAlgorithm[int](n_subquantizers, n_clusters, &list_start_ndarray[0], &lists[0], len(lists))
        self.max_candidates_to_take = len(lists)
    def find_and_write_candidates(self, cluster_distance_matrix, nearest_cluster_index_matrix, n_candidates):
        n_candidates = n_candidates if n_candidates < self.max_candidates_to_take else self.max_candidates_to_take
        # print("self.max_candidates_to_take", self.max_candidates_to_take)
        # print("n_candidates",n_candidates)

        cdef float[::1] cluster_distance_ravel=cluster_distance_matrix.ravel()
        cdef int[::1] nearest_cluster_index_ravel=nearest_cluster_index_matrix.ravel()
        candidates=np.empty((n_candidates,), dtype=np.int32)
        cdef int[::1] candidates_view=candidates
        self.msa.find_and_write_candidates(&cluster_distance_ravel[0], &nearest_cluster_index_ravel[0], &candidates_view[0], n_candidates)
        # cdef np.ndarray candidates_ndarray=np.asarray(candidates)
        return candidates
"""