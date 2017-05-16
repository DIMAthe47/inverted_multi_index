import unittest
import numpy as np
import sys

sys.path.append("../")
import py_inverted_multi_index as pyimi


class TestInvertedMultiIndexSearcher(unittest.TestCase):
    py_imi_searcher = None

    @classmethod
    def setUpClass(cls):
        vector_ids = np.array([10, 20, 30, 40], dtype=np.int32)
        vectors_arr = np.array([
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 2, 3,
            1, 2, 3, 0, 0, 0,
            1, 2, 3, 1, 2, 3,
        ], dtype=np.float32
        ).reshape((4, 3 * 2))

        subspaced_centrois = np.array([
            1, 2, 3, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5,
            0, 0, 0, 5, 5, 5, 5, 5, 5, 1, 2, 3, 5, 5, 5,
        ],
            dtype=np.float32
        ).reshape((2, 5, 3))

        py_imi = pyimi.PyInvertedMultiIndexBuilder().buildInvertedMultiIndex(vector_ids, vectors_arr,
                                                                               subspaced_centrois)

        # right_entries_list_start_vector = np.array([0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4,
        #                                             4, 4, 4, 4, 4], dtype=np.int32)
        # cls.assertTrue(np.array_equal(right_entries_list_start_vector, py_imi.entries_list_starts))
        # right_entries = np.array([30, 40, 10, 20])
        # cls.assertTrue(np.array_equal(right_entries, py_imi.entries))

        TestInvertedMultiIndexSearcher.py_imi_searcher = pyimi.PyInvertedMultiIndexSearcher(py_imi,
                                                                                              subspaced_centrois)

    def check_candidates_are_right(self, query_vector, possible_truth_variants):
        query_vector_ndarray = np.array(query_vector, np.float32)
        nearest_entries = self.py_imi_searcher.findNearest(query_vector_ndarray, 4)
        # print("nearest_entries", nearest_entries)
        is_right = False
        for truth_variant in possible_truth_variants:
            truth_variant_ndarray = np.array(truth_variant, np.int32)
            if np.array_equal(nearest_entries, truth_variant_ndarray):
                is_right = True
        self.assertTrue(is_right)

    def test_search1(self):
        self.check_candidates_are_right([0, 0, 0, 0, 0, 0], [[10, 30, 20, 40], [10, 20, 30, 40]])

    def test_search2(self):
        self.check_candidates_are_right([0, 0, 0, 1, 2, 3], [[20, 10, 40, 30], [20, 40, 10, 30]])

    def test_search3(self):
        self.check_candidates_are_right([1, 2, 3, 0, 0, 0], [[30, 10, 40, 20], [30, 40, 10, 20]])

    def test_search4(self):
        self.check_candidates_are_right([1, 2, 3, 1, 2, 3], [[40, 20, 30, 10], [40, 30, 20, 10]])

    def test_search5(self):
        self.check_candidates_are_right([4, 4, 4, 5, 2, 3], [[40, 20, 30, 10], [40, 30, 20, 10]])


if __name__ == '__main__':
    unittest.main()
