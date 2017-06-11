# distutils: language=c++
from libcpp cimport bool
cimport cython
cimport numpy as np
import numpy as np

ctypedef int CODE_
ctypedef float FLOAT_
DEF FLOAT_dtype="float32"

cdef extern from "distance/AsymmetricDistanceComputer.h":
    cdef cppclass AsymmetricDistanceComputer[CODE, FLOAT]:
        AsymmetricDistanceComputer(const FLOAT *subspaced_centroids, const size_t n_subspaces,
                                      const size_t n_centroids_in_each_subspace, const size_t subspace_dim,
                                      const CODE *base_vectors_codes, const size_t n_base_vectors)
        void computePairwiseSquaredDistances(const FLOAT *query_vectors, const size_t n_queries,
                                                 FLOAT *out_distances_matrix)

cdef class PyAsymmetricDistanceComputer:
    cdef AsymmetricDistanceComputer[CODE_, FLOAT_]* c_adc
    cdef size_t n_base_vectors

    def __cinit__(self, const CODE_[:,::1] base_vectors_codes, const FLOAT_[:,:,::1] subspaced_centroids):
        self.n_base_vectors = base_vectors_codes.shape[0]
        cdef size_t n_subspaces = base_vectors_codes.shape[1]
        cdef size_t n_centroids_in_each_subspace = subspaced_centroids.shape[1]
        cdef size_t subspace_dim = subspaced_centroids.shape[2]
        self.c_adc = new AsymmetricDistanceComputer[CODE_,FLOAT_](&subspaced_centroids[0][0][0], n_subspaces, n_centroids_in_each_subspace, subspace_dim,
            &base_vectors_codes[0,0], self.n_base_vectors)

    def computePairwiseSquaredDistances(self, const FLOAT_[:,::1] query_vectors):
        cdef size_t n_queries = query_vectors.shape[0]
        cdef np.ndarray out_distances_matrix = np.empty((n_queries, self.n_base_vectors), dtype=FLOAT_dtype)
        cdef FLOAT_[:,::1] out_distances_matrix_view = out_distances_matrix
        self.c_adc.computePairwiseSquaredDistances(&query_vectors[0][0], n_queries, &out_distances_matrix_view[0][0])

        return out_distances_matrix

    def __dealloc__(self):
        del self.c_adc
