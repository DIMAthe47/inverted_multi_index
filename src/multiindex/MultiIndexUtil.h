#ifndef MULTIINDEXUTIL_H
#define MULTIINDEXUTIL_H

template<typename T>
class MultiIndexUtil {
private:
    const int n_dims;
    const int dim_size;
    T *flatindex_multipliers;

public:
    MultiIndexUtil(const int n_dims, const int dim_size) : n_dims(n_dims), dim_size(dim_size) {
        flatindex_multipliers = new T[n_dims];
        flatindex_multipliers[n_dims - 1] = 1;
        for (int dim = n_dims - 2; dim >= 0; dim--) {
            flatindex_multipliers[dim] = flatindex_multipliers[dim + 1] * dim_size;
        }
    }

    T flat_index(const int *multi_index) {
        T flatindex = 0;
        for (int dim = 0; dim < n_dims; dim++) {
            flatindex += multi_index[dim] * flatindex_multipliers[dim];
        }
        return flatindex;
    }

    void flat_indices(const int *multi_indices, int multi_indices_len, T* flatindices_out) {
        for (int i = 0; i < multi_indices_len; i++) {
            T flatindex = 0;
            for (int dim = 0; dim < n_dims; dim++) {
                flatindex += multi_indices[i * n_dims + dim] * flatindex_multipliers[dim];
            }
            flatindices_out[i] = flatindex;
        }
    }

    T flat_dim_offset(const int singular_dim) {
        T dim_offset = flatindex_multipliers[singular_dim];
        T flatindex = dim_offset;
        return flatindex;
    }

    int total_elements() {
        int total_elements_ = 1;
        for (int i = 0; i < n_dims; i++) {
            total_elements_ *= dim_size;
        }
        return total_elements_;
    }

    ~MultiIndexUtil() {
        delete[] flatindex_multipliers;
    }
};

#endif