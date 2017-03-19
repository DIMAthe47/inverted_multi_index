#define _CRT_SECURE_NO_WARNINGS
#include "InvertedMultiIndex.h"
#include "MultiSequenceAlgorithm.h"

template<class T>
T* read_array(char* filepath) {
	FILE *file = fopen(filepath, "rb");
	fseek(file, 0, SEEK_END);
	int filesize = ftell(file);
	fseek(file, 0, SEEK_SET);
	int n_elements = filesize / sizeof(T);
	printf("read %d elements from %s\n", n_elements, filepath);
	T* arr = new T[n_elements];
	fread(arr, sizeof(T), n_elements, file);
	return arr;
}

int main() {
	char* centroid_indices_filepath = "C:\\Users\\Dima\\GoogleDisk\\notebooks\\ci_i32.bin";
	char* X_filepath = "C:\\Users\\Dima\\GoogleDisk\\notebooks\\bd_f32.bin";
	char* X_ids_filepath= "C:\\Users\\Dima\\GoogleDisk\\notebooks\\bdi_i32.bin";
	char* X_centroid_indices_filepath = "C:\\Users\\Dima\\GoogleDisk\\notebooks\\bdci_i32.bin";

	char* cluster_distance_matrix_filepath= "C:\\Users\\Dima\\GoogleDisk\\notebooks\\cd_f32.bin";
	char* cluster_index_matrix_filepath = "C:\\Users\\Dima\\GoogleDisk\\notebooks\\ci_i32.bin";


	float* base_data = read_array<float>(X_filepath);
	int* base_data_ids = read_array<int>(X_ids_filepath);
	int* base_data_centroid_indices = read_array<int>(X_centroid_indices_filepath);
	
	const int n_base_data = 10000;
	InvertedMultiIndex<int>* invertedMultiIndex = build_InvertedMultiIndex<int>(base_data_ids, n_base_data, base_data_centroid_indices, 4, 128);


	float* q_cluster_distance_matrix = read_array<float>(cluster_distance_matrix_filepath);
	int* q_nearest_cluster_index_matrix = read_array<int>(cluster_index_matrix_filepath);

	const int n_candidates = 80;
	const int n_queries = 1000;
	
	
	MultiSequenceAlgorithm<int> multiSequenceAlgorithm(invertedMultiIndex);
	const int n_clusters = 512;

	for (int i = 0; i < n_queries; i++) {
		printf("i: %d\n", i);
		int* candidate_list = new int[n_candidates];
		multiSequenceAlgorithm.find_and_write_candidates(&q_cluster_distance_matrix[i*n_clusters], &q_nearest_cluster_index_matrix[i*n_clusters], candidate_list, n_candidates);
		delete[] candidate_list;
	}


	delete invertedMultiIndex;
	delete[] base_data;
	delete[] base_data_ids;
	delete[] base_data_centroid_indices;
	delete[] q_cluster_distance_matrix;
	delete[] q_nearest_cluster_index_matrix;

}

int main0() {
	printf("%i\n", sizeof(int));
	MultiIndexUtil multiIndexUtil(4, 1024);
	printf("end\n");
	return 0;
}

int main1() {
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

