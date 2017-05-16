#ifndef MULTIINDEXUTIL2_H
#define MULTIINDEXUTIL2_H

class MultiIndexUtil2 {
private:
    const int n_dims;
    const int dim_size;
    int *flatindex_multipliers;
    int* multi_index;

public:
    MultiIndexUtil2(const int n_dims, const int dim_size) : n_dims(n_dims), dim_size(dim_size) {
        flatindex_multipliers = new int[n_dims];
        flatindex_multipliers[n_dims - 1] = 1;
        for (int dim = n_dims - 2; dim >= 0; dim--) {
            flatindex_multipliers[dim] = flatindex_multipliers[dim + 1] * dim_size;
        }

        multi_index=new int[n_dims];
    }

    int flat_index() {
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

    int total_elements() {
        int total_elements_ = 1;
        for (int i = 0; i < n_dims; i++) {
            total_elements_ *= dim_size;
        }
        return total_elements_;
    }

    int* multi_index_ptr(){
        return multi_index;
    }

    ~MultiIndexUtil2() {
        delete[] flatindex_multipliers;
        delete[] multi_index;
    }
};

#endif