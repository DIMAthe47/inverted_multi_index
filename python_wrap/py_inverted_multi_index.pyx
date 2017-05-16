# distutils: language=c++
from libcpp cimport bool
from cython.operator cimport dereference as deref
cimport c_inverted_multi_index as cimi
cimport py_inverted_multi_index as pyimi
import numpy as np
cimport numpy as cnp
cimport cython
from py_inverted_multi_index_common cimport IndexEntry, FLOAT

cdef class PyInvertedMultiIndex:
    def __cinit__(self, cnp.ndarray entries, cnp.ndarray entries_list_starts, int subspaces_count, int centroids_count_in_each_subspace):
        self.entries=entries
        self.entries_list_starts=entries_list_starts
        self.subspaces_count=subspaces_count
        self.centroids_count_in_each_subspace=centroids_count_in_each_subspace

cdef class PyInvertedMultiIndexBuilder:
    def buildInvertedMultiIndex(self, IndexEntry[::1] x, FLOAT[:,::1] xVectors, FLOAT[:,:,::1] subspacedCentroids):
        cdef cimi.Vectors[FLOAT]* x_vectors_ = py_to_c_vectors(xVectors)
        cdef cimi.SubspacedVectors[FLOAT]* subspaced_centroids_ = py_to_c_subspaced_vectors(subspacedCentroids)

        cdef cimi.InvertedMultiIndexBuilder[IndexEntry] imibuilder
        cdef cimi.InvertedMultiIndex[IndexEntry]* c_inverted_multi_index = imibuilder.buildInvertedMultiIndex(&x[0], deref(x_vectors_), deref(subspaced_centroids_))
        del x_vectors_
        del subspaced_centroids_

        cdef PyInvertedMultiIndex py_imi = c_to_py_imi(c_inverted_multi_index)
        c_inverted_multi_index.manage_memory=False

        return py_imi

cdef class PyInvertedMultiIndexSearcher:
    def __cinit__(self, PyInvertedMultiIndex py_imi, FLOAT[:,:,::1] subspaced_centroids):
        self.py_imi = py_imi
        cdef cimi.InvertedMultiIndex[IndexEntry]* c_imi = py_to_c_imi(py_imi)
        cdef cimi.SubspacedVectors[FLOAT]* c_subspaced_centroids = py_to_c_subspaced_vectors(subspaced_centroids)
        self.c_imi_searcher = new cimi.InvertedMultiIndexSearcher[int, float](c_imi, deref(c_subspaced_centroids))
        del c_subspaced_centroids

    def findNearest(self, FLOAT[::1] query_vector, int nearest_entries_count):
        cdef cnp.ndarray out_nearest_entries = np.empty(nearest_entries_count, np.int32)
        cdef IndexEntry[::1] out_nearest_entries_view = out_nearest_entries
        self.c_imi_searcher.findNearest(&query_vector[0], &out_nearest_entries_view[0], nearest_entries_count)
        return out_nearest_entries

cdef class _finalizer:
    def __dealloc__(self):
        cimi.c_delete(self._data)

cdef void set_base(cnp.ndarray arr, const void* c_arr):
    cdef _finalizer f=_finalizer()
    f._data=<const void*>c_arr
    cnp.set_array_base(arr, f)

cdef pyimi.PyInvertedMultiIndex c_to_py_imi(cimi.InvertedMultiIndex[IndexEntry]* c_imi):
    cdef IndexEntry[::1] entries_view=<IndexEntry[:c_imi.entries_count]>c_imi.entries
    cdef cnp.ndarray entries = np.asarray(entries_view)
    set_base(entries, c_imi.entries)

    cdef int[::1] entries_list_starts_view=<int[:c_imi.entries_list_starts_len]>c_imi.entries_list_starts
    cdef cnp.ndarray entries_list_starts=np.asarray(entries_list_starts_view)
    set_base(entries_list_starts, c_imi.entries_list_starts)

    cdef int subspaces_count = c_imi.subspaces_count
    cdef int centroids_count_in_each_subspace = c_imi.centroids_count_in_each_subspace
    cdef pyimi.PyInvertedMultiIndex py_imi = pyimi.PyInvertedMultiIndex(entries, entries_list_starts, subspaces_count, centroids_count_in_each_subspace)
    return py_imi

cdef cimi.InvertedMultiIndex[IndexEntry]* py_to_c_imi(pyimi.PyInvertedMultiIndex py_imi):
    cdef IndexEntry[::1] entries_view = py_imi.entries
    cdef int[::1] entries_list_starts_view = py_imi.entries_list_starts
    cdef int entries_list_starts_len = py_imi.entries_list_starts.shape[0]
    cdef cimi.InvertedMultiIndex[IndexEntry]* c_imi = new cimi.InvertedMultiIndex[IndexEntry](&entries_view[0], &entries_list_starts_view[0], entries_list_starts_len,
                                                                    py_imi.subspaces_count, py_imi.centroids_count_in_each_subspace)
    # cdef cimi.InvertedMultiIndex[IndexEntry]* c_imi = new cimi.InvertedMultiIndex[IndexEntry](<IndexEntry*>py_imi.entries.data, <int*>py_imi.entries_list_starts.data, py_imi.entries_list_starts.shape[0],
    #                                                                 py_imi.subspaces_count, py_imi.centroids_count_in_each_subspace)
    c_imi.manage_memory=False
    return c_imi


cdef cimi.Vectors[FLOAT]* py_to_c_vectors(FLOAT[:,::1] vectors):
    cdef int vectors_count=vectors.shape[0]
    cdef int vectors_dim=vectors.shape[1]
    cdef cimi.Vectors[FLOAT]* vectors_=new cimi.Vectors[FLOAT](&vectors[0][0], vectors_count, vectors_dim)
    return vectors_

cdef cimi.SubspacedVectors[FLOAT]* py_to_c_subspaced_vectors(FLOAT[:,:,::1] vectors):
    cdef int subspaces_count=vectors.shape[0]
    cdef int vectors_count=vectors.shape[1]
    cdef int subspace_dim=vectors.shape[2]
    cdef cimi.SubspacedVectors[FLOAT]* subspaced_vectors=new cimi.SubspacedVectors[FLOAT](&vectors[0][0][0], subspaces_count, vectors_count, subspace_dim)
    return subspaced_vectors