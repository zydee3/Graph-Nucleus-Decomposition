#include "array_util.h"

// Begin Locale Helper Functions

int cmp_ints_asc(const void* a, const void* b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;

    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int cmp_ints_dsc(const void* a, const void* b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;

    if (arg1 > arg2) return -1;
    if (arg1 < arg2) return 1;
    return 0;
}

/**
 * @brief Uses linear search to find the index of the target element.
 *
 * If the target element is not found and param should_return_closest
 * is true, index where the element would be appended to the array is
 * returned. In other words, since the array is assumed to be
 * unordered, and the element does not exist, if it were to be
 * inserted, it would be appended.
 *
 * If the target element is not found and param should_return_closest
 * is false, -1 is returned.
 *
 * @attention This function assumes the array is unsorted. If the
 * array is sorted, use array_binary_search() instead.
 *
 * @param array The array to search for param target.
 * @param len_array The length of the array.
 * @param idx_lower_bound The inclusive lower bound of the search.
 * @param idx_upper_bound The inclusive upper bound of the search.
 * @param val_target The target value to search for.
 * @param should_return_closest True if the index of the closest
 * element should be returned if the target element is not found,
 * false otherwise.
 * @return int The index of the target element if it exists, the index
 * of the closest element if it does not exist and param
 * should_return_closest is true, or -1 if the target element does not
 * exist and param should_return_closest is false.
 */
static inline int _linear_search(int* array, int len_array, int idx_lower_bound, int idx_upper_bound, int val_target, bool should_return_closest) {
    assert(array != NULL);
    assert(len_array > 0);
    assert(idx_lower_bound >= 0);
    assert(idx_upper_bound < len_array);
    assert(idx_lower_bound <= idx_upper_bound);

    for (int i = idx_lower_bound; i <= idx_upper_bound; i++) {
        if (array[i] == val_target) {
            return i;
        }
    }

    return should_return_closest ? len_array : -1;
}

/**
 * @brief Uses binary search to find the index of the target element.
 *
 * If the target element is not found and param should_return_closest
 * is true, the index of the closest element is returned. In other
 * words, the index of where the element would be if it did exist in
 * the array is returned.
 *
 * If the target element is not found and param should_return_closest
 * is false, -1 is returned.
 *
 * @attention This function assumes the array is sorted. If the
 * array is unsorted, use array_linear_search() instead.
 *
 * @param array The array to search for param target.
 * @param len_array The length of the array.
 * @param idx_lower_bound The inclusive lower bound of the search.
 * @param idx_upper_bound The inclusive upper bound of the search.
 * @param val_target The target value to search for.
 * @param should_return_closest True if the index of the closest
 * element should be returned if the target element is not found,
 * false otherwise.
 * @return int The index of the target element if it exists, the index
 * of the closest element if it does not exist and param
 * should_return_closest is true, or -1 if the target element does not
 * exist and param should_return_closest is false.
 */
static inline int _binary_search(int* array, int len_array, int idx_lower_bound, int idx_upper_bound, int val_target, bool should_return_closest) {
    assert(array != NULL);
    assert(idx_lower_bound >= 0);

    if (len_array == 0) {
        return should_return_closest ? 0 : -1;
    }

    if (idx_upper_bound >= len_array) {
        // printf("idx_upper_bound: %d\n", idx_upper_bound);
        // printf("len_array: %d\n", len_array);
        // printf("target: %d\n", val_target);
        return -1;
    }

    assert(idx_upper_bound < len_array);

    while (idx_lower_bound <= idx_upper_bound) {
        int mid_idx = (idx_lower_bound + idx_upper_bound) / 2;

        if (array[mid_idx] == val_target) {
            return mid_idx;
        }

        if (array[mid_idx] < val_target) {
            idx_lower_bound = mid_idx + 1;
        } else {
            idx_upper_bound = mid_idx - 1;
        }
    }

    return should_return_closest ? idx_lower_bound : -1;
}

/**
 * @brief Shifts the elements in the array to the left by len_shift
 * places starting at idx_begin_read. This is an outwards shift as
 * elements are moving away from the center of the array.
 *
 * The bytes shifted are those inclusively between idx_begin_read and
 * the beginning of the array.
 *
 * If the array cannot be resized, then len_shift elements are lost
 * and only the elements that can be shifted are shifted. In other
 * words, no writing is performed outside the bounds of the array. If
 * the array can be resized, then the array is resized to fit the
 * shifted elements. In other words, len_shift number of zeros are
 * essentially insertted between idx_begin_read-1 and idx_begin_read.
 * Memcpy and memmove are used to perform the byte shifts.
 *
 * @param ptr_array Double pointer to the array of integers to shift.
 * @param len_array Length of param ptr_array.
 * @param len_shift Length of the shift.
 * @param idx_begin_read Index to begin shifting at.
 * @param can_resize_array Bool indicating whether the array can be
 * resized. True if the array can be resized, false otherwise.
 * @return int The new length of the shifted array. The returned
 * length will be the same as the original length unless the array
 * can be resized and the array is resized.
 */
static inline int _shift_out_left(int** ptr_array, int len_array, int len_shift, int idx_begin_read, bool can_resize_array) {
    assert(ptr_array != NULL);
    assert(*ptr_array != NULL);
    assert(len_array > 0);
    assert(idx_begin_read >= 0);
    assert(idx_begin_read < len_array);

    // Shifting left means that the bytes at the front of the array
    // are always lost unless the array can be resized. If the array
    // can be resized, then len_shift elements are removed from the
    // front of the array.
    if (can_resize_array) {
        int* new_array = (int*)malloc((len_array - len_shift) * sizeof(int));
        assert(new_array != NULL);

        int new_len = len_array - len_shift;

        // Copy the bytes from the array to the new array.
        memcpy(new_array, &(*ptr_array)[len_shift], new_len * sizeof(int));

        // Free the old array.
        free(*ptr_array);

        // Set the pointer to the new array.
        *ptr_array = new_array;

        // The new length of the array is the old length minus the
        // number of bytes shifted.
        return new_len;
    }

    // The number of bytes lost is simply the number of bytes shifted.
    int num_bytes_lost = len_shift;

    // The number of bytes shifted is the number of bytes between the
    // beginning of the array and index being shifted.
    int num_bytes_shifted = idx_begin_read - num_bytes_lost + 1;

    // Shifting left means we will always begin to write at the front
    int idx_begin_write = 0;

    // We are writing num_bytes_shifted bytes starting at the front.
    // we are reading num_bytes_shifted bytes starting at len_shift.
    int idx_end_write = num_bytes_shifted - 1;

    // The first byte that is not lost is at index len_shift.
    // Consider for example len_shift=2. The first byte that is not
    // lost is at index 2 since the bytes at index 0 and 1 are lost
    // when shifting left by len_shift.
    idx_begin_read = len_shift;

    // printf("\n\nSHIFT LEFT\n\nidx_begin_read: %d\nidx_begin_write: %d\nidx_end_write: %d\nnum_bytes_shifted: %d\nnum_bytes_lost: %d\n\n\n", idx_begin_read, idx_begin_write, idx_end_write, num_bytes_shifted, num_bytes_lost);

    // We've computed everything we need. Move the bytes.
    memmove(&(*ptr_array)[idx_begin_write], &(*ptr_array)[idx_begin_read], num_bytes_shifted * sizeof(int));

    // Zero out the bytes where elements used to be before the shift.
    memset(&(*ptr_array)[idx_end_write + 1], 0, num_bytes_lost * sizeof(int));

    return len_array;
}

/**
 * @brief Shifts the elements in the array to the right by len_shift
 * places starting at idx_begin_read. This is an outwards shift as
 * elements are moving away from the center of the array.
 *
 * The bytes shifted are those inclusively between idx_begin_read and
 * the end of the array.
 *
 * If the array cannot be resized, then len_shift elements are lost
 * and only the elements that can be shifted are shifted. In other
 * words, no writing is performed outside the bounds of the array. If
 * the array can be resized, then the array is resized to fit the
 * shifted elements. In other words, len_shift number of zeros are
 * essentially insertted between idx_begin_read-1 and idx_begin_read.
 * Memcpy and memmove are used to perform the byte shifts.
 *
 * @param ptr_array Double pointer to the array of integers to shift.
 * @param len_array Length of param ptr_array.
 * @param len_shift Length of the shift.
 * @param idx_begin_read Index to begin shifting at.
 * @param can_resize_array Bool indicating whether the array can be
 * resized. True if the array can be resized, false otherwise.
 * @return int The new length of the shifted array. The returned
 * length will be the same as the original length unless the array
 * can be resized and the array is resized.
 */
static inline int _shift_out_right(int** ptr_array, int len_array, int len_shift, int idx_begin_read, bool can_resize_array) {
    assert(ptr_array != NULL);
    assert(*ptr_array != NULL);
    assert(len_array > 0);
    assert(idx_begin_read >= 0);
    assert(idx_begin_read < len_array);

    // Shifting right means that the bytes at the end of the array
    // are always lost unless the array can be resized. If the array
    // can be resized, then len_shift zeros are added between the
    // element at idx_begin_read and idx_begin_read + 1. This will
    // offset the element at idx_begin_read+1 by len_shift.

    if (can_resize_array) {
        int new_len = len_array + len_shift;

        int* new_array = malloc(new_len * sizeof(int));
        assert(new_array != NULL);

        memset(new_array, 0, new_len * sizeof(int));

        // Copy the bytes from 0 to idx_begin_read
        memcpy(new_array, *ptr_array, idx_begin_read * sizeof(int));

        // Copy the bytes from idx_begin_read + 1 to the end of the
        // array to the new array starting at the offset.
        // (idx_begin_read + len_shift)
        memcpy(&new_array[idx_begin_read + len_shift + 1], &(*ptr_array)[idx_begin_read + 1], (len_array - idx_begin_read - 1) * sizeof(int));

        // Free the old array.
        free(*ptr_array);

        // Set the pointer to the new array.
        *ptr_array = new_array;

        // The new length of the array is the old length plus the
        // number of bytes shifted.return new_len;
        return new_len;
    }

    // The number of bytes lost is simply the number of bytes shifted.
    int num_bytes_lost = len_shift;

    // The number of bytes shifted is the number of bytes between the
    // index that was shifted and the end of the array. We subtract
    // the number of bytes lost as those are just discarded.
    int num_bytes_shifted = len_array - idx_begin_read - num_bytes_lost;

    // Calculate where to wirte by using len_shift as an offset
    int idx_begin_write = idx_begin_read + len_shift;

    // printf("\n\nSHIFT RIGHT\n\nidx_begin_read: %d\nidx_begin_write: %d\nidx_end_write: %d\nnum_bytes_shifted: %d\nnum_bytes_lost: %d\n\n\n", idx_begin_read, idx_begin_write, idx_end_write, num_bytes_shifted, num_bytes_lost);

    memmove(&(*ptr_array)[idx_begin_write], &(*ptr_array)[idx_begin_read], num_bytes_shifted * sizeof(int));

    memset(&(*ptr_array)[idx_begin_read], 0, num_bytes_lost * sizeof(int));

    return len_array;
}

static inline void _parallel_sort(int* array_1, int* array_2, int idx_lower_bound, int idx_upper_bound, bool (*cmp)(int, int)) {
    int idx_curr_lower_bound = idx_lower_bound;
    int idx_curr_upper_bound = idx_upper_bound;
    int idx_curr_middle = (idx_curr_lower_bound + idx_curr_upper_bound) / 2;
    int pivot = array_1[idx_curr_middle];  // change from array_2 to array_1

    while (idx_curr_lower_bound <= idx_curr_upper_bound) {
        while (cmp(array_1[idx_curr_lower_bound], pivot) == true) {  // change from array_2 to array_1
            idx_curr_lower_bound++;
        }

        while (array_1[idx_curr_upper_bound] != pivot && cmp(array_1[idx_curr_upper_bound], pivot) == false) {  // change from array_2 to array_1
            idx_curr_upper_bound--;
        }

        if (idx_curr_lower_bound <= idx_curr_upper_bound) {
            int temp = array_1[idx_curr_lower_bound];

            array_1[idx_curr_lower_bound] = array_1[idx_curr_upper_bound];
            array_1[idx_curr_upper_bound] = temp;

            temp = array_2[idx_curr_lower_bound];                           // change from array_1 to array_2
            array_2[idx_curr_lower_bound] = array_2[idx_curr_upper_bound];  // change from array_1 to array_2
            array_2[idx_curr_upper_bound] = temp;                           // change from array_1 to array_2

            idx_curr_lower_bound++;
            idx_curr_upper_bound--;
        }
    }

    if (idx_lower_bound < idx_curr_upper_bound) {
        _parallel_sort(array_1, array_2, idx_lower_bound, idx_curr_upper_bound, cmp);
    }

    if (idx_curr_lower_bound < idx_upper_bound) {
        _parallel_sort(array_1, array_2, idx_curr_lower_bound, idx_upper_bound, cmp);
    }
}

// End Locale Helper Functions
// Begin Public Functions
// Begin Create and Delete Functions

/**
 * @brief Creates a new array with param len_array number of elements
 * starting at val_begin and incrementing by val_inc.
 *
 * The array is allocated using calloc and the elements are set using
 * a for loop. The array is returned.
 *
 * @param val_begin The value of the first element in the array.
 * @param val_inc The value to increment by.
 * @param len_array The length of the array.
 * @return int* The new array.
 */
int* array_generate_sequence(int val_begin, int val_inc, int len_array) {
    assert(len_array > 0);

    int* array = calloc(len_array, sizeof(int));
    assert(array != NULL);

    for (int i = 0; i < len_array; i++) {
        array[i] = val_begin + (i * val_inc);
    }

    return array;
}

/**
 * @brief Creates a new shuffled array with param len_array number of
 * elements starting at val_begin and incrementing by val_inc.
 *
 * The initial sequence is generated using array_generate_sequence.
 * The array is then shuffled using array_shuffle. The shuffled array
 * is returned.
 *
 * @param val_begin The value of the first element in the array.
 * @param val_inc The value to increment by.
 * @param len_array The length of the array.
 * @return int* The new shuffled array.
 */
int* array_generate_sequence_shuffled(int val_begin, int val_inc, int len_array) {
    assert(len_array > 0);

    int* array = array_generate_sequence(val_begin, val_inc, len_array);
    assert(array != NULL);

    return array_shuffle(array, len_array, true);
}

// End Create and Delete Functions
// Begin Search Functions (Linear)

int array_linear_search(int* array, int len_array, int val_target) {
    return _linear_search(array, len_array, 0, len_array - 1, val_target, false);
}

int array_linear_search_range(int* array, int len_array, int idx_begin, int idx_end, int val_target) {
    return _linear_search(array, len_array, idx_begin, idx_end, val_target, false);
}

int array_linear_search_or_closest(int* array, int len_array, int val_target) {
    return _linear_search(array, len_array, 0, len_array - 1, val_target, true);
}

int array_linear_search_range_or_closest(int* array, int len_array, int idx_begin, int idx_end, int val_target) {
    return _linear_search(array, len_array, idx_begin, idx_end, val_target, true);
}

// End Search Functions (Linear)
// Begin Search Functions (Binary)

int array_binary_search(int* array, int len_array, int val_target) {
    return _binary_search(array, len_array, 0, len_array - 1, val_target, false);
}

int array_binary_search_range(int* array, int len_array, int idx_begin, int idx_end, int val_target) {
    return _binary_search(array, len_array, idx_begin, idx_end, val_target, false);
}

int array_binary_search_or_closest(int* array, int len_array, int val_target) {
    return _binary_search(array, len_array, 0, len_array - 1, val_target, true);
}

int array_binary_search_range_or_closest(int* array, int len_array, int idx_begin, int idx_end, int val_target) {
    return _binary_search(array, len_array, idx_begin, idx_end, val_target, true);
}

/**
 * @brief Reverses the elements in an array between the specified
 * indices idx_begin_read and idx_end_read (inclusive).
 *
 * @param array The array to reverse the elements in within the range.
 * @param array_size The size of the array.
 * @param idx_begin_read The position to begin reverseing.
 * @param idx_end_read The position to stop reversing.
 */
void array_reverse(int* array, int array_size, int idx_begin_read, int idx_end_read) {
    assert(array != NULL);
    assert(array_size >= 0);
    assert(idx_begin_read >= 0 && idx_begin_read < array_size);
    assert(idx_end_read >= 0 && idx_end_read < array_size);
    assert(idx_begin_read <= idx_end_read);

    int left_idx = idx_begin_read;
    int right_idx = idx_end_read;

    while (left_idx < right_idx) {
        int temp = array[left_idx];
        array[left_idx] = array[right_idx];
        array[right_idx] = temp;

        left_idx++;
        right_idx--;
    }
}

/**
 * @brief Shuffles an array using the Fisher-Yates algorithm.
 *
 * A copy of the param array is made if use_original_array is false
 * using calloc and memcpy. The array is then shuffeled using the
 * Fisher-Yates algorithm. The algorithm is implemented using a loop
 * that iterates backwards over the array. The shuffled array is
 * returned.
 *
 * @note The Fisher-Yates algorithm produces a non-identity
 * permutation with high probability. The probability of obtaining the
 * identity permutation is P(identity) = 1/n!.
 *
 * @param array The array to shuffle.
 * @param len_array The length of the array.
 * @param use_original_array True if the original array should be
 * used, false if a copy should be made.
 * @return int* A pointer to the shuffled array.
 */
int* array_shuffle(int* array, int len_array, bool use_original_array) {
    assert(array != NULL);
    assert(len_array >= 0);

    int* shuffled_array = NULL;

    if (use_original_array) {
        shuffled_array = array;
    } else {
        shuffled_array = calloc(len_array, sizeof(int));
        memcpy(shuffled_array, array, len_array * sizeof(int));
    }

    assert(shuffled_array != NULL);
    srand(time(0));

    for (int i = len_array - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int temp = shuffled_array[i];
        shuffled_array[i] = shuffled_array[j];
        shuffled_array[j] = temp;
    }

    return shuffled_array;
}

/**
 * @brief Sorts param array_1 based on the sorting of param array_2.
 * Param array_2 is sorted in ascending order if param is_ascending is
 * true, otherwise param array_2 is sorted in descending order.
 *
 * Example
 * A = [1,22,34,43,51]  => [43,34,22,51,1]
 * B = [5,3,2,1,4]      => [1,2,3,4,5]
 *
 * @param array_1 The array to sort relative to array_2.
 * @param array_2 The array to sort.
 * @param idx_lower_bound The lower bound of the array to sort.
 * @param idx_upper_bound The upper bound of the array to sort.
 * @param is_ascending True if param array_2 should be sorted in
 * ascending order, false if param array_2 should be sorted in
 * descending order.
 */
void array_parallel_sort_2(int* array_1, int* array_2, int len_array_1, int len_array_2, bool is_ascending) {
    assert(array_1 != NULL);
    assert(array_2 != NULL);
    assert(len_array_1 == len_array_2);
    assert(len_array_1 >= 0);

    if (is_ascending) {
        _parallel_sort(array_1, array_2, 0, len_array_1 - 1, cmp_min);
    } else {
        _parallel_sort(array_1, array_2, 0, len_array_1 - 1, cmp_max);
    }

    for (int i = 0; i < len_array_1; i++) {
        int lower_bound = i;
        int upper_bound = i;

        for (int j = i + 1; j < len_array_1; j++) {
            if (array_1[j] != array_1[i]) {
                upper_bound = j;
                i = j - 1;
                break;
            }
        }

        qsort(&array_2[lower_bound], upper_bound - lower_bound, sizeof(int), cmp_qsort_min);
    }
}

int array_filtered_argmin(int* array, int len_array, bool* idx_filtered) {
    assert(array != NULL);
    assert(len_array > 0);

    bool is_min_set = false;
    int min_idx = 0;

    for (int i = 0; i < len_array; i++) {
        if (idx_filtered[i] == true) {
            continue;
        }

        if (is_min_set == false) {
            min_idx = i;
            is_min_set = true;
        } else if (array[i] < array[min_idx]) {
            min_idx = i;
        }
    }

    assert(idx_filtered[min_idx] == false);

    return min_idx;
}

int array_count_symmetric_difference(int* array_1, int* array_2, int len_array_1, int len_array_2) {
    assert(array_1 != NULL);
    assert(array_2 != NULL);
    assert(len_array_1 > 0);
    assert(len_array_2 > 0);

    int num_differences = 0;

    int idx_array_1 = 0;
    int idx_array_2 = 0;

    while (idx_array_1 < len_array_1 && idx_array_2 < len_array_2) {
        if (array_1[idx_array_1] < array_2[idx_array_2]) {
            num_differences++;
            idx_array_1++;
        } else if (array_1[idx_array_1] > array_2[idx_array_2]) {
            num_differences++;
            idx_array_2++;
        } else {
            idx_array_1++;
            idx_array_2++;
        }
    }

    num_differences += len_array_1 - idx_array_1;
    num_differences += len_array_2 - idx_array_2;

    return num_differences;
}

void array_union(int* array_1, int* array_2, int len_array_1, int len_array_2, int** ptr_array_union, int* len_array_union) {
    assert(array_1 != NULL);
    assert(array_2 != NULL);

    int* array_union = malloc((len_array_1 + len_array_2) * sizeof(int));

    int idx_array_1 = 0;
    int idx_array_2 = 0;
    int idx_union = 0;

    while (idx_array_1 < len_array_1 && idx_array_2 < len_array_2) {
        if (array_1[idx_array_1] < array_2[idx_array_2]) {
            array_union[idx_union] = array_1[idx_array_1];
            idx_array_1++;
        } else if (array_1[idx_array_1] > array_2[idx_array_2]) {
            array_union[idx_union] = array_2[idx_array_2];
            idx_array_2++;
        } else {
            array_union[idx_union] = array_1[idx_array_1];
            idx_array_1++;
            idx_array_2++;
        }

        idx_union++;
    }

    while (idx_array_1 < len_array_1) {
        array_union[idx_union] = array_1[idx_array_1];
        idx_array_1++;
        idx_union++;
    }

    while (idx_array_2 < len_array_2) {
        array_union[idx_union] = array_2[idx_array_2];
        idx_array_2++;
        idx_union++;
    }

    array_union = realloc(array_union, idx_union * sizeof(int));
    assert(array_union != NULL);

    *ptr_array_union = array_union;
    *len_array_union = idx_union;
}

// End Manipulation Functions
// Begin Utility Functions

/**
 * @brief Compares two arrays for equality.
 *
 * To be equal, both arrays must have the same size and the same
 * elements in the same order.
 *
 * @param array_1 The first array to compare.
 * @param array_2 The second array to compare.
 * @param array_1_size The size of the first array.
 * @param array_2_size The size of the second array.
 * @return bool True if the arrays are equal, false otherwise.
 */
bool array_is_equal(int* array_1, int* array_2, int len_array_1, int len_array_2) {
    if (len_array_1 != len_array_2) {
        return false;
    }

    assert(array_1 != NULL);
    assert(array_2 != NULL);
    assert(len_array_1 >= 0);
    assert(len_array_2 >= 0);

    return memcmp(array_1, array_2, len_array_2 * sizeof(int)) == 0;
}

/**
 * @brief Prints an array to stdout.
 *
 * Prints out a comma separated list of the elements in the array
 * without a newline.
 *
 * @param array The array to print.
 * @param array_size The size of the array.
 * @param should_print_newline True if a newline should be printed
 * at the end of all print statements, false otherwise.
 */
void array_print(int* array, int len_array, bool should_print_newline) {
    for (int i = 0; i < len_array; i++) {
        printf("%d", array[i]);
        if (i < len_array - 1) {
            printf(", ");
        }
    }

    if (should_print_newline) {
        printf("\n");
    }
}

void array_bool_print(bool* array, int len_array, bool should_print_newline) {
    for (int i = 0; i < len_array; i++) {
        printf("%d", array[i]);
        if (i < len_array - 1) {
            printf(", ");
        }
    }

    if (should_print_newline) {
        printf("\n");
    }
}

// End Utility Functions
// End Public Functions