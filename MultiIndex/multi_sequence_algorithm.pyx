# distutils: language=c++
import numpy as np
# cimport numpy as np

cdef extern from "MultiSequenceAlgorithmClass.cpp":
    cdef cppclass MultiSequenceAlgorithm[T]:
        MultiSequenceAlgorithm(const int n_subquantizers, const int n_clusters, const int* list_start_ndarray, const T* lists, const int lists_len)
        void find_and_write_candidates(const float *cluster_distance_matrix, const int* nearest_cluster_index_matrix, T* candidate_list, const int candidate_list_len)

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
