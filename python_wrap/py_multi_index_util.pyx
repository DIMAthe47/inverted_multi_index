# distutils: language=c++
from libcpp cimport bool
cimport cython
cimport numpy as cnp
import numpy as np
ctypedef long long LONGLONG

cdef extern from "multiindex/MultiIndexUtil.h":
    cdef cppclass MultiIndexUtil[LONGLONG]:
          MultiIndexUtil(const int n_dims, const int dim_size)
          long long flat_index(const int *multi_index)
          long long* flat_indices(const int *multi_index, int multi_indices_len, LONGLONG* flatindices_out)


cdef class PyMultiIndexUtil:
    cdef MultiIndexUtil[LONGLONG]* c_multi_index_util

    def __cinit__(self, n_dims, dim_size):
        self.c_multi_index_util = new MultiIndexUtil[LONGLONG](n_dims, dim_size)

    def flat_index(self, cnp.ndarray multi_index):
        multi_index = multi_index.astype(self.np.int32, copy=False)
        cdef int[::1] multi_index_view = multi_index
        cdef LONGLONG flatindex = self.c_multi_index_util.flat_index(&multi_index_view[0])
        return flatindex

    def flat_indices(self, cnp.ndarray multi_indices):
        multi_indices = multi_indices.astype(np.int32, copy=False)
        cdef int[:, ::1] multi_indices_view = multi_indices

        cdef int elems_len = multi_indices.shape[0]
        cdef cnp.ndarray flatindices = np.empty((elems_len,), dtype=np.int64)
        cdef LONGLONG[::1] flatindices_view = flatindices

        self.c_multi_index_util.flat_indices(&multi_indices_view[0][0], elems_len, &flatindices_view[0])

        return flatindices

    def __dealloc__(self):
        del self.c_multi_index_util
