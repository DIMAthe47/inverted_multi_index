#include "InvertedMultiIndex.h"
#include "MultiSequenceAlgorithm.h"


int main() {
	int x[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
	int x_len = 10;
	int X_centroid_indices[] = {
		2,1,0,
		1,1,0,
		1,2,0,
		0,1,2,
		0,0,0,
		0,0,0,
		1,1,1,
		1,1,1,
		2,2,2,
		2,2,2,
	};
	InvertedMultiIndex<int>* invertedMultiIndex = build_InvertedMultiIndex(x, x_len, X_centroid_indices, 3, 3);
	MultiSequenceAlgorithm<int> multiSequenceAlgorithm(invertedMultiIndex);



	delete invertedMultiIndex;

	return 0;
}

int main2() {

	const float* cluster_distance_matrix = new const float[6]{ 2.0, 0., 2.0, 0., 2.0, 0., };
	const int* nearest_cluster_index_matrix = new const int[6]{ 1, 0,   1, 0,  1, 0, };
	int n_subquantizers = 3;
	int n_clusters = 2;

	//буфферный элемент в конце
	int list_start_ndarray[] = { 0, 1, 2, 2, 4, 4, 6, 8,    10 };
	int lists[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };

	InvertedMultiIndex<int>* invertedMultiIndex = new InvertedMultiIndex<int>(lists, list_start_ndarray, n_subquantizers, n_clusters);

	MultiSequenceAlgorithm<int> msa(invertedMultiIndex);

	int* candidates = new int[10];
	msa.find_and_write_candidates(cluster_distance_matrix, nearest_cluster_index_matrix, candidates, 10);
	print_array(candidates, 10, "%d ");
	delete[] candidates;
	return 0;
}

