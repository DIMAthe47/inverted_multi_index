#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include "common.h"
//using namespace std;


int try_pass_numpy_arr(int* arr)
{
	arr[0] = 1000;
	return 0;
}


std::pair<int, int> find_list_slice(const int* list_starts, const int lists_len, const int n_clusters, const int ui, const int vj) {
	int list_starts_len = n_clusters*n_clusters;
	int pos_in_list_starts = ui*n_clusters + vj;
	int list_start = list_starts[pos_in_list_starts];

	int list_size;
	if (pos_in_list_starts + 1 == list_starts_len) {
		list_size = lists_len - list_start;
	}
	else {
		int next_list_start = list_starts[pos_in_list_starts + 1];
		list_size = next_list_start - list_start;
	}

	std::pair<int, int> list_slice(list_start, list_start + list_size);

	return list_slice;
}



//same length for u_distances, v_distances, u_nearest_indices, v_nearest_indices = n_clusters
template <typename T>
void multisequence_algorithm_get_candidates(const float* u_distances, const float* v_distances, const int* u_nearest_indices, const int* v_nearest_indices, const int n_clusters,
	const int* list_starts, const T* lists, const int lists_len, T* candidate_list, const int candidate_list_len) {
	const int* u = u_nearest_indices;
	const int* v = v_nearest_indices;
	const float* r = u_distances;
	const float* s = v_distances;
	int ui = 0;
	int vj = 0;
	const int K = n_clusters;

	bool* visited = new bool[K*K];
	memset(visited, false, sizeof(bool)*K*K);

	std::priority_queue<PriorityTuple2, std::vector<PriorityTuple2>, ComparePriorityTuple2> min_heap;
	PriorityTuple2 start_element = { r[u[0]] + s[v[0]], 0, 0 };
	min_heap.push(start_element);

	//int total_candidates_to_take = candidate_list_len < lists_len ? n_candidates : lists_len;
	int total_candidates_to_take = candidate_list_len;
	//T* candidate_list = new T[total_candidates_to_take];
	int candidates_taken = 0;

	while (candidates_taken < total_candidates_to_take) {
		PriorityTuple2 nearest = min_heap.top();
		min_heap.pop();
		int i = nearest.value1;
		int j = nearest.value2;
		printf("popped: %f (%d,%d) -> (%d, %d)\n", nearest.priority, i, j, u[i], v[j]);
		visited[i*K + j] = true;
		print_matrix(visited, K, K, "%d ");
		std::pair<int, int> list_slice = find_list_slice(list_starts, lists_len, n_clusters, u[i], v[j]);
		int copy_len = copy_array(lists, candidate_list, list_slice, candidates_taken, total_candidates_to_take);
		//printf("copied: %d\n", copy_len);
		candidates_taken += copy_len;
		printf("candidates: ");
		print_array(candidate_list, candidates_taken, "%d ");

		if ((i + 1 < K && !visited[(i + 1)*K + j]) && (j == 0 || visited[(i + 1)*K + j - 1])) {
			PriorityTuple2 consider_next = { r[u[i + 1]] + s[v[j]], i + 1, j };
			min_heap.push(consider_next);
			printf("push: %f (%d,%d)\n", r[u[i + 1]] + s[v[j]], i + 1, j);
		}
		if ((j + 1 < K && !visited[i *K + j + 1]) && (i == 0 || visited[(i - 1)*K + j + 1])) {
			PriorityTuple2 consider_next = { r[u[i]] + s[v[j + 1]], i, j + 1 };
			min_heap.push(consider_next);
			printf("push: %f (%d,%d)\n", r[u[i]] + s[v[j + 1]], i, j + 1);
		}
		//printf("---------------------------------------------\n");
	}
	//printf("end multisequence_algorithm_get_candidates loop\n");

	delete[] visited;

	//return candidate_list;
}

int main8() {

	int n_clusters = 4;
	float u_distaces[] = { 0., 1., 10., 2. };
	float v_distaces[] = { 0., 1., 10., 2. };
	int u_nearest_indices[] = { 0, 1, 3, 2 };
	int v_nearest_indices[] = { 0, 1, 3, 2 };
	int list_starts[] = { 0, 0, 0, 0, 0, 1, 1, 2, 2, 4, 4, 4, 4, 6, 8, 8 };
	int lists[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
	int lists_len = 10;



	int* candidates = new int[10];
	multisequence_algorithm_get_candidates<int>(u_distaces, v_distaces, u_nearest_indices, v_nearest_indices, n_clusters, list_starts, lists, lists_len, candidates, 10);
	print_array(candidates, 10, "%d ");
	printf("before delete[] candidates\n");
	delete[] candidates;
	printf("after delete[] candidates\n");
	return 0;
}