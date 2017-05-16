import unittest
import numpy as np
import sys

sys.path.append("../")
import py_inverted_multi_index as pyimi


class TestInvertedMultiIndexBuilder(unittest.TestCase):
    def test_build(self):
        vector_ids = np.array([10, 20, 30, 40])
        vectors_arr = np.array([
            [0, 0, 0, 0, 0, 0],
            [0, 0, 0, 1, 2, 3],
            [1, 2, 3, 0, 0, 0],
            [1, 2, 3, 1, 2, 3]
        ], dtype=np.float32
        )

        subspaces_count = 3
        centroids_count_in_each_subspace = 2
        subspaced_centrois = np.array([
            1, 2, 3, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5,
            0, 0, 0, 5, 5, 5, 5, 5, 5, 1, 2, 3, 5, 5, 5,
        ],
            dtype=np.float32
        ).reshape((2, 5, 3))

        py_imi = pyimi.PyInvertedMultiIndexBuilder().buildInvertedMultiIndex(vector_ids, vectors_arr,
                                                                               subspaced_centrois)

        right_entries_list_start_vector = np.array([0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4,
                                                    4, 4, 4, 4, 4], dtype=np.int32)
        self.assertTrue(np.array_equal(right_entries_list_start_vector, py_imi.entries_list_starts))

        right_entries = np.array([30, 40, 10, 20])
        self.assertTrue(np.array_equal(right_entries, py_imi.entries))


if __name__ == '__main__':
    unittest.main()
