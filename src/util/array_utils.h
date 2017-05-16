#ifndef COMMON_H
#define COMMON_H

#include <cstdlib>
#include <utility>
#include <vector>
#include <stdio.h>


template<typename T>
void print_matrix(T *matrix, int width, int height, char *format) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf(format, matrix[i * width + j]);
        }
        printf("\n");
    }
    printf("\n");
}

template<typename T>
void print_matrix(T **matrix, int width, int height, char *format) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf(format, matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

template<typename T>
void print_array(T *arr, int width, char const *format, bool print_index = true) {
    for (int i = 0; i < width; i++) {
        if (print_index)
            printf("%d:", i);
        printf(format, arr[i]);
    }
    printf("\n");
}

template<typename T>
T sum_array(const T *arr, int width) {
    T sum = 0;
    for (int i = 0; i < width; i++) {
        sum += arr[i];
    }
    return sum;
}

template<typename T>
T prod_array(const T *arr, int width) {
    T prod = 1;
    for (int i = 0; i < width; i++) {
        prod = prod * arr[i];
    }
    return prod;
}

template<typename T>
int copy_array(const T *from, T *to, const std::pair<int, int> slice_from, const int start_to, const int len_to) {
    int from_len = slice_from.second - slice_from.first;
    int copy_len = start_to + from_len < len_to ? from_len : len_to - start_to;

    int p_from = slice_from.first;
    int p_to = start_to;
    int save_copy_len = copy_len;
    while (copy_len > 0) {
        to[p_to] = from[p_from];
        p_from++;
        p_to++;
        copy_len--;
    }

    return save_copy_len;
}

template<typename T>
int copy_array(const T *from, T *to, const int len) {
    for (int i = 0; i < len; i++) {
        to[i] = from[i];
    }
    return 0;
}

template<typename T>
void take_in_rows(const T *matrix, int matrix_width, const int *columns, T *target_array, const int target_array_len) {
    for (int i = 0; i < target_array_len; i++) {
        int column = columns[i];
        target_array[i] = matrix[i * matrix_width + column];
    }
}

template<typename T>
std::vector<T> array_to_vector(T *arr, const int n_element) {
    std::vector<T> vec(arr,
                       arr + n_element);
    return vec;
}


template<typename T>
std::vector<T> array_to_vector(const T *arr, const int n_element) {
    std::vector<T> vec(arr,
                       arr + n_element);
    return vec;
}

template<typename T>
void c_delete(T *c_arr) {
    delete[] c_arr;
}

template<typename T>
void c_delete(const T *c_arr) {
    delete[] c_arr;
}

#endif