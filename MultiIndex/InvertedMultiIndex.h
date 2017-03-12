#pragma once


template<class T>
struct InvertedMultiIndex {
	T* entries;
	int entries_len;
	int* entries_list_starts;
	int n_subquantizers;
	int n_clusters;

	InvertedMultiIndex(T* entries, int* entries_list_starts, int n_subquantizers, int n_clusters) :
		entries(entries), entries_list_starts(entries_list_starts), n_subquantizers(n_subquantizers), n_clusters(n_clusters)
	{
		int n_total_cells = 1;
		for (int i = 0; i < n_subquantizers; i++) {
			n_total_cells *= n_clusters;
		}
		//last element must be len(entries)
		entries_len = entries_list_starts[n_total_cells];
	}

	~InvertedMultiIndex() {
		delete[] entries;
		delete[] entries_list_starts;
	}
};