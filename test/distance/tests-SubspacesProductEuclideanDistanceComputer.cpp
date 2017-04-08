//
// Created by dima on 08.04.17.
//
#include "catch.hpp"
#include "../../src/distance/SubspacesProductEuclideanDistanceComputer.h"
#include "../../src/util/array_utils.h"


TEST_CASE("compute subspace distances from query_vector to base_vectors[4,2,2]",
          "[distance][subspace]") {

    const int subspaces = 2;
    const int subspace_base_vectors_count = 4;
    const int subspace_vector_dim = 2;

    float subspace_base_vectors[
            subspaces * subspace_base_vectors_count * subspace_vector_dim] = {1, 2, 3, 4, 5, 6, 7, 8,
                                                                              3, 4, 0, 0, 1, 2, 1, 2};
    float query_vector[subspaces * subspace_vector_dim] = {0, 0,
                                                           3, 4};

    SubspacesProductEuclideanDistanceComputer subspacesProductEuclideanDistanceComputer(subspace_base_vectors,
                                                                                        subspaces,
                                                                                        subspace_base_vectors_count,
                                                                                        subspace_vector_dim);
    const int distances_count_ = subspaces * subspace_base_vectors_count;
    float subspace_distances[distances_count_];
    subspacesProductEuclideanDistanceComputer.computeDistances(query_vector, subspace_distances);

//    for (int i = 0; i < distances_count_; i++) {
//        printf("%f\n", subspace_distances[i]);
//    }

    std::vector<float> answer_vector = array_to_vector(subspace_distances, distances_count_);
    std::vector<float> right_answer_vector = {5, 25, 61, 113, 0, 25, 8, 8};
    REQUIRE(answer_vector == right_answer_vector);
}
