#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "MultiIndexUtil.h"
#include "InvertedMultiIndex.h"


template<class T>
class InvertedMultiIndexBuilder {
public:
	/*InvertedMultiIndexBuilder(T* out_entires, int* out_entries_list_starts) :entires(out_entires), entries_list_starts(out_entries_list_starts) {
	}*/

	InvertedMultiIndex<T>* build(T* x, int x_len, int* X_centroid_indices, int n_subquantizers, int n_clusters) {
		MultiIndexUtil multiIndexUtil(n_subquantizers, n_clusters);
		int m = n_subquantizers;
		int* _multi_index = new int[m];

		int n_total_cells = 1;
		for (int i = 0; i < n_subquantizers; i++) {
			n_total_cells *= n_clusters;
		}

		int* counts = new int[n_total_cells + 1];//+1 border cell
		memset(counts, 0, sizeof(int)*n_total_cells);

		for (int i = 0; i < x_len; i++) {
			int row_offset = i*n_subquantizers;
			copy_array(&X_centroid_indices[row_offset], _multi_index, n_subquantizers);
			int flatindex = multiIndexUtil.flat_index(_multi_index);
			counts[flatindex]++;
		}
		print_array(counts, n_total_cells, "%d ");


		for (int i = 1; i < n_total_cells; i++) {
			counts[i] = counts[i - 1] + counts[i];
		}
		print_array(counts, n_total_cells, "%d ");

		int* entries_list_ends = counts;
		T* entries = new T[x_len];
		for (int i = 0; i < x_len; i++) {
			int row_offset = i*n_subquantizers;
			copy_array(&X_centroid_indices[row_offset], _multi_index, n_subquantizers);
			int flatindex = multiIndexUtil.flat_index(_multi_index);
			entries_list_ends[flatindex]--;

			entries[entries_list_ends[flatindex]] = x[i];
		}
		int* entries_list_starts = entries_list_ends;
		entries_list_starts[n_total_cells] = x_len;

		print_array(entries, x_len, "%d ", false);

		delete[] _multi_index;

		InvertedMultiIndex<T>* invertedMultiIndex = new InvertedMultiIndex<T>(entries, entries_list_starts, n_subquantizers, n_clusters);
		return invertedMultiIndex;
	}
};


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
	InvertedMultiIndexBuilder<int> invertedMultiIndexBuilder;
	InvertedMultiIndex<int>* invertedMultiIndex = invertedMultiIndexBuilder.build(x, x_len, X_centroid_indices, 3, 3);
	//InvertedMultiIndex<int> invertedMultiIndex= invertedMultiIndexBuilder.bui invertedMultiIndexBuilder..bu<int>().b	(x, x_len, X_centroid_indices, 3, 3);

	delete invertedMultiIndex;
	return 0;
}