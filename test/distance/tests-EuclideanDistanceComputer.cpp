//
// Created by dima on 08.04.17.
//
#include "catch.hpp"
#include "../../src/distance/EuclideanDistanceComputer.h"
#include "../../src/util/array_utils.h"


TEST_CASE("compute distances from query_vector to base_vectors[3,4]", "[distance]") {
    const int base_vectors_count = 3;
    const int base_vectors_dim = 4;
    float base_vectors[base_vectors_count * base_vectors_dim] = {1, 2, 3, 4,
                                                                 3, 4, 0, 0,
                                                                 0, 0, 0, 0};
    float query[base_vectors_dim] = {1, 2, 4, 4};
    EuclideanDistanceComputer euclideanDistanceComputer(base_vectors, 3, 4);
    const int distances_count = base_vectors_count;
    float distances[distances_count];
    euclideanDistanceComputer.computeDistances(query, &distances[0]);

//    printf("%f %f %f\n", distances[0], distances[1], distances[2]);

    std::vector<float> answer_vector = array_to_vector(distances, distances_count);
    std::vector<float> right_answer_vector = {1.0, 40.0, 37.0};
    REQUIRE(answer_vector == right_answer_vector);
}
