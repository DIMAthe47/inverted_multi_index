import numpy as np
import os;
from itertools import *;
import io
import itertools_util
import collections
import time
from sklearn.cluster import KMeans

from sklearn.neighbors import KNeighborsClassifier
KNeighborsClassifier

class PQQuantizer():
    def __init__(self, nclusters, n_quantizers, n_jobs=-1, precompute_distances='auto', n_init=10, max_iter=200):
        self.nclusters = nclusters
        self.n_quantizers = n_quantizers
        self.subquantizers = []
        self.n_jobs=n_jobs
        self.precompute_distances=precompute_distances
        self.n_init=n_init
        self.max_iter=max_iter

    def fit(self, X: np.ndarray):
        self.subvector_length = len(X[0]) // self.n_quantizers
        print(self.subvector_length)
        X = X.reshape((len(X), self.n_quantizers, self.subvector_length))
        for i in range(self.n_quantizers):
            subvectors = X[:, i, :]
            #subvectors = np.copy(X[:, i, :], order='C')
            # print("subvectorsshape",subvectors.shape)
            kmeans = KMeans(n_clusters=self.nclusters, precompute_distances=self.precompute_distances, n_jobs=self.n_jobs, max_iter=self.max_iter, n_init=self.n_init,
                            verbose=True).fit(subvectors)
            self.subquantizers.append(kmeans)

    def predict(self, X):
        """
            X - matrix, rows: vectors
            get cluster indices for vectors in X
            X: [
                [x00,x01],
                [x10,x11],
                ...
            ]
            returns:
            [
                [u0, v0],
                [u1, v1],
                ...
            ]
        """
        centroids = np.empty(shape=(len(X), self.n_quantizers), dtype=np.int32)
        self.subvector_length = len(X[0]) / self.n_quantizers
        X = X.reshape((len(X), self.nquantizers, self.subvector_length))
        for i in range(self.n_quantizers):
            subvectors = X[:, i, :]
            subquantizer = self.subquantizers[i]
            # print("subvectorsshape", subvectors.shape)
            centroid_indexes = subquantizer.predict(subvectors)
            centroids[:, i] = centroid_indexes
        # centroids = centroids
        return centroids

    def cluster_centers_(self):
        cluster_centers = np.array((
            [self.subquantizers[i].cluster_centers_ for i in range(self.n_quantizers)]
        ))
        return cluster_centers



import unittest


class NearestSearchTest(unittest.TestCase):
    def test1(self):
        base_vector_ids = np.array([1, 100, 200, 53])
        base_vectors = np.array([
            [0, 2, 4, 2],
            [0, 3, 4, 2],
            [1, 3, 5, 1],
            [1, 3, 6, 0]
        ])
        pq=PQQuantizer(2, 2)
        pq.fit(base_vectors)
        print(pq.subquantizers[0].cluster_centers_)
        print(pq.subquantizers[1].cluster_centers_)
        x=np.array([
            [0,2,4,0],
            [0,2,4,1],
            [0, 2, 4, 0.1],
            [1,3,5.5,0],
            [0.6, 3, 5.5, 0]
            ]
        )
        print(pq.predict(x))
        print(pq.predict(base_vectors))

if __name__ == '__main__':
    unittest.main()