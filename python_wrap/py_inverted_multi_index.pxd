cimport c_inverted_multi_index as cimi
cimport numpy as cnp
cimport cython
from py_inverted_multi_index_common cimport IndexEntry, FLOAT, LONGLONG


cdef class PyInvertedMultiIndex:
    cdef readonly cnp.ndarray entries
    cdef readonly cnp.ndarray entries_list_starts
    cdef readonly int subspaces_count
    cdef readonly int centroids_count_in_each_subspace

cdef class PyInvertedMultiIndexBuilder:
    pass

cdef class PyInvertedMultiIndexSearcher:
    cdef cimi.InvertedMultiIndexSearcher[IndexEntry, FLOAT]* c_imi_searcher
    cdef PyInvertedMultiIndex py_imi #to hold ref => py_imi will not be destroyed

cdef class PyMultiIndexUtil:
    cdef cimi.MultiIndexUtil[LONGLONG]* c_multi_index_util

cdef class _finalizer:
    cdef const void *_data

cdef void set_base(cnp.ndarray arr, const void* c_arr)

cdef PyInvertedMultiIndex c_to_py_imi(cimi.InvertedMultiIndex[IndexEntry]* c_imi)

cdef cimi.InvertedMultiIndex[IndexEntry]* py_to_c_imi(PyInvertedMultiIndex py_imi)

cdef cimi.Vectors[FLOAT]* py_to_c_vectors(FLOAT[:,::1] vectors)

cdef cimi.SubspacedVectors[FLOAT]* py_to_c_subspaced_vectors(FLOAT[:,:,::1] vectors)
