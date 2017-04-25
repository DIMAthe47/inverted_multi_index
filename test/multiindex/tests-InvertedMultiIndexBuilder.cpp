//
// Created by dima on 08.04.17.
//

#include "catch.hpp"
#include "../../src/multiindex/InvertedMultiIndex.h"
#include "../../src/multiindex/InvertedMultiIndexBuilder.h"
#include "../../src/util/array_utils.h"


TEST_CASE("build InvertedMultiIndex", "[multiindex][InvertedMultiIndex][InvertedMultiIndexBuilder]") {
    int vector_ids[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int vector_ids_count = 10;
    int vectors_centroid_indices[] = {
            0, 0, 0,//10
            1, 1, 1,//20
            0, 0, 0,//30
            0, 0, 0,//40
            0, 0, 0,//50
            0, 0, 0,//60
            0, 0, 1,//70
            0, 1, 0,//80
            1, 0, 0,//90
            1, 0, 1//100
    };

    const int subspaces_count = 3;
    const int centroids_count_in_each_subspace = 2;
    InvertedMultiIndex<int> *invertedMultiIndex = InvertedMultiIndexBuilder::buildInvertedMultiIndex(vector_ids,
                                                                                                     vector_ids_count,
                                                                                                     vectors_centroid_indices,
                                                                                                     subspaces_count,
                                                                                                     centroids_count_in_each_subspace);
    REQUIRE(invertedMultiIndex->entries_count == vector_ids_count);
    REQUIRE(invertedMultiIndex->subspaces_count == subspaces_count);
    REQUIRE(invertedMultiIndex->centroids_count_in_each_subspace == centroids_count_in_each_subspace);

//    print_array(invertedMultiIndex->entries_list_starts, centroids_count_in_each_subspace *
//                                                         centroids_count_in_each_subspace *
//                                                         centroids_count_in_each_subspace, "%d ", false);
//    print_array(invertedMultiIndex->entries, invertedMultiIndex->entries_count, "%d ", false);

    MultiIndexUtil multiIndexUtil(subspaces_count, centroids_count_in_each_subspace);
    int cells_ = multiIndexUtil.total_elements();

    std::vector<int> entries_list_starts_vector = array_to_vector(invertedMultiIndex->entries_list_starts,
                                                                  cells_);
    std::vector<int> right_entries_list_start_vector = {0, 5, 6, 7, 7, 8, 9, 9};
    REQUIRE(entries_list_starts_vector == right_entries_list_start_vector);

    std::vector<int> entries = array_to_vector(invertedMultiIndex->entries,
                                               invertedMultiIndex->entries_count);
    std::vector<int> right_entries = {60, 50, 40, 30, 10, 70, 80, 90, 100, 20};
    REQUIRE(entries == right_entries);
}


TEST_CASE("build InvertedMultiIndex2", "[multiindex][InvertedMultiIndex][InvertedMultiIndexBuilder]") {
    int vector_ids[] = {10, 20, 30, 40};
    int vector_ids_count = 4;
    float *vectors_arr = new float[2 * 4 * 3]{
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

    InvertedMultiIndex<int> *invertedMultiIndex = InvertedMultiIndexBuilder::buildInvertedMultiIndex(vector_ids,
                                                                                                     vectors,
                                                                                                     subspacedCentroids);

    REQUIRE(invertedMultiIndex->entries_count == vector_ids_count);
    REQUIRE(invertedMultiIndex->subspaces_count == subspaces_count);
    REQUIRE(invertedMultiIndex->centroids_count_in_each_subspace == centroids_count_in_each_subspace);

    print_array(invertedMultiIndex->entries_list_starts, invertedMultiIndex->entries_list_starts_len, "%d ", false);
    print_array(invertedMultiIndex->entries, invertedMultiIndex->entries_count, "%d ", false);


    std::vector<int> entries_list_starts_vector = array_to_vector(invertedMultiIndex->entries_list_starts,
                                                                  invertedMultiIndex->entries_list_starts_len);
    std::vector<int> right_entries_list_start_vector = {0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4,
                                                        4, 4, 4, 4, 4};
    REQUIRE(entries_list_starts_vector == right_entries_list_start_vector);

    std::vector<int> entries = array_to_vector(invertedMultiIndex->entries,
                                               invertedMultiIndex->entries_count);
    std::vector<int> right_entries = {30, 40, 10, 20};
    REQUIRE(entries == right_entries);
}
