import numpy as np
import sklearn.metrics as metrics


class NearestSearch:
    def find_nearest_indices(self, X: np.ndarray, Q: np.ndarray, n_nearest=None, metric='l2', n_jobs=1,
                             return_distances=False, partition_only=False):
        Q = np.atleast_2d(Q)
        vectors_distances_matrix = self.find_vectors_distances(X, Q, metric, n_jobs)
        if (not n_nearest):
            n_nearest = vectors_distances_matrix.shape[1]

        x_len = len(Q)
        rows_range = np.arange(x_len).reshape((-1, 1))
        # if n_nearest==1:
        #     nearest_indices = np.argmax(vectors_distances_matrix, axis=1)
        # else:
        nearest_indices = np.argpartition(vectors_distances_matrix, axis=1, kth=n_nearest - 1)[:, :n_nearest]
        if not partition_only:
            nearest_indices = nearest_indices[
                rows_range, np.argsort(vectors_distances_matrix[rows_range, nearest_indices])]

        if return_distances:
            return (nearest_indices, vectors_distances_matrix)
        else:
            return nearest_indices

    def find_vectors_distances(self, X: np.ndarray, Q: np.ndarray, metric='l2', n_jobs=1):
        # print("X", X.shape, "Q", Q.shape)
        X = X.squeeze()
        Q = Q.squeeze()
        vectors_distances_matrix = metrics.pairwise_distances(Q, X, metric=metric, n_jobs=n_jobs)
        return vectors_distances_matrix
