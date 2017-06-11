//
// Created by dima on 08.04.17.
//
#include "catch.hpp"
#include "../../src/distance/AsymmetricDistanceComputer.h"
#include "../../src/util/array_utils.h"
#include <limits>

TEST_CASE("compute ADC from query_vector to base_vectors", "[AsymmetricDistanceComputer][distance]") {
    const int n_base_vectors = 6;
    const int n_subspaces = 2;
    const int n_centroids_in_each_subspace = 4;
    const int subspace_dim = 5;
    int base_vectors_codes[n_base_vectors * n_subspaces] = {
            0, 0,
            1, 2,
            3, 3,
            0, 1,
            0, 1,
            1, 1,
    };
    float subspaced_centroids[n_subspaces * n_centroids_in_each_subspace * subspace_dim] = {
            0, 1, 1, 4, 0,
            1, 0, 2, 1, 0,
            1, 2, 0, 3, 0,
            4, 1, 3, 0, 0,

            0, 1, 1, 1, 0,
            1, 0, 2, 1, 0,
            1, 2, 0, 4, 0,
            1, 1, 4, 0, 0
    };

    AsymmetricDistanceComputer<int, float> asymmetricDistanceComputer(&subspaced_centroids[0], n_subspaces,
                                                                      n_centroids_in_each_subspace, subspace_dim,
                                                                      &base_vectors_codes[0], n_base_vectors);

    float query_vector[] = {1, 2, 0, 3, 0, 1, 1, 4, 0, 1};
    std::vector<float> distances(n_base_vectors);
    asymmetricDistanceComputer.computeSquaredDistances(&query_vector[0], distances.data());

    std::vector<float> right_answer_vector = {4 + 12, 12 + 34, 28 + 1, 4 + 7, 4 + 7, 12 + 7};
    REQUIRE(distances == right_answer_vector);
}

TEST_CASE("compute ADC from query_vectors_codes to base_vectors_codes", "[AsymmetricDistanceComputer][distance]") {
    const int n_base_vectors = 6;
    const int n_subspaces = 2;
    const int n_centroids_in_each_subspace = 4;
    const int subspace_dim = 5;
    int base_vectors_codes[n_base_vectors * n_subspaces] = {
            0, 0,
            1, 2,
            3, 3,
            0, 1,
            0, 1,
            1, 1,
    };
    float subspaced_centroids[n_subspaces * n_centroids_in_each_subspace * subspace_dim] = {
            0, 1, 1, 4, 0,
            1, 0, 2, 1, 0,
            1, 2, 0, 3, 0,
            4, 1, 3, 0, 0,

            0, 1, 1, 1, 0,
            1, 0, 2, 1, 0,
            1, 2, 0, 4, 0,
            1, 1, 4, 0, 0
    };

    AsymmetricDistanceComputer<int, float> asymmetricDistanceComputer(&subspaced_centroids[0], n_subspaces,
                                                                      n_centroids_in_each_subspace, subspace_dim,
                                                                      &base_vectors_codes[0], n_base_vectors);

    float query_vectors[] = {
            1, 2, 0, 3, 0, 1, 1, 4, 0, 1,
            1, 2, 0, 3, 0, 1, 1, 4, 0, 1
    };
    std::vector<float> distances(n_base_vectors * 2);
    asymmetricDistanceComputer.computePairwiseSquaredDistances(&query_vectors[0], 2, distances.data());

    std::vector<float> right_answer_vector1 = {4 + 12, 12 + 34, 28 + 1, 4 + 7, 4 + 7, 12 + 7};
    std::vector<float> answer_vector1(distances.begin(), distances.begin() + n_base_vectors);
    REQUIRE(answer_vector1 == right_answer_vector1);

    std::vector<float> right_answer_vector2 = {4 + 12, 12 + 34, 28 + 1, 4 + 7, 4 + 7, 12 + 7};
    std::vector<float> answer_vector2(distances.begin() + n_base_vectors, distances.begin() + 2 * n_base_vectors);
    REQUIRE(answer_vector2 == right_answer_vector2);
}

TEST_CASE("survive big arrays distances computation AsymmetricDistanceComputer",
          "[AsymmetricDistanceComputer][distance]") {
    const int n_base_vectors = 1 << 20;
    const int n_subspaces = 4;
    const int subspace_dim = 1 << 7;
    std::vector<int> base_vectors_codes(n_base_vectors * n_subspaces, 0);
    const int n_centroids_in_each_subspace = 256;
    const int n_queries_vectors = 1 << 10;
    std::vector<float> subspaced_centroids(n_subspaces * n_centroids_in_each_subspace * subspace_dim, 0);
    std::vector<float> queries_vectors(n_queries_vectors * n_subspaces * subspace_dim, 0);

    AsymmetricDistanceComputer<int, float> asymmetricDistanceComputer(&subspaced_centroids[0], n_subspaces,
                                                                      n_centroids_in_each_subspace, subspace_dim,
                                                                      &base_vectors_codes[0], n_base_vectors);
    std::vector<float> distances(n_queries_vectors * n_base_vectors);
    asymmetricDistanceComputer.computePairwiseSquaredDistances(queries_vectors.data(), n_queries_vectors,
                                                              distances.data());

    std::vector<float> right_answer_distances(n_queries_vectors * n_base_vectors, 0);
    REQUIRE(distances == right_answer_distances);
}
