//
// Created by dima on 08.04.17.
//

#include "catch.hpp"
#include "../../src/multiindex/InvertedMultiIndex.h"
#include "../../src/multiindex/InvertedMultiIndexBuilder.h"
#include "../../src/util/array_utils.h"


TEST_CASE("build InvertedMultiIndex", "[multiindex]") {
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

    int cells_ = 1;
    for (int i = 0; i < subspaces_count; i++) {
        cells_ *= centroids_count_in_each_subspace;
    }
    std::vector<int> entries_list_starts_vector = array_to_vector(invertedMultiIndex->entries_list_starts,
                                                                  cells_);
    std::vector<int> right_entries_list_start_vector = {0, 5, 6, 7, 7, 8, 9, 9};
    REQUIRE(entries_list_starts_vector == right_entries_list_start_vector);

    std::vector<int> entries = array_to_vector(invertedMultiIndex->entries,
                                               invertedMultiIndex->entries_count);
    std::vector<int> right_entries = {60, 50, 40, 30, 10, 70, 80, 90, 100, 20};
    REQUIRE(entries == right_entries);
}
