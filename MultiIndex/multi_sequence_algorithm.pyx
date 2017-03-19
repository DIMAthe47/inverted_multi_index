# distutils: language=c++
import numpy as np
import nearest_search3 as ns
# cimport numpy as np

import sys

cdef extern from "InvertedMultiIndex.h":
    cdef cppclass InvertedMultiIndex[T]:
        int entries_len
    # cdef InvertedMultiIndex[T]* build_InvertedMultiIndex[T](T* x, int x_len, int* X_centroid_indices, int n_subquantizers, int n_clusters)
    cdef InvertedMultiIndex[int]* build_InvertedMultiIndex[int](int* x, int x_len, int* X_centroid_indices, int n_subquantizers, int n_clusters)

cdef extern from "MultiSequenceAlgorithm.h":
    cdef cppclass MultiSequenceAlgorithm[T]:
        MultiSequenceAlgorithm(InvertedMultiIndex[T]* invertedMultiIndex)
        void find_and_write_candidates(const float *cluster_distance_matrix, const int* nearest_cluster_index_matrix, T* out_candidate_list, const int out_candidate_list_len)


cdef class PyInvertedMultiIndexSearcher:
    cdef PyMultiSequenceAlgorithm pyMultiSequenceAlgorithm
    cdef cluster_centers
    cdef int n_subquantizers    # m
    cdef int n_clusters         # K
    cdef int n_dims             # D/m
    cdef PyInvertedMultiIndex pyInvertedMultiIndex

    def __cinit__(self, cluster_centers):
        """"
            cluster_centers[m, K, D/m]
        """
        self.cluster_centers = cluster_centers
        self.n_subquantizers = cluster_centers.shape[0]
        self.n_clusters = cluster_centers.shape[1]
        self.n_dims = cluster_centers.shape[2]

    def init_from_invertedMultiIndex(self, PyInvertedMultiIndex pyInvertedMultiIndex):
        self.pyMultiSequenceAlgorithm = PyMultiSequenceAlgorithm(pyInvertedMultiIndex)
        #в таком варианте инициализации мы не следим за pyInvertedMultiIndex и не разрушаем его

    # def init_build_invertedMultiIndex(self, X, int[::1] x_ids):
    def init_build_invertedMultiIndex(self, x, x_ids):
        #в таком варианте инициализации мы создаём pyInvertedMultiIndex и храним его в поле объекта. При разрушении объекта, разрушится и pyInvertedMultiIndex
        cdef int x_len = len(x)
        cdef int m = self.n_subquantizers
        cdef X_centroid_index_matrix = np.empty((x_len, m, 1), dtype=np.int32)

        nearestSearch = ns.NearestSearch3()
        # print(x.flags)
        # sys.stdout.flush()
        cdef float[:, :, :] X = x.reshape((x_len, m, self.n_dims), order='C')
        # print(X.flags)
        # sys.stdout.flush()

        for i in range(m):
            X_centroid_index_matrix[:, i, :] = nearestSearch.find_nearest_indices(self.cluster_centers[i,:,:], X[:,i,:], n_nearest=1)

        X_centroid_index_matrix = X_centroid_index_matrix.reshape((x_len, m))
        self.pyInvertedMultiIndex = PyInvertedMultiIndex(x_ids, X_centroid_index_matrix, self.n_clusters)
        self.pyMultiSequenceAlgorithm = PyMultiSequenceAlgorithm(self.pyInvertedMultiIndex)

    def __find_indices_and_distances(self, Q):
        """
            Q[len(Q), D]
        """
        cdef int m = self.n_subquantizers
        cdef int K = self.n_clusters
        cdef cluster_distance_matrix = np.empty((len(Q), m, K), dtype=np.float32)
        cdef cluster_nearest_index_matrix = np.empty((len(Q), m, K), dtype=np.int32)

        nearestSearch = ns.NearestSearch3()
        Q = Q.reshape((len(Q), m, self.n_dims))
        for i in range(m):
            cluster_nearest_index_matrix[:, i, :], cluster_distance_matrix[:, i, :] = nearestSearch.find_nearest_indices(self.cluster_centers[i,:,:], Q[:,i,:], include_distances=True)
        return cluster_nearest_index_matrix, cluster_distance_matrix


    def find_candidates(self, Q, int n_candidates):
        """
            Q[len(Q), D]
        """
        indices_and_distances = self.__find_indices_and_distances(Q)
        cdef cluster_nearest_index_matrix = indices_and_distances[0]
        cdef cluster_distance_matrix = indices_and_distances[1]

        cdef candidates_matrix=np.empty((len(Q), n_candidates), dtype=np.int32)
        cdef int Q_len = len(Q)
        for i in range(Q_len):
            # print("call for i{0}".format(i))
            # sys.stdout.flush()
            candidates_matrix[i,:] = self.pyMultiSequenceAlgorithm.find_candidates(cluster_distance_matrix[i], cluster_nearest_index_matrix[i], n_candidates)

        return candidates_matrix

cdef class PyInvertedMultiIndex:
    """
        wrap for InvertedMultiIndex class
    """

    cdef InvertedMultiIndex[int]* invertedMultiIndex

    def __cinit__(self, int[::1] x_ids, int[:,::1] X_centroid_indices, int n_clusters):
        """
            x_ids[len(x_ids)]
            X_centroid_indices[len(x_ids), m]
        """
        cdef int x_len = len(x_ids)
        cdef int n_subquantizers = X_centroid_indices.shape[1]
        self.invertedMultiIndex = build_InvertedMultiIndex[int](&x_ids[0], x_len, &X_centroid_indices[0,0], n_subquantizers, n_clusters)

    def __dealloc__(self):
        del self.invertedMultiIndex

cdef class PyMultiSequenceAlgorithm:
    """
        wrap for MultiSequenceAlgorithm class
    """

    cdef MultiSequenceAlgorithm[int]* multiSequenceAlgorithm
    cdef int max_candidates_to_take

    def __cinit__(self, PyInvertedMultiIndex pyInvertedMultiIndex):
        self.multiSequenceAlgorithm = new MultiSequenceAlgorithm[int](pyInvertedMultiIndex.invertedMultiIndex)
        self.max_candidates_to_take = pyInvertedMultiIndex.invertedMultiIndex.entries_len

    # def find_candidates(self, float[:,::1] cluster_distance_matrix, int[:,::1] nearest_cluster_index_matrix, int n_candidates):
    #     """
    #         cluster_distance_matrix[m, K]
    #         nearest_cluster_index_matrix[m, K]
    #     """
    #     cdef int[::1] candidates = self.__find_candidates(&cluster_distance_matrix[0,0], &nearest_cluster_index_matrix[0,0], n_candidates)
    #     return np.asarray(candidates)

    def find_candidates(self, float[:, ::1] cluster_distance_matrix, int[:,::1] nearest_cluster_index_matrix, int n_candidates):
        cdef int candidates_to_take = n_candidates if n_candidates < self.max_candidates_to_take else self.max_candidates_to_take
        cdef int[::1] candidates = np.empty((candidates_to_take,), dtype=np.int32)
        self.multiSequenceAlgorithm.find_and_write_candidates(&cluster_distance_matrix[0,0], &nearest_cluster_index_matrix[0,0], &candidates[0], candidates_to_take)
        return candidates
    """
    def find_candidates(self, float[::1] cluster_distance_matrix, int[::1] nearest_cluster_index_matrix, int n_candidates):
        cdef int candidates_to_take = n_candidates if n_candidates < self.max_candidates_to_take else self.max_candidates_to_take
        cdef int[::1] candidates = np.empty((candidates_to_take,), dtype=np.int32)
        self.multiSequenceAlgorithm.find_and_write_candidates(&cluster_distance_matrix[0], &nearest_cluster_index_matrix[0], &candidates[0], candidates_to_take)
        # print(candidates)
        return candidates
    """

    def __dealloc__(self):
        del self.multiSequenceAlgorithm
