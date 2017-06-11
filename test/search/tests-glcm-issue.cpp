//
// Created by Dima on 29.05.2017.
//

#include "catch.hpp"
#include "../../src/distance/SubspacesProductEuclideanDistanceComputer.h"
#include "../../src/util/array_utils.h"
#include "../../src/multiindex/InvertedMultiIndex.h"
#include "../../src/multiindex/InvertedMultiIndexBuilder.h"
#include "../../src/multiindex/InvertedMultiIndexSearcher.h"


TEST_CASE("glcm issue",
          "[distance][subspace][SubspacedVectors][SubspacesProductEuclideanDistanceComputer]") {

    const int subspaces_count = 2;
    const int subspace_base_vectors_count = 256;
    const int subspace_vector_dim = 32768;
//    const int subspace_vector_dim = 10;
    SubspacedVectors<float> subspacedVectors(subspaces_count, subspace_base_vectors_count, subspace_vector_dim);

    std::unique_ptr<float[]> subspace_base_vectors(
            new float[subspaces_count * subspace_base_vectors_count * subspace_vector_dim]);
    subspacedVectors.setSubspacedVectors(subspace_base_vectors.get());

    std::unique_ptr<float[]> query_vector(new float[subspaces_count * subspace_vector_dim]);

    SECTION("survive distance") {
        SubspacesProductEuclideanDistanceComputer<float> subspacesProductEuclideanDistanceComputer(subspacedVectors);
        const int distances_count_ = subspacedVectors.getTotalVectorsCount();


        SECTION("raw array") {
            float subspace_distances[distances_count_];
            subspacesProductEuclideanDistanceComputer.
                    computeSquaredDistances(query_vector.get(), subspace_distances
            );
            std::vector<float> answer_vector = array_to_vector(subspace_distances, distances_count_);
        }
    }

    SECTION("survive imi") {

        int x_ids[1] = {1};
        float *x = new float[subspace_vector_dim * subspaces_count];
        Vectors<float> x_vectors(x, 1, subspace_vector_dim * subspaces_count);
        InvertedMultiIndex<int> *imi = InvertedMultiIndexBuilder<int>().buildInvertedMultiIndex(&x_ids[0], x_vectors,
                                                                                                subspacedVectors);
        InvertedMultiIndexSearcher<int, float> imis(imi, subspacedVectors);
        int *nearest = new int[1];
        imis.findNearest(query_vector.get(), nearest, 1);
    }

}