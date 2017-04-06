import numpy as np
import inverted_multi_index as imi

import unittest

class TestSearcher(unittest.TestCase):
    def test_init_fromX(self):
        x_ids = np.array([10, 20, 30, 40, 50, 60, 70, 80, 90, 100], dtype=np.int32)
        X = np.array([
            [0, 10, 99],
            [0, 10, 99],
            [0, 10, 99],
            [0, 10, 99],
            [0, 10, 99],
            [0, 10, 99],
            [0, 10, 99],
            [0, 10, 99],
            [1, 22, 220],
            [3, 40, 400],
        ], dtype=np.float32)
        cluster_centers = np.array([
            [
                [0],
                [1],
                [2],
                [3]
            ],
            [
                [10],
                [20],
                [30],
                [40]
            ],
            [
                [100],
                [200],
                [300],
                [400]
            ]
        ])

        searcher = imi.PyInvertedMultiIndexSearcher(cluster_centers)
        searcher.init_build_invertedMultiIndex(X, x_ids)

        Q = np.array(
            [
                [3, 40, 400],
                [3, 40, 400],
                [1, 9, 98]
            ]
        )
        ground_truth_candidates = np.array([
            [100, 90, 80, 70, 60, 50, 40, 30, 20, 10],
            [100, 90, 80, 70, 60, 50, 40, 30, 20, 10],
            [80, 70, 60, 50, 40, 30, 20, 10, 90, 100]
        ])
        candidates = searcher.find_candidates(Q, 10)
        # print(candidates)
        self.assertTrue((candidates == ground_truth_candidates).all())


if __name__ == '__main__':
    unittest.main()
