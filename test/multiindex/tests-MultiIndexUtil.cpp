//
// Created by dima on 08.04.17.
//

#include "catch.hpp"
#include "../../src/multiindex/MultiIndexUtil.h"

TEST_CASE("run multiIndexUtil", "[multiindex][MultiIndexUtil]") {
    const int n_dims = 3;
    const int dim_size = 128;
    MultiIndexUtil<long long> multiIndexUtil(n_dims, dim_size);

    int multiindex[n_dims] = {1, 0, 33};
    long long flatindex = multiIndexUtil.flat_index(multiindex);
    REQUIRE(flatindex == 33 + 128 * (0 + 128 * (1)));

    int multiindices[3*n_dims]={
            1,0,33,
            2,0,0,
            1,1,1
    };

    long long* flatindices=multiIndexUtil.flat_indices(multiindices, 3);
    REQUIRE(flatindices[0] == 33 + 128 * (0 + 128 * (1)));
    REQUIRE(flatindices[1] == 0 + 128 * (0 + 128 * (2)));
    REQUIRE(flatindices[2] == 1 + 128 * (1 + 128 * (1)));

}

