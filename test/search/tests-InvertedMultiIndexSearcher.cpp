//
// Created by Dima on 25.04.2017.
//
#include "catch.hpp"
#include "../../src/multiindex/InvertedMultiIndexSearcher.h"
#include "../../src/multiindex/InvertedMultiIndexBuilder.h"

TEST_CASE("find nearest indices by InvertedMultiIndex", "[nearest][InvertedMultiIndexSearcher]") {
    int vector_ids[] = {10, 20, 30, 40};
    float *vectors_arr = new float[4 * 2 * 3]{
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 2, 3,
            1, 2, 3, 0, 0, 0,
            1, 2, 3, 1, 2, 3,
    };
    Vectors<float> vectors(vectors_arr, 4, 6);

    const int subspaces_count = 2;
    const int centroids_count_in_each_subspace = 5;
    const int centroid_dim = 3;
    float *subspaced_centroids = new float[subspaces_count * centroids_count_in_each_subspace * centroid_dim]{
            1, 2, 3, 5, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5,
            0, 0, 0, 5, 5, 5, 5, 5, 5, 1, 2, 3, 5, 5, 5,
    };
    SubspacedVectors<float> subspacedCentroids(subspaced_centroids, subspaces_count, centroids_count_in_each_subspace,
                                               centroid_dim);

    InvertedMultiIndex<int>* invertedMultiIndex = InvertedMultiIndexBuilder<int>().buildInvertedMultiIndex(vector_ids,
                                                                                                     vectors,
                                                                                                     subspacedCentroids);
    InvertedMultiIndexSearcher<int, float> imis(invertedMultiIndex, subspacedCentroids);

    int *nearest_entries = new int[4];
    SECTION("vector1") {
        float *query_vector = new float[2 * 3]{0, 0, 0, 0, 0, 0};
        imis.findNearest(query_vector, nearest_entries, 4);
        print_array(nearest_entries, 4, "%d ", false);
        std::vector<int> nearest_entries_vector = array_to_vector(nearest_entries, 4);
        std::vector<int> nearest_entries_vector_truth1 = {10,20,30,40};
        std::vector<int> nearest_entries_vector_truth2 = {10,30,20,40};
        REQUIRE((nearest_entries_vector==nearest_entries_vector_truth1||nearest_entries_vector==nearest_entries_vector_truth2));
    }
    SECTION("vector2") {
        float *query_vector = new float[2 * 3]{0, 0, 0, 1, 2, 3};
        imis.findNearest(query_vector, nearest_entries, 4);
        print_array(nearest_entries, 4, "%d ", false);
        std::vector<int> nearest_entries_vector = array_to_vector(nearest_entries, 4);
        std::vector<int> nearest_entries_vector_truth1 = {20,10,40,30};
        std::vector<int> nearest_entries_vector_truth2 = {20,40,10,30};
        REQUIRE((nearest_entries_vector==nearest_entries_vector_truth1||nearest_entries_vector==nearest_entries_vector_truth2));
    }
    SECTION("vector3") {
        float *query_vector = new float[2 * 3]{1, 2, 3, 0, 0, 0};
        imis.findNearest(query_vector, nearest_entries, 4);
        print_array(nearest_entries, 4, "%d ", false);
        std::vector<int> nearest_entries_vector = array_to_vector(nearest_entries, 4);
        std::vector<int> nearest_entries_vector_truth1 = {30,10,40,20};
        std::vector<int> nearest_entries_vector_truth2 = {30,40,10,20};
        REQUIRE((nearest_entries_vector==nearest_entries_vector_truth1||nearest_entries_vector==nearest_entries_vector_truth2));
    }
    SECTION("vector4") {
        float *query_vector = new float[2 * 3]{1, 2, 3, 1, 2, 3};
        imis.findNearest(query_vector, nearest_entries, 4);
        print_array(nearest_entries, 4, "%d ", false);
        std::vector<int> nearest_entries_vector = array_to_vector(nearest_entries, 4);
        std::vector<int> nearest_entries_vector_truth1 = {40,20,30,10};
        std::vector<int> nearest_entries_vector_truth2 = {40,30,20,10};
        REQUIRE((nearest_entries_vector==nearest_entries_vector_truth1||nearest_entries_vector==nearest_entries_vector_truth2));
    }
    SECTION("vector5") {
        float *query_vector = new float[2 * 3]{4, 4, 4, 5, 2, 3};
        imis.findNearest(query_vector, nearest_entries, 4);
        print_array(nearest_entries, 4, "%d ", false);
        std::vector<int> nearest_entries_vector = array_to_vector(nearest_entries, 4);
        std::vector<int> nearest_entries_vector_truth1 = {40,20,30,10};
        std::vector<int> nearest_entries_vector_truth2 = {40,30,20,10};
        REQUIRE((nearest_entries_vector==nearest_entries_vector_truth1||nearest_entries_vector==nearest_entries_vector_truth2));
    }


    delete[] subspaced_centroids;
    delete invertedMultiIndex;
}
