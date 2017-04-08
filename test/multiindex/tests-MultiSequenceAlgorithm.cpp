//
// Created by dima on 08.04.17.
//

#include "catch.hpp"
#include "../../src/multiindex/InvertedMultiIndex.h"
#include "../../src/multiindex/MultiSequenceAlgorithm.h"

TEST_CASE("run MultiSequenceAlgorithm", "[multiindex]") {
    const int subspaces_count = 2;
    const int centroids_count_in_each_subspace = 3;

    int cells_ = 1;
    for (int i = 0; i < subspaces_count; i++) {
        cells_ *= centroids_count_in_each_subspace;
    }

    const int centroids_ = subspaces_count * centroids_count_in_each_subspace;
    const float *cluster_distance_matrix = new float[centroids_]{1, 2, 3,
                                                                 0, 70, 60};
    const int *nearest_cluster_index_matrix = new int[cells_]{0, 1, 2,
                                                              0, 2, 1};
    //(0,0), (1,0), (2,0), (0,2), (1,2), (2,2), (0,1), (1,1), (2,1)
    // 0-1,   2-4,   6-8,   2-2,   4-6,  10-10,  1-2    4-4    8-10
    // 0     20,30   60,70   -     40,50   -      10,    -     80,90
    int list_entries_starts[] = {0, 1, 2,
                                 2, 4, 4,
                                 6, 8, 10, 10};
    int lists[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90};

    InvertedMultiIndex<int> *invertedMultiIndex = new InvertedMultiIndex<int>(lists, list_entries_starts,
                                                                              subspaces_count,
                                                                              centroids_count_in_each_subspace);
    MultiSequenceAlgorithm<int> msa(invertedMultiIndex);

    const int n_candidates = 10;
    int *candidates = new int[n_candidates];
    msa.find_and_write_candidates(cluster_distance_matrix, nearest_cluster_index_matrix, candidates, n_candidates);

    std::vector<int> truth_nearest = {0, 20, 30, 60, 70, 40, 50, 10, 80, 90};
    std::vector<int> nearest_ = array_to_vector(candidates, n_candidates);
    REQUIRE(truth_nearest == nearest_);

//    print_array(candidates, n_candidates, "%d ");
    delete[] candidates;
}
