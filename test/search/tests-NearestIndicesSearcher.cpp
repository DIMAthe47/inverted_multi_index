//
// Created by dima on 08.04.17.
//

#include "catch.hpp"
#include "../../src/search/NearestIndicesSearcher.h"


TEST_CASE("find nearest indices by distances[3]", "[nearest][NearestIndicesSearcher]") {

    const int max_n_nearest = 3;
    const NearestIndicesSearcher<float> nearestIndicesSearcher(max_n_nearest);

    const int distances_count = 3;
    const float distances[] = {1.2, 0, 5};
    int nearest_indices[] = {-1, -1, -1};
    const float n_nearest = max_n_nearest;
    nearestIndicesSearcher.findNearestIndices(distances, distances_count, nearest_indices, n_nearest);

    int right_answer[] = {1, 0, 2};
    std::vector<int> answer_vector(nearest_indices, nearest_indices + sizeof(nearest_indices) / sizeof(int));
    std::vector<int> right_answer_vector(right_answer, right_answer + sizeof(right_answer) / sizeof(int));
    REQUIRE(answer_vector == right_answer_vector);
}

TEST_CASE("find nearest indices matrix by distances[3]", "[nearest][NearestIndicesSearcher]") {

    const int max_n_nearest = 3;
    const NearestIndicesSearcher<float> nearestIndicesSearcher(max_n_nearest);

    const float distances[] = {
            1.2, 0, 5,
            2, 1, 0

    };

    std::vector<int> nearest_indices(2 * 3, -1);
    const int n_nearest = max_n_nearest;
    nearestIndicesSearcher.findNearestIndices(distances, 3, 2, nearest_indices.data(), n_nearest);

    std::vector<int> right_answer={
            1,0,2,
            2,1,0
    };
    REQUIRE(nearest_indices == right_answer);
}
