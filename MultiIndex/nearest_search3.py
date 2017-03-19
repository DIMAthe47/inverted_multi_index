import numpy as np
import sklearn.metrics as metrics
import collections
# import itertools_util as itut
from sklearn.cross_validation import KFold


class NearestSearch3:
    def find_nearest_indices(self, X:np.ndarray, Q: np.ndarray, n_nearest=None, metric='l2',n_jobs=1, include_distances=False, partition_only=False):
        Q = np.atleast_2d(Q)
        vectors_distances_matrix = self.find_vectors_distances(X, Q, metric, n_jobs)
        if (not n_nearest):
            n_nearest = vectors_distances_matrix.shape[1]

        x_len = len(Q)
        rows_range = np.arange(x_len).reshape((-1, 1))
        # if n_nearest==1:
        #     nearest_indices = np.argmax(vectors_distances_matrix, axis=1)
        # else:
        nearest_indices = np.argpartition(vectors_distances_matrix, axis=1, kth=n_nearest-1)[:,:n_nearest]
        if not partition_only:
           nearest_indices = nearest_indices[rows_range,np.argsort(vectors_distances_matrix[rows_range, nearest_indices])]

        if include_distances:
            return (nearest_indices, vectors_distances_matrix)
        else:
            return nearest_indices


    def find_vectors_distances(self, X: np.ndarray, Q: np.ndarray, metric='l2', n_jobs=1):
        vectors_distances_matrix = metrics.pairwise_distances(Q, X, metric=metric, n_jobs=n_jobs)
        return vectors_distances_matrix


import unittest


def extract_first_integer(arr):
    val=np.frombuffer(arr[0].tobytes(), dtype='<i4')[0]
    return val
def extract_data(file_path, is_float):
    dtype='<f4' if is_float else '<i4'
    data_values=np.fromfile(file_path, dtype)
    dimension=np.frombuffer(data_values[0].tobytes(), dtype='<i4')[0]
    vectorsize = 4 + dimension*4
    vectorsize_el=vectorsize//4
    data_values=data_values.reshape((-1,vectorsize_el))
    data_values=data_values[:,1:]
    return data_values

class NearestSearchTest(unittest.TestCase):
    def test_findNearestIndices_all(self):
        base_vectors = np.array([
            [0, 2, 4],
            [0, 3, 4],
            [1, 3, 5],
            [1, 3, 6]
        ])
        ns = NearestSearch()

        query_vectors = np.array([
            [0, 2, 4],
            [1, 3, 5]
        ])
        nearest_indices = ns.find_nearest_indices(base_vectors, query_vectors, metric='l1')
        print(nearest_indices)
        truth_indices = np.array([
            [0, 1, 2, 3],
            [2, 3, 1, 0],
        ])
        self.assertTrue((nearest_indices == truth_indices).all())
    def test_2(self):
        cluster_centers = np.load('cluster_centers-4.npy')
        print(cluster_centers.shape)
        data_root_path = r'C:\Users\Dima\GoogleDisk\datasets'
        data_path_10K = r'siftsmall\siftsmall\siftsmall_{0}'
        import os
        data_path = data_path_10K
        data_path = os.path.join(data_root_path, data_path)
        base_data_path = data_path.format('base.fvecs')

        self.cluster_centers = cluster_centers
        self.n_subquantizers = cluster_centers.shape[0]
        self.n_clusters = cluster_centers.shape[1]
        self.n_dims = cluster_centers.shape[2]

        base_data = extract_data(base_data_path, True)
        X=base_data

        print("init_build_invertedMultiIndex")
        m = self.n_subquantizers
        X_centroid_indices = np.empty((len(X), m, 1), dtype=np.int32)
        print("X_centroid_indices shape:", X_centroid_indices.shape)

        nearestSearch = NearestSearch3()
        X = X.reshape((len(X), m, self.n_dims))
        print("before loop")
        for i in range(m):
            print(i)
            print("cluster_centers", self.cluster_centers[i, :, :].shape)
            print("X", X[:, i, :].shape)
            print("before find")
            finded = nearestSearch.find_nearest_indices(self.cluster_centers[i, :, :], X[:, i, :], n_nearest=1)
            print(finded.shape, finded.dtype)
            print("finded", finded.shape)
            finded = finded.astype(np.int32)
            print("before assign")
            X_centroid_indices[:, i, :] = finded
        print("after loop")
        print(X_centroid_indices.shape)

if __name__ == '__main__':
    unittest.main()
