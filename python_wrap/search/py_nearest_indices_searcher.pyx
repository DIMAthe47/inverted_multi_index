# distutils: language=c++
from libcpp cimport bool
cimport cython
cimport numpy as np
import numpy as np

ctypedef int CODE_
ctypedef float FLOAT_
DEF INT_dtype="int32"

cdef extern from "search/NearestIndicesSearcher.h":
    cdef cppclass NearestIndicesSearcher[FLOAT]:
        NearestIndicesSearcher(const size_t max_distances_count_)
        void findNearestIndices(const FLOAT *distances_matrix, const size_t width_distances_matrix,
                                    const size_t height_distances_matrix, int *out_nearest_indices_matrix,
                                    const size_t n_nearest) const

cdef class PyNearestIndicesSearcher:
    cdef NearestIndicesSearcher[FLOAT_]* c_nis
    cdef size_t n_base_vectors

    def __cinit__(self, const size_t max_distances_count_):
        self.c_nis = new NearestIndicesSearcher[FLOAT_](max_distances_count_)

    def findNearestIndices(self, const FLOAT_[:,::1] distances_matrix, const size_t n_nearest):
        cdef size_t n_queries = distances_matrix.shape[0]
        cdef size_t n_base_vectors =  distances_matrix.shape[1]
        cdef np.ndarray out_nearest_indices_matrix = np.empty((n_queries, n_nearest), dtype=INT_dtype)
        cdef int[:,::1] out_nearest_indices_matrix_view = out_nearest_indices_matrix

        self.c_nis.findNearestIndices(&distances_matrix[0][0], n_base_vectors, n_queries, &out_nearest_indices_matrix_view[0][0], n_nearest)

        return out_nearest_indices_matrix

    def __dealloc__(self):
        del self.c_nis
