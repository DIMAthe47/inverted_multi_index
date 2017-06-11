//
// Created by dima on 08.04.17.
//
#include "catch.hpp"
#include "../../src/distance/EuclideanDistanceComputer.h"
#include "../../src/util/array_utils.h"
#include <limits>

TEST_CASE("compute distances from query_vector to base_vectors", "[EuclideanDistanceComputer][distance]") {
    const int base_vectors_count = 3;
    const int base_vectors_dim = 4;
    float base_vectors[base_vectors_count * base_vectors_dim] = {1, 2, 3, 4,
                                                                 3, 4, 0, 0,
                                                                 0, 0, 0, 0};
    float query[base_vectors_dim] = {1, 2, 4, 4};
    EuclideanDistanceComputer<float> euclideanDistanceComputer(base_vectors, 3, 4);
    const int distances_count = base_vectors_count;
    float distances[distances_count];
    euclideanDistanceComputer.computeSquaredDistances(query, &distances[0]);

    std::vector<float> answer_vector = array_to_vector(distances, distances_count);
    std::vector<float> right_answer_vector = {1.0, 40.0, 37.0};
    REQUIRE(answer_vector == right_answer_vector);
}

TEST_CASE("compute computePairwiseSquaredDistances from query_vectors to base_vectors", "[EuclideanDistanceComputer][distance]") {
    const int base_vectors_count = 3;
    const int base_vectors_dim = 4;
    float base_vectors[base_vectors_count * base_vectors_dim] = {1, 2, 3, 4,
                                                                 3, 4, 0, 0,
                                                                 0, 0, 0, 0};
    float queries[base_vectors_dim * 2] = {
            1, 2, 4, 4,
            3, 4, 0, 0,
    };
    EuclideanDistanceComputer<float> euclideanDistanceComputer(base_vectors, 3, 4);
    float distances[base_vectors_count * 2];
    euclideanDistanceComputer.computePairwiseSquaredDistances(queries, 2, &distances[0]);

    std::vector<float> answer_vector1 = array_to_vector(distances, base_vectors_count);
    std::vector<float> right_answer_vector1 = {1.0, 40.0, 37.0};
    REQUIRE(answer_vector1 == right_answer_vector1);

    std::vector<float> answer_vector2 = array_to_vector(&distances[base_vectors_count], base_vectors_count);
    std::vector<float> right_answer_vector2 = {33.0, 0.0, 25.0};
    REQUIRE(answer_vector2 == right_answer_vector2);
}

TEST_CASE("survive big arrays distances computation", "[EuclideanDistanceComputer][distance]") {
    const int base_vectors_count = 1 << 14;
    const int base_vectors_dim = 1 << 16;
//    float *a = new float[base_vectors_count * base_vectors_dim];
    std::unique_ptr<float[]> base_vectors(new float[base_vectors_count * base_vectors_dim]);
    for (size_t i = 0; i < base_vectors_count * base_vectors_dim; i++) {
        if (i % base_vectors_dim == 0)
            base_vectors[i] = 0.1;
        else
            base_vectors[i] = 0;
    }

    std::unique_ptr<float[]> query(new float[base_vectors_dim]);
    for (size_t i = 0; i < base_vectors_dim; i++) {
        query[i] = 0;
    }

    EuclideanDistanceComputer<float> euclideanDistanceComputer(base_vectors.get(), base_vectors_count,
                                                               base_vectors_dim);
    const int distances_count = base_vectors_count;
    float distances[distances_count];
    euclideanDistanceComputer.computeSquaredDistances(query.get(), &distances[0]);
    std::vector<float> calculated_distances = array_to_vector(distances, distances_count);
    std::vector<float> truth_distances(distances_count, 0.01);
    for (size_t i = 0; i < distances_count; i++) {
        REQUIRE(calculated_distances[i] == Approx(truth_distances[i]));
    }

}
