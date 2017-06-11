//
// Created by dima on 08.04.17.
//
#include "catch.hpp"
#include "../../src/distance/SymmetricDistanceComputer.h"
#include "../../src/util/array_utils.h"
#include <limits>

TEST_CASE("compute distances from query_vector_codes to base_vectors_codes", "[SymmetricDistanceComputer][distance]") {
    const int n_base_vectors = 3;
    const int n_subspaces = 2;
    int base_vectors_codes[n_base_vectors * n_subspaces] = {
            0, 0,
            1, 2,
            3, 3};
    float centroids_pairwise_squared_distances[2 * 4 * 4] = {
            0, 1, 1, 4,
            1, 0, 2, 1,
            1, 2, 0, 3,
            4, 1, 3, 0,

            0, 1, 1, 1,
            1, 0, 2, 1,
            1, 2, 0, 4,
            1, 1, 4, 0
    };

    int query_codes[n_subspaces] = {1, 2};

    SymmetricDistanceComputer<int, float> symmetricDistanceComputer(&base_vectors_codes[0], n_base_vectors,
                                                                    &centroids_pairwise_squared_distances[0], 2, 4);
    float distances[n_base_vectors];
    symmetricDistanceComputer.computeSquaredDistances(&query_codes[0], &distances[0]);

    std::vector<float> answer_vector = array_to_vector(distances, n_base_vectors);
    std::vector<float> right_answer_vector = {1 + 1, 0 + 0, 1 + 4};
    REQUIRE(answer_vector == right_answer_vector);
}

TEST_CASE("compute distances from query_vectors_codes to base_vectors_codes", "[SymmetricDistanceComputer][distance]") {
    const int n_base_vectors = 3;
    const int n_subspaces = 2;
    int base_vectors_codes[n_base_vectors * n_subspaces] = {
            0, 0,
            1, 2,
            3, 3};
    float centroids_pairwise_squared_distances[2 * 4 * 4] = {
            0, 1, 1, 4,
            1, 0, 2, 1,
            1, 2, 0, 3,
            4, 1, 3, 0,

            0, 1, 1, 1,
            1, 0, 2, 1,
            1, 2, 0, 4,
            1, 1, 4, 0
    };

    int queries_codes[2 * n_subspaces] = {
            1, 2,
            0, 0
    };

    SymmetricDistanceComputer<int, float> symmetricDistanceComputer(&base_vectors_codes[0], n_base_vectors,
                                                                    &centroids_pairwise_squared_distances[0], 2, 4);
    float distances[2 * n_base_vectors];
    symmetricDistanceComputer.computePairwiseSquaredDistances(&queries_codes[0], 2, &distances[0]);

    std::vector<float> answer_vector1 = array_to_vector(distances, n_base_vectors);
    std::vector<float> right_answer_vector1 = {1 + 1, 0 + 0, 1 + 4};
    REQUIRE(answer_vector1 == right_answer_vector1);

    std::vector<float> answer_vector2 = array_to_vector(distances + n_base_vectors, n_base_vectors);
    std::vector<float> right_answer_vector2 = {0 + 0, 1 + 1, 4 + 1};
    REQUIRE(answer_vector2 == right_answer_vector2);
}

TEST_CASE("survive big arrays distances computation SymmetricDistanceComputer", "[EuclideanDistanceComputer][distance]") {
    const int n_base_vectors = 1 << 20;
    const int n_subspaces = 4;
    std::vector<int> base_vectors_codes(n_base_vectors * n_subspaces, 0);
    const int n_centroids = 256;
    const int n_queries_vectors = 1 << 10;
    std::vector<float> centroids_pairwise_squared_distances(n_subspaces * n_centroids * n_centroids, 0);
    std::vector<int> queries_codes(n_queries_vectors * n_subspaces, 0);

    SymmetricDistanceComputer<int, float> symmetricDistanceComputer(base_vectors_codes.data(), n_base_vectors,
                                                                    centroids_pairwise_squared_distances.data(),
                                                                    n_subspaces, n_centroids);
    std::vector<float> distances(n_queries_vectors * n_base_vectors);
    symmetricDistanceComputer.computePairwiseSquaredDistances(queries_codes.data(), n_queries_vectors,
                                                              distances.data());

    std::vector<float> right_answer_distances(n_queries_vectors * n_base_vectors, 0);
    REQUIRE(distances == right_answer_distances);
}
