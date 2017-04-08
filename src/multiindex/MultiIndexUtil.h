#ifndef MULTIINDEXUTIL_H
#define MULTIINDEXUTIL_H

class MultiIndexUtil {
private:
    const int n_dims;
    const int dim_size;
    int *flatindex_multipliers;

public:
    MultiIndexUtil(const int n_dims, const int dim_size) : n_dims(n_dims), dim_size(dim_size) {
        flatindex_multipliers = new int[n_dims];
        flatindex_multipliers[n_dims - 1] = 1;
        for (int dim = n_dims - 2; dim >= 0; dim--) {
            flatindex_multipliers[dim] = flatindex_multipliers[dim + 1] * dim_size;
        }
    }

    int flat_index(const int *multi_index) {
        int flatindex = 0;
        for (int dim = 0; dim < n_dims; dim++) {
            flatindex += multi_index[dim] * flatindex_multipliers[dim];
        }
        return flatindex;
    }

    int flat_dim_offset(const int singular_dim) {
        int dim_offset = flatindex_multipliers[singular_dim];
        int flatindex = dim_offset;
        return flatindex;
    }

    ~MultiIndexUtil() {
        delete[] flatindex_multipliers;
    }
};

#endif