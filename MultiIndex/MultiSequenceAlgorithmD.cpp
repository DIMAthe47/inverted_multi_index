#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <algorithm>    // std::copy
#include "common.h"
#include <ctime>
//using namespace std;

void print_multi_index(const int* multi_index, const int n_dims) {
	printf("(");
	for (int i = 0; i < n_dims; i++) {
		printf("%d ", multi_index[i]);
	}
	printf(")");
}

int flat_index(const int* multi_index, const int n_multi_index_dims, const int* multi_index_dim_length_array) {
	int flatindex = 0;
	for (int dim = 0; dim < n_multi_index_dims; dim++) {
		int dim_offset = 1;
		for (int j = dim + 1; j < n_multi_index_dims; j++) {
			//можно сделать кумулятивное произведение
			dim_offset *= multi_index_dim_length_array[j];
		}
		flatindex += multi_index[dim] * dim_offset;
	}
	return flatindex;
}

int flat_dim_offset(const int singular_dim, const int n_multi_index_dims, const int* multi_index_dim_length_array) {
	int dim_offset = 1;
	for (int j = singular_dim + 1; j < n_multi_index_dims; j++) {
		dim_offset *= multi_index_dim_length_array[j];
	}
	int flatindex = dim_offset;
	return flatindex;
}

template <typename T>
void take_in_rows(const T** matrix, const int* columns, T* target_array, const int target_array_len) {
	for (int i = 0; i < target_array_len; i++) {
		int column = columns[i];
		target_array[i] = matrix[i][column];
	}
}

//pos_in_list_start_ndarray must be < len(list_start_ndarray)
std::pair<int, int> find_list_sliceD(const int* list_start_ndarray, const int pos_in_list_start_ndarray) {
	int list_start = list_start_ndarray[pos_in_list_start_ndarray];

	int list_size;
	int next_list_start = list_start_ndarray[pos_in_list_start_ndarray + 1];
	list_size = next_list_start - list_start;

	std::pair<int, int> list_slice(list_start, list_start + list_size);

	return list_slice;
}

bool check_cell_out_of_bounds(const int* multi_index, const int n_dims, const int*  multi_index_dim_length_array) {
	for (int i = 0; i < n_dims; i++) {
		if (multi_index[i] >= multi_index_dim_length_array[i]) {
			return true;
		}
	}
	return false;
	//bool next_cell_out_of_bounds = multi_index[dim_to_inc] + 1 >= multi_index_dim_length_array[dim_to_inc];
	//return next_cell_out_of_bounds;
}

bool check_cell_already_visited(const int* multi_index, const int n_dims, const int*  multi_index_dim_length_array, bool* visited) {
	int flatindex = flat_index(multi_index, n_dims, multi_index_dim_length_array);
	//int flatindex_inc = flat_dim_offset(dim_to_inc, n_dims, multi_index_dim_length_array);
	//flatindex += flatindex_inc;

	bool already_visited = visited[flatindex];
	return already_visited;
}

bool check_surrounding_cells_visited(const int* multi_index, const int n_dims, const int* multi_index_dim_length_array, bool* visited) {
	bool surrounding_cells_visited = true;
	for (int i = 0; i < n_dims; i++) {
		//multi_index_copy[i]--;
		bool is_border = multi_index[i] == 0;
		bool surrounding_cell_i_visited = is_border;
		if (!is_border) {
			int flatindex = flat_index(multi_index, n_dims, multi_index_dim_length_array);
			int flatindex_inc = flat_dim_offset(i, n_dims, multi_index_dim_length_array);
			flatindex -= flatindex_inc;
			//multi_index_copy[i]++;
			surrounding_cell_i_visited = visited[flatindex];
		}
		surrounding_cells_visited = surrounding_cells_visited&& surrounding_cell_i_visited;

		if (!surrounding_cell_i_visited)
			break;
	}

	return surrounding_cells_visited;
}

//внутри изменяется multi_index и восстанавливается обратно
bool check_for_push(bool *visited, const int* multi_index, const int n_dims, const int* multi_index_dim_length_array) {
	bool should_push = true;

	bool is_cell_already_visited = check_cell_already_visited(multi_index, n_dims, multi_index_dim_length_array, visited);
	should_push = !is_cell_already_visited;

	if (!is_cell_already_visited) {
		bool is_out_of_bounds = check_cell_out_of_bounds(multi_index, n_dims, multi_index_dim_length_array);
		should_push = !is_out_of_bounds;

		if (!is_out_of_bounds) {
			//bool is_border = check_multi_index_border(multi_index, n_dims);
			//should_push = is_border;
			//if (!is_border) {
				bool surrounding_cells_visited = check_surrounding_cells_visited(multi_index, n_dims, multi_index_dim_length_array, visited);
				should_push = surrounding_cells_visited;
			//}
		}
	}

	return should_push;
}
/*
void brute_iterate_fill(bool *visited, int n_dims, int* multi_index_dim_length_array) {
	int* _multi_index = new int[n_dims];
	memset(_multi_index, 0, sizeof(int)*n_dims);
	for (int fixed_dim = 0; fixed_dim < n_dims; fixed_dim++) {
		int n_points = prod_array(multi_index_dim_length_array, n_dims);
		n_points = n_points / multi_index_dim_length_array[fixed_dim];
		for (int i = 0; i < n_points; i++) {
			for (int last_dim_pos = 0; last_dim_pos < multi_index_dim_length_array[n_dims - 1]; last_dim_pos++) {
				_multi_index[n_dims - 1]++;
			}
			_multi_index[n_dims - 1] = 0;
			_multi_index[n_dims - 2] += 1;
			for (int dim = n_dims - 2; dim > 0; dim--) {
				if (_multi_index[dim] == multi_index_dim_length_array[dim]) {
					_multi_index[dim] = 0;
					_multi_index[dim]
				}
			}
			_multi_index[n_dims - 1] += 1;
			if (_multi_index[n_dims - 1])
				int flatindex = flat_index(_multi_index, m, n_clusters_array);
			visited[flatindex] = true;
		}
	}
	delete[]_multi_index;
}
*/

template <typename T>
void multisequence_algorithm_get_candidatesD(const float **cluster_distance_matrix, const int** nearest_cluster_index_matrix, const int n_subquantizers, const int* n_clusters_array,
	const int* list_start_ndarray, const T* lists, const int lists_len, T* candidate_list, const int candidate_list_len) {
	int m = n_subquantizers;
	int* _cluster_index_array = new int[m];
	float* _distances_array = new float[m];

	int* start_cell_multiindex = new int[m];
	memset(start_cell_multiindex, 0, sizeof(int)*m);

	int n_total_cells = prod_array(n_clusters_array, m);
	bool* visited = new bool[n_total_cells];
	memset(visited, false, sizeof(bool)*n_total_cells);

	take_in_rows(nearest_cluster_index_matrix, start_cell_multiindex, _cluster_index_array, m);
	take_in_rows(cluster_distance_matrix, _cluster_index_array, _distances_array, m);
	float start_distance = sum_array(_distances_array, m);

	std::priority_queue<PriorityTuple<int*>, std::vector<PriorityTuple<int*>>, ComparePriorityTuple<int*>> min_heap;
	PriorityTuple<int*> start_element = { start_distance, start_cell_multiindex };
	min_heap.push(start_element);
	float PREV_PRIORITY = start_element.priority;

	int* nearest_cluster_index_array = new int[m];
	int total_candidates_to_take = candidate_list_len;
	int candidates_taken = 0;
	while (candidates_taken < total_candidates_to_take) {
		PriorityTuple<int*> nearest = min_heap.top();
		min_heap.pop();
		int* nearest_cell_multiindex = nearest.value;
		//printf("popped: %f ", nearest.priority);
		//print_multi_index(nearest_cell_multiindex, m);
		//printf("\n");
		int flatindex = flat_index(nearest_cell_multiindex, m, n_clusters_array);


		if (visited[flatindex]) {
			perror("cell already visited. Error in implementation");
			exit(1);
		}
		if (nearest.priority < PREV_PRIORITY) {
			perror("cell already visited. Error in implementation");
			exit(1);
		}
		else {
			PREV_PRIORITY = nearest.priority;
		}


		visited[flatindex] = true;
		take_in_rows(nearest_cluster_index_matrix, nearest_cell_multiindex, nearest_cluster_index_array, m);
		int pos_in_list_start_ndarray = flat_index(nearest_cluster_index_array, m, n_clusters_array);
		std::pair<int, int> list_slice = find_list_sliceD(list_start_ndarray, pos_in_list_start_ndarray);
		int copy_len = copy_array(lists, candidate_list, list_slice, candidates_taken, total_candidates_to_take);
		candidates_taken += copy_len;
		//printf("candidates: ");
		//print_array(candidate_list, candidates_taken, "%d ");


		for (int dim = m-1; dim>=0; dim--) {
			int* next_cell_multi_index = new int[m];
			copy_array(nearest_cell_multiindex, next_cell_multi_index, m);
			next_cell_multi_index[dim] += 1;

			bool can_push = check_for_push(visited, next_cell_multi_index, m, n_clusters_array);
			if (can_push) {
				take_in_rows(nearest_cluster_index_matrix, next_cell_multi_index, _cluster_index_array, m);
				take_in_rows(cluster_distance_matrix, _cluster_index_array, _distances_array, m);
				float next_cell_distance = sum_array(_distances_array, m);

				PriorityTuple<int*> next_consider_cell = { next_cell_distance,next_cell_multi_index };
				min_heap.push(next_consider_cell);

				//printf("	push: %f ", next_consider_cell.priority);
				//print_multi_index(next_consider_cell.value, m);
				//printf("\n");

			}
			else {
				delete[] next_cell_multi_index;
			}
		}
		delete[] nearest_cell_multiindex;

	}

}


int main22() {
	float u_distaces[] = { 0., 1., 10., 2. };
	int u_nearest_indices[] = { 0, 1, 3, 2 };

	const float** cluster_distance_matrix = new const float*[2]{ u_distaces ,u_distaces };
	const int** nearest_cluster_index_matrix = new const int*[2]{ u_nearest_indices, u_nearest_indices };
	int n_subquantizers = 2;
	int n_clusters_array[] = { 4,4 };
	int v_nearest_indices[] = { 0, 1, 3, 2 };
	//буфферный элемент в конце
	int list_start_ndarray[] = { 0, 0, 0, 0, 0, 1, 1, 2, 2, 4, 4, 4, 4, 6, 8, 8,  10 };
	int lists[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
	int lists_len = 10;

	int* candidates = new int[10];
	multisequence_algorithm_get_candidatesD<int>(cluster_distance_matrix, nearest_cluster_index_matrix, n_subquantizers, n_clusters_array, list_start_ndarray, lists, lists_len, candidates, 10);
	printf("before delete[] candidates\n");
	delete[] candidates;
	printf("after delete[] candidates\n");
	return 0;
}

int main() {
	float u_distaces[] = { 2.0, 0. };
	float v_distaces[] = { 2.0, 0., 1.0 };
	float z_distaces[] = { 2.0, 0. };
	int u_nearest_indices[] = { 1, 0 };
	int v_nearest_indices[] = { 1, 2, 0 };
	int z_nearest_indices[] = { 1, 0 };

	const float** cluster_distance_matrix = new const float*[3]{ u_distaces ,v_distaces,z_distaces };
	const int** nearest_cluster_index_matrix = new const int*[3]{ u_nearest_indices, v_nearest_indices, z_nearest_indices };
	int n_subquantizers = 3;
	int n_clusters_array[] = { 2,3,2 };
	//буфферный элемент в конце
	int list_start_ndarray[] = {
		0, 0, 0,
		1, 1, 2,
		2, 4, 4,
		6, 8, 8,
		10 };
	int lists[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
	int lists_len = 10;

	int* candidates = new int[10];
	multisequence_algorithm_get_candidatesD<int>(cluster_distance_matrix, nearest_cluster_index_matrix, n_subquantizers, n_clusters_array, list_start_ndarray, lists, lists_len, candidates, 10);
	print_array(candidates, 10, "%d ");
	delete[] candidates;
	return 0;
}


int main3D8() {
	float u_distaces[] = { 2.0, 0. };
	int u_nearest_indices[] = { 1, 0 };

	const float** cluster_distance_matrix = new const float*[3]{ u_distaces ,u_distaces,u_distaces };
	const int** nearest_cluster_index_matrix = new const int*[3]{ u_nearest_indices, u_nearest_indices, u_nearest_indices };
	int n_subquantizers = 3;
	int n_clusters_array[] = { 2,2,2 };
	//буфферный элемент в конце
	int list_start_ndarray[] = { 0, 1, 2, 2, 4, 4, 6, 8,    10 };
	int lists[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
	int lists_len = 10;

	int* candidates = new int[10];
	multisequence_algorithm_get_candidatesD<int>(cluster_distance_matrix, nearest_cluster_index_matrix, n_subquantizers, n_clusters_array, list_start_ndarray, lists, lists_len, candidates, 10);
	delete[] candidates;
	return 0;
}

int main4d() {
	float u_distaces[] = { 0., 1. };
	int u_nearest_indices[] = { 0, 1, };

	const float** cluster_distance_matrix = new const float*[4]{ u_distaces ,u_distaces,u_distaces,u_distaces };
	const int** nearest_cluster_index_matrix = new const int*[4]{ u_nearest_indices, u_nearest_indices, u_nearest_indices,u_nearest_indices };
	int n_subquantizers = 4;
	int n_clusters_array[] = { 2,2,2,2 };
	//буфферный элемент в конце
	int list_start_ndarray[] = { 0, 0, 0, 0, 0, 1, 1, 2, 2, 4, 4, 4, 4, 6, 8, 8,     10 };
	int lists[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
	int lists_len = 10;

	int* candidates = new int[10];
	multisequence_algorithm_get_candidatesD<int>(cluster_distance_matrix, nearest_cluster_index_matrix, n_subquantizers, n_clusters_array, list_start_ndarray, lists, lists_len, candidates, 10);
	printf("before delete[] candidates\n");
	delete[] candidates;
	printf("after delete[] candidates\n");
	return 0;
}

int main2() {

	int multi_index[] = { 1,2,0,3, };
	int multi_index_dim_length_array[] = { 5,2,4,3 };
	int fi = flat_index(multi_index, 4, multi_index_dim_length_array);
	printf("%d\n", fi);

	int multi_index2[] = { 1,2,0,3 };
	int multi_index_dim_length_array2[] = { 4,4,4,4 };
	int fi2 = flat_index(multi_index2, 4, multi_index_dim_length_array2);
	printf("%d\n", fi2);


	const int n = 3;
	const int **matrix = new const int*[n];
	for (int i = 0; i < n; i++) {
		matrix[i] = new int[n] {1, 2, 3};
		/*for (int j = 0; j < n; j++) {
			matrix[i][j] = i + j;
		}*/
	}
	print_matrix(matrix, n, n, "%d ");


	const int columns[] = { 1,2,0 };
	int* target_array = new int[3];
	take_in_rows(matrix, columns, target_array, 3);
	print_array(target_array, 3, "%d ");

	int sum1 = sum_array(target_array, 3);
	printf("%d\n", sum1);

	float float_array[] = { 0.1,0.2,0.3 };
	float sum2 = sum_array(float_array, 3);
	printf("%f\n", sum2);

	return 0;
}
int create_and_delete(int n) {
	int* arr = new int[n];
	for (int i = 0; i < n; i++) {
		arr[i] = i;
	}

	int* arr2 = new int[n];
	//std::copy(arr, arr+n, arr2);


	delete[] arr;
	delete[] arr2;
	return 0;
}


int main5() {
	clock_t begin = clock();
	for (int i = 0; i < 10000000; i++) {
		create_and_delete(10);
	}
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	printf("elapsed_secs: %f", elapsed_secs);
	char c = getchar();
	return 0;
}