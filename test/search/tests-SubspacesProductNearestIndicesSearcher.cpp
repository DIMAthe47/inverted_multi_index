//
// Created by dima on 08.04.17.
//

#include "catch.hpp"
#include "../../src/search/SubspacesProductNearestIndicesSearcher.h"
#include "../../src/util/array_utils.h"


TEST_CASE("find nearest subspace indices by distances[2, 3]", "[nearest][subspace]") {

    const int max_n_nearest = 3;
    const int subspace_count = 2;
    const SubspacesProductNearestIndicesSearcher subspacesProductNearestIndicesSearcher(max_n_nearest, subspace_count);

    const int distances_count_in_each_subspace = 3;
    const float subspace_distances[] = {1.2, 0, 5,
                                        0, 1, 2};

    const int n_nearest_in_each_subspace = max_n_nearest;
    const int n_nearest_ = subspace_count * n_nearest_in_each_subspace;
    int subspace_nearest_indices[n_nearest_] = {-1, -1, -1,
                                                -1, -1, -1};
    subspacesProductNearestIndicesSearcher.findNearestIndices(subspace_distances, distances_count_in_each_subspace,
                                                              subspace_nearest_indices, n_nearest_in_each_subspace);

    std::vector<int> answer_vector = array_to_vector(subspace_nearest_indices, n_nearest_);
    std::vector<int> right_answer_vector = {1, 0, 2,
                                            0, 1, 2};
    REQUIRE(answer_vector == right_answer_vector);
}
