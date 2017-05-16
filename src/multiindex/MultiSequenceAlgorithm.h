#ifndef MULTISEQUENCEALGORITHM_H
#define MULTISEQUENCEALGORITHM_H

//#define MY_DEBUG

#include "../util/array_utils.h"
#include "../util/PriorityTuple.h"
#include <queue>
#include "MultiIndexUtil.h"
#include "InvertedMultiIndex.h"
#include <exception>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <memory>

template<class T>
class MultiSequenceAlgorithm {
    //suppose each codebook has invertedMultiIndex->centroids_count_in_each_subspace codewords
private:
    const InvertedMultiIndex<T> *invertedMultiIndex;
    MultiIndexUtil multiIndexUtil;

    float PREV_PRIORITY = -1;

    void CHECK_ALGORITHM_ITSELF(bool *visited, int flatindex, float popped_priority) {
        if (visited[flatindex]) {
            fprintf(stderr, "flatindex: %d ", flatindex);
            perror("cell already visited. Error in implementation");

            exit(1);
        }
        if (popped_priority < this->PREV_PRIORITY) {
            perror("popped prioirties must increase. Error in implementation");
            exit(1);
        } else {
            PREV_PRIORITY = popped_priority;
        }
    }

    std::pair<int, int> find_list_slice_for_entries(const int pos_in_list_start_ndarray) {
        int list_start = invertedMultiIndex->entries_list_starts[pos_in_list_start_ndarray];
        int next_list_start = invertedMultiIndex->entries_list_starts[pos_in_list_start_ndarray + 1];
        int list_size = next_list_start - list_start;

        std::pair<int, int> list_slice(list_start, list_start + list_size);

        return list_slice;
    }

    bool check_cell_out_of_bounds(const int *multi_index) {
        for (int i = 0; i < invertedMultiIndex->subspaces_count; i++) {
            if (multi_index[i] >= this->invertedMultiIndex->centroids_count_in_each_subspace) {
                return true;
            }
        }
        return false;
    }

    bool check_cell_already_visited(const int *multi_index, bool *visited) {
        int flatindex = multiIndexUtil.flat_index(multi_index);
        bool already_visited = visited[flatindex];
        return already_visited;
    }

    bool check_surrounding_cells_visited(const int *multi_index, bool *visited) {
        bool surrounding_cells_visited = true;
        for (int i = 0; i < invertedMultiIndex->subspaces_count; i++) {
            bool is_border = multi_index[i] == 0;
            bool surrounding_cell_i_visited = is_border;
            if (!is_border) {
                int flatindex = multiIndexUtil.flat_index(multi_index);
                int flatindex_inc = multiIndexUtil.flat_dim_offset(i);
                flatindex -= flatindex_inc;
                surrounding_cell_i_visited = visited[flatindex];
            }
            surrounding_cells_visited = surrounding_cells_visited && surrounding_cell_i_visited;

            if (!surrounding_cell_i_visited)
                return false;
        }

        return surrounding_cells_visited;
    }

    bool check_for_push(bool *visited, const int *multi_index) {
        bool should_push = true;

        bool is_out_of_bounds = check_cell_out_of_bounds(multi_index);
        should_push = !is_out_of_bounds;
        if (should_push) {
            bool is_cell_already_visited = check_cell_already_visited(multi_index, visited);
            should_push = !is_cell_already_visited;
            if (should_push) {
                bool surrounding_cells_visited = check_surrounding_cells_visited(multi_index, visited);
                should_push = surrounding_cells_visited;
            }
        }

        return should_push;
    }

public:

    MultiSequenceAlgorithm(InvertedMultiIndex<T> *invertedMultiIndex) :
            invertedMultiIndex(invertedMultiIndex),
            multiIndexUtil(invertedMultiIndex->subspaces_count, invertedMultiIndex->centroids_count_in_each_subspace) {
    }

    //���� ���������� ��������������� ������� ���������� ��� ����, ����� �� ���������� nearest_cluster_index_matrix - �� ���������. ����� ������� ��������
    //cluster_distance_matrix[subspaces_count, centroids_count_in_each_subspace]
    //nearest_cluster_index_matrix[subspaces_count, centroids_count_in_each_subspace]
    void find_and_write_candidates(const float *cluster_distance_matrix, const int *nearest_cluster_index_matrix,
                                   T *out_candidate_list, const int out_candidate_list_len) {
        PREV_PRIORITY = -1;
        int m = this->invertedMultiIndex->subspaces_count;
        int K = this->invertedMultiIndex->centroids_count_in_each_subspace;

#ifdef MY_DEBUG
        printf("find_and_write_candidates: m:%d K:%d\n", m, K);
        printf("entries:\n");
        print_array(invertedMultiIndex->entries, invertedMultiIndex->entries_count, "%d ", false);
        printf("entries_list_starts:\n");
        print_array(invertedMultiIndex->entries_list_starts, invertedMultiIndex->entries_list_starts_len, "%d ", false);
        printf("cluster_distance_matrix:\n");
        print_array(cluster_distance_matrix, m*K, "%f ", false);
        printf("nearest_cluster_index_matrix:\n");
        print_array(nearest_cluster_index_matrix, m*K, "%d ", false);
#endif

        std::unique_ptr<int[]> _cluster_index_array(new int[m]);
//        int *_cluster_index_array = new int[m];
        float *_distances_array = new float[m];

        int *start_cell_multiindex = new int[m];
        memset(start_cell_multiindex, 0, sizeof(int) * m);

        int n_total_cells = 1;
        for (int i = 0; i < m; i++) {
            n_total_cells *= K;
        }
        bool *visited = new bool[n_total_cells];
        memset(visited, false, sizeof(bool) * n_total_cells);

#ifdef MY_DEBUG
        printf("find_and_write_candidates: m:%d K:%d\n", m, K);
        print_array(invertedMultiIndex->entries, invertedMultiIndex->entries_count, "%d ", false);
        print_array(invertedMultiIndex->entries_list_starts, invertedMultiIndex->entries_list_starts_len, "%d ", false);
#endif

        take_in_rows(nearest_cluster_index_matrix, K, start_cell_multiindex, _cluster_index_array.get(), m);
        take_in_rows(cluster_distance_matrix, K, _cluster_index_array.get(), _distances_array, m);
        float start_distance = sum_array(_distances_array, m);

        std::priority_queue<PriorityTuple<int *>, std::vector<PriorityTuple<int *>>, ComparePriorityTuple<int *>> min_heap;
        PriorityTuple<int *> start_element = {start_distance, start_cell_multiindex};
        min_heap.push(start_element);
        float PREV_PRIORITY = start_element.priority;

        int total_candidates_to_take = out_candidate_list_len;
        int candidates_taken = 0;

        try {
            while (candidates_taken < total_candidates_to_take) {

#ifdef MY_DEBUG
                if (min_heap.empty()) {
                    perror("min_heap is empty. Error in implementation");
                    exit(1);
                }
#endif
                PriorityTuple<int *> nearest = min_heap.top();
                min_heap.pop();
                int *nearest_cell_multiindex = nearest.value;
                int flatindex = multiIndexUtil.flat_index(nearest_cell_multiindex);
#ifdef MY_DEBUG
                printf("popped: (flat_index: %d, priority: %f)\n", flatindex, nearest.priority);
//                print_multi_index(nearest_cell_multiindex, m);
                CHECK_ALGORITHM_ITSELF(visited, flatindex, nearest.priority);
#endif
                visited[flatindex] = true;
                take_in_rows(nearest_cluster_index_matrix, K, nearest_cell_multiindex, _cluster_index_array.get(), m);
                int pos_in_list_start_ndarray = multiIndexUtil.flat_index(_cluster_index_array.get());
                std::pair<int, int> list_slice = find_list_slice_for_entries(pos_in_list_start_ndarray);
                int copy_len = copy_array<T>(invertedMultiIndex->entries, out_candidate_list, list_slice,
                                             candidates_taken, total_candidates_to_take);
                candidates_taken += copy_len;

#ifdef MY_DEBUG
                printf("candidates_taken: %d\n", candidates_taken);
                printf("candidates: ");
                print_array(out_candidate_list, candidates_taken, "%d ", false);
#endif

                for (int dim = m - 1; dim >= 0; dim--) {
                    int *next_cell_multi_index = new int[m];
                    copy_array<int>(nearest_cell_multiindex, next_cell_multi_index, m);
                    next_cell_multi_index[dim] += 1;

                    bool can_push = check_for_push(visited, next_cell_multi_index);
                    if (can_push) {
                        take_in_rows(nearest_cluster_index_matrix, K, next_cell_multi_index, _cluster_index_array.get(),
                                     m);
                        take_in_rows(cluster_distance_matrix, K, _cluster_index_array.get(), _distances_array, m);
                        float next_cell_distance = sum_array(_distances_array, m);

                        PriorityTuple<int *> next_consider_cell = {next_cell_distance, next_cell_multi_index};
                        min_heap.push(next_consider_cell);

#ifdef MY_DEBUG
                        printf("	push: (flat_index: %d, priority: %f)",
                               multiIndexUtil.flat_index(next_consider_cell.value), next_consider_cell.priority);
//                        print_multi_index(next_consider_cell.value, m);
                        printf("\n");
#endif

                    } else {
                        delete[] next_cell_multi_index;
                    }
                }
                delete[] nearest_cell_multiindex;
            }

        }
        catch (std::exception &e) {
            printf("%s\n", e.what());
            exit(1);
        }

        while (!min_heap.empty()) {
            PriorityTuple<int *> cell_leftover = min_heap.top();
            min_heap.pop();
            delete[] cell_leftover.value;
        }

//        delete[] _cluster_index_array;
        delete[] _distances_array;
        delete[] visited;
    }
};

#endif