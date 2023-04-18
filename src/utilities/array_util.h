#ifndef ARRAY_UTIL_H_INCLUDED
#define ARRAY_UTIL_H_INCLUDED

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Create and Delete Functions
int* array_generate_sequence(int val_begin, int val_inc, int len_array);
int* array_generate_sequence_shuffled(int val_begin, int val_inc, int len_array);

// Search Functions (Linear)
int array_linear_search(int* array, int len_array, int val_target);
int array_linear_search_range(int* array, int len_array, int idx_begin, int idx_end, int val_target);
int array_linear_search_or_closest(int* array, int len_array, int val_target);
int array_linear_search_range_or_closest(int* array, int len_array, int idx_begin, int idx_end, int val_target);

// Search Functions (Binary)
int array_binary_search(int* array, int len_array, int val_target);
int array_binary_search_range(int* array, int len_array, int idx_begin, int idx_end, int val_target);
int array_binary_search_or_closest(int* array, int len_array, int val_target);
int array_binary_search_range_or_closest(int* array, int len_array, int idx_begin, int idx_end, int val_target);

// Maniuplation Functions
void array_clear(int* array, int len_array);
int array_shift_out(int** ptr_array, int len_array, int len_shift, int idx_begin_read, bool can_resize_array);
void array_swap(int* array, int len_array, int index_1, int index_2);
int* array_shuffle(int* array, int len_array, bool use_original_array);
void array_resize(int** ptr_array, int len_array, int len_new_array);
void array_parallel_sort_2(int* array_1, int* array_2, int len_array_1, int len_array_2, bool is_ascending);
void array_parallel_sort_3(int* array_1, int* array_2, int* array_3, int len_array_1, int len_array_2, int len_array_3, bool is_ascending);

// Utility Functions
bool array_is_equal(int* array_1, int* array_2, int len_array_1, int len_array_2);
void array_print(int* array, int len_array, bool should_print_newline);
void array_bool_print(bool* array, int len_array, bool should_print_newline);

#endif