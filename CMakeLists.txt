cmake_minimum_required(VERSION 3.7)
project(image_processing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_CXX_STANDARD 11)

#include_directories("/opt/OpenBLAS/include")
#include_directories("/home/dima/Downloads/catch")

include_directories("C:/Users/Dima/Downloads/OpenBLAS-v0.2.19-Win64-int32/OpenBLAS-v0.2.19-Win64-int32/include")
include_directories("test/catch")

set(SOURCE_FILES
        src/util/array_utils.h
        src/multiindex/InvertedMultiIndex.h
        src/multiindex/MultiIndexUtil.h
        src/multiindex/MultiSequenceAlgorithm.h
        src/util/PriorityTuple.h
        src/multiindex/InvertedMultiIndexSearcher.h
        src/util/cblas_wraps.h
        src/distance/EuclideanDistanceComputer.h
        src/distance/SubspacesProductEuclideanDistanceComputer.h
        src/search/SubspacesProductNearestIndicesSearcher.h
        src/search/NearestIndicesSearcher.h
        src/multiindex/InvertedMultiIndexBuilder.h src/util/SubspacedVectors.h
        src/util/SubspacedScalars.h
        src/util/SubspacedVector.h src/util/Vectors.h)

add_library(MY_LIB STATIC ${SOURCE_FILES})
set_target_properties(MY_LIB PROPERTIES LINKER_LANGUAGE CXX)

add_executable(${PROJECT_NAME}-test "test/tests-main.cpp"
        "test/distance/tests-EuclideanDistanceComputer.cpp"
        test/distance/tests-SubspacesProductEuclideanDistanceComputer.cpp
        test/search/tests-NearestIndicesSearcher.cpp
        test/search/tests-SubspacesProductNearestIndicesSearcher.cpp
        test/multiindex/tests-InvertedMultiIndexBuilder.cpp
        test/multiindex/tests-MultiSequenceAlgorithm.cpp test/search/tests-InvertedMultiIndexSearcher.cpp)

#target_link_libraries(${PROJECT_NAME}-test "/opt/OpenBLAS/lib/libopenblas.so")
#target_link_libraries(${PROJECT_NAME} "/opt/OpenBLAS/lib/libopenblas.so")

target_link_libraries(${PROJECT_NAME}-test "C:/Users/Dima/Downloads/OpenBLAS-v0.2.19-Win64-int32/OpenBLAS-v0.2.19-Win64-int32/bin/libopenblas.dll")
#target_link_libraries(${PROJECT_NAME}  MY_LIB "C:/Users/Dima/Downloads/OpenBLAS-v0.2.19-Win64-int32/OpenBLAS-v0.2.19-Win64-int32/bin/libopenblas.dll")