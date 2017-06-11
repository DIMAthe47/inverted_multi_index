# distutils: language=c++
from libcpp cimport bool
cimport cython
cimport numpy as np
import numpy as np

ctypedef int CODE_
ctypedef float FLOAT_
DEF FLOAT_dtype="float32"

cdef extern from "distance/SymmetricDistanceComputer.h":
    cdef cppclass SymmetricDistanceComputer[CODE, FLOAT]:
        SymmetricDistanceComputer(const CODE *base_vectors_codes, const size_t n_base_vectors,
                                    const FLOAT *centroids_pairwise_squared_distances, const size_t n_subspaces,
                                    const size_t n_centroids_in_each_subspace)
        void computePairwiseSquaredDistances(const CODE *query_vectors_codes, const size_t n_queries,
                                                 FLOAT *out_distances_matrix)

cdef class PySymmetricDistanceComputer:
    cdef SymmetricDistanceComputer[CODE_, FLOAT_]* c_sdc
    cdef size_t n_base_vectors

    def __cinit__(self, const CODE_[:,::1] base_vectors_codes, const FLOAT_[:,:,::1] centroids_pairwise_squared_distances):
        self.n_base_vectors=base_vectors_codes.shape[0]
        cdef size_t n_subspaces=base_vectors_codes.shape[1]
        cdef size_t n_centroids_in_each_subspace=centroids_pairwise_squared_distances.shape[1]
        self.c_sdc = new SymmetricDistanceComputer[CODE_,FLOAT_](&base_vectors_codes[0,0], self.n_base_vectors, &centroids_pairwise_squared_distances[0,0,0], n_subspaces, n_centroids_in_each_subspace)

    def computePairwiseSquaredDistances(self, const CODE_[:,::1] query_vectors_codes):
        cdef size_t n_queries = query_vectors_codes.shape[0]
        cdef np.ndarray out_distances_matrix = np.empty((n_queries, self.n_base_vectors), dtype=FLOAT_dtype)
        cdef FLOAT_[:,::1] out_distances_matrix_view = out_distances_matrix
        self.c_sdc.computePairwiseSquaredDistances(&query_vectors_codes[0][0], n_queries, &out_distances_matrix_view[0][0])

        return out_distances_matrix

    def __dealloc__(self):
        del self.c_sdc
