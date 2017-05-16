//
// Created by dima on 08.04.17.
//

#include "catch.hpp"
#include "../../src/search/SubspacesProductNearestIndicesSearcher.h"
#include "../../src/util/array_utils.h"


TEST_CASE("find nearest subspace indices by distances[2, 3]", "[nearest][subspace][SubspacesProductNearestIndicesSearcher][SubspacedScalars]") {

    const int max_n_nearest = 3;
    const int subspace_count = 2;
    const SubspacesProductNearestIndicesSearcher<float> subspacesProductNearestIndicesSearcher(max_n_nearest, subspace_count);

    const int distances_count_in_each_subspace = 3;
    float subspace_distances[] = {1.2, 0, 5,
                                  0, 1, 2};
    const SubspacedScalars<float> subspacedDistances(subspace_distances, subspace_count, distances_count_in_each_subspace);

    const int n_nearest_in_each_subspace = max_n_nearest;
    int subspace_nearest_indices[subspace_count * n_nearest_in_each_subspace] = {-1, -1, -1,
                                                                                    -1, -1, -1};
    SubspacedScalars<int> subspacedNearestIndices(subspace_nearest_indices, subspace_count,
                                                   n_nearest_in_each_subspace);
    subspacesProductNearestIndicesSearcher.findNearestIndices(subspacedDistances, subspacedNearestIndices);

    std::vector<int> answer_vector = array_to_vector(subspace_nearest_indices, subspace_count * n_nearest_in_each_subspace);
    std::vector<int> right_answer_vector = {1, 0, 2,
                                            0, 1, 2};
    REQUIRE(answer_vector == right_answer_vector);
}
