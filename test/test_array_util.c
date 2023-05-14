#include "test_array_util.h"

// Helper Functions

// Begin Test Functions
// Begin Array Generation Unit Tests

/**
 * @brief A unit test for array_generate_sequence.
 *
 * The function generates two sequences and compares each of them with
 * the expected results.
 *
 * The test is considered passing if each array is equal to their
 * expected result.
 */
void test_array_generate_sequence() {
    bool is_passing = true;

    int* test_1 = array_generate_sequence(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);
    int answer_1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    int* test_2 = array_generate_sequence(0, 8, NUM_ELEMS);
    int answer_2[] = {0, 8, 16, 24, 32, 40, 48, 56, 64, 72};

    is_passing = is_passing && array_is_equal(test_1, answer_1, NUM_ELEMS, NUM_ELEMS);
    is_passing = is_passing && array_is_equal(test_2, answer_2, NUM_ELEMS, NUM_ELEMS);

    free(test_1);
    free(test_2);

    print_test_result(__FILE__, __func__, is_passing);
}

/**
 * @brief A unit test for array_generate_sequence_shuffled.
 *
 * The function generates an sequence A which is initially ordered.
 * The function then generates an unordered sequence B by shuffling A.
 *
 * The test is considered passing if A and B are not equal.
 *
 * @note The test passes with high probability. See array_shuffle.
 */
void test_array_generate_sequence_shuffled() {
    bool is_passing = true;
    int* original_array = array_generate_sequence(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);
    int* shuffled_array = array_generate_sequence_shuffled(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);

    is_passing = is_passing && !array_is_equal(original_array, shuffled_array, NUM_ELEMS, NUM_ELEMS);

    free(original_array);
    free(shuffled_array);

    print_test_result(__FILE__, __func__, is_passing);
}

// End Array Generation Unit Tests
// Begin Linear Search Unit Tests

/**
 * @brief A unit test for array_linear_search.
 *
 * This function generates a shuffled sequence and searches for each
 * element in the sequence.
 *
 * The test is considered passing if the index returned by the search
 * function for each element in the shuffled array is equal to the
 * index of the element in the sequence.
 */
void test_array_linear_search() {
    bool is_passing = true;
    int* test = array_generate_sequence_shuffled(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);

    for (int i = 0; i < NUM_ELEMS; i++) {
        is_passing = is_passing && (array_linear_search(test, NUM_ELEMS, test[i]) == i);
    }

    free(test);
    print_test_result(__FILE__, __func__, is_passing);
}

/**
 * @brief A unit test for array_linear_search_range.
 *
 * The function generates a shuffled sequence and runs a nested
 * for-loop. The outer loop iterates over the sequence which produces
 * a lower bound and upper bound for the ranged search. The inner loop
 * iterates over the entire sequence and searches for each element in
 * the sequence. If the inner loop is at an element within the range
 * produced by the outer loop, array_linear_search_range should return
 * the index of the element. Otherwise, array_linear_search_range
 * should return -1.
 *
 * The test is considered passing if array_linear_search_range
 * correctly returns the index of the element if the element is within
 * the range and -1 if the element is not within the range.
 */
void test_array_linear_search_range() {
    bool is_passing = true;
    int* test = array_generate_sequence_shuffled(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);
    int window_size = NUM_ELEMS / 3;

    for (int i = 0; i < NUM_ELEMS - window_size; i++) {
        int lower_bound = i;
        int upper_bound = i + window_size;

        for (int j = 0; j < NUM_ELEMS; j++) {
            int search_result = array_linear_search_range(test, NUM_ELEMS, lower_bound, upper_bound, test[j]);
            int expected_result = (lower_bound <= j && j <= upper_bound) ? j : -1;
            is_passing = is_passing && (search_result == expected_result);
        }
    }

    free(test);
    print_test_result(__FILE__, __func__, is_passing);
}

/**
 * @brief A unit test for array_linear_search_or_closest.
 *
 * The function generates a sequence of multiples of 10. The test
 * runs from 0 to the maximum value in the sequence and calls
 * array_linear_search_or_closest. The function should return the
 * index of the element if the element is in the sequence. Otherwise,
 * the function should return the number of elements in the sequence
 * indicating that the element is not in the sequence and should the
 * user decide to insert the element, the element would be appended.
 *
 * The test is considered passing if the function returns the index of
 * the element if the element is in the sequence and the number of
 * elements in the sequence if the element is not in the sequence.
 */
void test_array_linear_search_or_closest() {
    bool is_passing = true;
    int* original_array = array_generate_sequence(0, 10, NUM_ELEMS);

    int max_value = original_array[NUM_ELEMS - 1];

    for (int i = 0; i < max_value; i++) {
        int search_result = array_linear_search_or_closest(original_array, NUM_ELEMS, i);

        if (i % 10 == 0) {
            is_passing = is_passing && (search_result == i / 10);
        } else {
            is_passing = is_passing && (search_result == NUM_ELEMS);
        }
    }

    free(original_array);
    print_test_result(__FILE__, __func__, is_passing);
}

/**
 * @brief A unit test for array_linear_search_range_or_closest.
 *
 * This function generates a sequence of multiples of 10 and runs a
 * nested for-loop. The outer loop iterates over the sequence which
 * produces a lower bound and upper bound for the ranged search. The
 * inner loop iterates over the entire sequence and searches for each
 * element in the sequence. If the inner loop is at an element within
 * the range produced by the outer loop,
 * array_linear_search_range_or_closest should return the index of
 * the element. Otherwise, it should return the index directly outside
 * the range indicating that the element is not in the sequence and
 * should the user decide to insert the element, the element would be
 * appended.
 *
 * The test is considered passing if the function returns the index of
 * the element if the element is in the sequence and the index
 * directly outside the range if the element is not in the sequence.
 */
void test_array_linear_search_range_or_closest() {
    bool is_passing = true;
    int* original_array = array_generate_sequence(0, 10, NUM_ELEMS);
    int window_size = NUM_ELEMS / 3;

    for (int i = 0; i < NUM_ELEMS - window_size; i++) {
        int lower_bound = i;
        int upper_bound = i + window_size;

        for (int j = 0; j < NUM_ELEMS; j++) {
            int search_result = array_linear_search_range_or_closest(original_array, NUM_ELEMS, lower_bound, upper_bound, j);

            if (lower_bound <= j && j <= upper_bound && j % 10 == 0) {
                is_passing = is_passing && (search_result == j / 10);
            } else {
                is_passing = is_passing && (search_result == NUM_ELEMS);
            }
        }
    }

    free(original_array);
    print_test_result(__FILE__, __func__, is_passing);
}

// End Linear Search Unit Tests
// Begin Binary Search Unit Tests

/**
 * @brief A unit test for array_binary_search.
 *
 * This function generates a sequence of numbers from 0 to
 * NUM_ELEMS - 1 and runs a sequence of searches for existing and
 * non-existing elements.
 *
 * The test is considered passing if the function returns the index of
 * the element if the element is in the sequence and -1 if the element
 * is not in the sequence.
 */
void test_array_binary_search() {
    bool is_passing = true;
    int* ordered_array = array_generate_sequence(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);

    for (int i = 0; i < NUM_ELEMS; i++) {
        int existing_search_result = array_binary_search(ordered_array, NUM_ELEMS, i);
        is_passing = is_passing && (existing_search_result == i);

        int nonexistent_search_result = array_binary_search(ordered_array, NUM_ELEMS, NUM_ELEMS + i);
        is_passing = is_passing && (nonexistent_search_result == -1);
    }

    free(ordered_array);
    print_test_result(__FILE__, __func__, is_passing);
}

/**
 * @brief A unit test for array_binary_search_range.
 *
 * This function generates a sequence of numbers from 0 to
 * NUM_ELEMS - 1 and runs a nested for-loop to check for correctness.
 * The outer loop iterates from 0 to NUM_ELEMS - window_size where
 * window_size is a defined integer which defines a range. The inner
 * loop iterates from 0 to NUM_ELEMS. If the current search value is
 * within the range, the function should return the index of the
 * element. Otherwise, the function should return -1.
 *
 * The test is considered passing if the function returns the index of
 * the element if the element is in the sequence and -1 if the element
 * is not in the sequence.
 */
void test_array_binary_search_range() {
    bool is_passing = true;
    int* ordered_array = array_generate_sequence(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);
    int window_size = NUM_ELEMS / 3;

    for (int i = 0; i < NUM_ELEMS - window_size; i++) {
        int lower_bound = i;
        int upper_bound = i + window_size;

        for (int j = 0; j < NUM_ELEMS; j++) {
            int search_result = array_binary_search_range(ordered_array, NUM_ELEMS, lower_bound, upper_bound, j);

            if (lower_bound <= j && j <= upper_bound) {
                is_passing = is_passing && (search_result == j);
            } else {
                is_passing = is_passing && (search_result == -1);
            }
        }
    }

    free(ordered_array);
    print_test_result(__FILE__, __func__, is_passing);
}

/**
 * @brief A unit test for array_binary_search_or_closest.
 *
 * This function generates a sequence of multiples of 10 and runs a
 * for-loop from 0 to the maximum value in the sequence. At each
 * iteration, the function computes the expected result and compares
 * it to the actual result returned by array_binary_search_or_closest.
 *
 * The test is considered passing if the function returns the index of
 * the element if the element is in the sequence and the index of
 * where the element should be inserted if the element is not in the
 * sequence.
 */
void test_array_binary_search_or_closest() {
    bool is_passing = true;
    int* ordered_array = array_generate_sequence(0, 10, NUM_ELEMS);
    int max_value = ordered_array[NUM_ELEMS - 1];

    for (int i = 0; i < max_value; i++) {
        int search_result = array_binary_search_or_closest(ordered_array, NUM_ELEMS, i);
        int expected_result = i / 10;

        if (i % 10 == 0) {
            is_passing = is_passing && (search_result == expected_result);
        } else {
            is_passing = is_passing && (search_result == expected_result + 1);
        }
    }

    free(ordered_array);
    print_test_result(__FILE__, __func__, is_passing);
}

/**
 * @brief A unit test for array_binary_search_range_or_closest.
 *
 * This function generates a sequence of numbers from 0 to
 * NUM_ELEMS - 1 and runs a nested for-loop to check for correctness.
 * The outer loop iterates from 0 to NUM_ELEMS - window_size where
 * window_size is a defined integer which defines a range. The inner
 * loop iterates from 0 to NUM_ELEMS. If the current search value is
 * within the range, the function should return the index of the
 * element. Otherwise, the function should return the index of the
 * closest element.
 *
 * The test is considered passing if the function returns the index of
 * the element if the element is in the sequence and the index of the
 * closest element if the element is not in the sequence.
 */
void test_array_binary_search_range_or_closest() {
    bool is_passing = true;
    int* ordered_array = array_generate_sequence(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);
    int window_size = NUM_ELEMS / 3;

    for (int i = 0; i < NUM_ELEMS - window_size; i++) {
        int lower_bound = i;
        int upper_bound = i + window_size;

        for (int j = 0; j < NUM_ELEMS; j++) {
            int search_result = array_binary_search_range_or_closest(ordered_array, NUM_ELEMS, lower_bound, upper_bound, j);

            if (lower_bound <= j && j <= upper_bound) {
                is_passing = is_passing && (search_result == j);
            } else if (j < lower_bound) {
                // if the element is less than the lower bound, then
                // the element would be inserted at the lower bound
                is_passing = is_passing && (search_result == lower_bound);
            } else {
                // if the element is greater than the upper bound,
                // then the element would be inserted at the index
                // after the upper bound (upper_bound + 1)
                is_passing = is_passing && (search_result == upper_bound + 1);
            }
        }
    }

    free(ordered_array);
    print_test_result(__FILE__, __func__, is_passing);
}

// End Binary Search Unit Tests
// Begin Manipulation Unit Tests

/**
 * @brief A unit test for array_binary_search.
 *
 * This function generates a sequence of numbers from 0 to
 * NUM_ELEMS - 1 and runs a sequence of searches for existing and
 * non-existing elements.
 *
 * This generates a sequence and performing a series of shift tests on
 * the sequence to check for general performance and edge cases. The
 * tests check for the following: (1) shifting left and right without
 * resizing the array, (2) shifting left and right with resizing the
 * array. Various shift sizes are tested ranging from 1 shift to a
 * large shift.
 *
 * The test is considered passing if the array is shifted correctly
 * and the number of elements in the array is correct after each
 * shift (for when resizing is allowed).
 */
void test_array_shift_out() {
    bool is_passing = true;
    int* test = array_generate_sequence(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);
    int num_elements = NUM_ELEMS;

    // result after shifting left 1 time at index 3
    int answer_1[] = {1, 2, 3, 0, 4, 5, 6, 7, 8, 9};
    num_elements = array_shift_out(&test, num_elements, -1, 3, false);
    is_passing = is_passing && array_is_equal(test, answer_1, num_elements, NUM_ELEMS);

    // result after shifting right 1 time at index 5
    int answer_2[] = {1, 2, 3, 0, 4, 0, 5, 6, 7, 8};
    num_elements = array_shift_out(&test, num_elements, 1, 5, false);
    is_passing = is_passing && array_is_equal(test, answer_2, num_elements, NUM_ELEMS);

    // result after shifting left 3 times at index 4
    int answer_3[] = {0, 4, 0, 0, 0, 0, 5, 6, 7, 8};
    num_elements = array_shift_out(&test, num_elements, -3, 4, false);
    is_passing = is_passing && array_is_equal(test, answer_3, num_elements, NUM_ELEMS);

    // result after shifting right 2 times at index 7
    int answer_4[] = {0, 4, 0, 0, 0, 0, 5, 0, 0, 6};
    num_elements = array_shift_out(&test, num_elements, 2, 7, false);
    is_passing = is_passing && array_is_equal(test, answer_4, num_elements, NUM_ELEMS);

    // result after shifting right 4 times at index 3 with resizing
    int answer_5[] = {0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 6};
    num_elements = array_shift_out(&test, num_elements, 4, 3, true);
    is_passing = is_passing && array_is_equal(test, answer_5, num_elements, num_elements);

    // // result after shifting left 4 times at index 3 with resizing
    int answer_6[] = {0, 0, 0, 0, 0, 0, 5, 0, 0, 6};
    num_elements = array_shift_out(&test, num_elements, -4, 3, true);
    is_passing = is_passing && array_is_equal(test, answer_6, num_elements, num_elements);

    free(test);
    print_test_result(__FILE__, __func__, is_passing);
}

/**
 * @brief A unit test for array_shift_in.
 *
 * The function generates an ordered sequence of numbers. The function
 * then uses a for-loop to decrement the array length by 1 and then
 * performs a resize. After each resize, the function checks if the
 * previous last element is set to 0 indicating a proper delete. The
 * function repeats this test where instead of decrementing the array
 * length, the array length is incremented by 1.
 *
 * The test is considered passing if the array is resized correctly
 * after each iteration.
 */
void test_array_resize() {
    bool is_passing = true;
    int* test = array_generate_sequence(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);

    for (int curr_len = NUM_ELEMS; curr_len > 0; curr_len--) {
        int next_len = curr_len - 1;
        array_resize(&test, curr_len, next_len);

        if (next_len == 0) {
            is_passing = is_passing && (test == NULL);
        } else {
            size_t expected_usable_size = sizeof(int) * (next_len);
            is_passing = is_passing && (malloc_usable_size(test) == expected_usable_size);
        }
    }

    // int* test_2 = NULL;
    // for (int curr_len = 0; curr_len < NUM_ELEMS; curr_len++) {
    //     int next_len = curr_len + 1;
    //     array_resize(&test_2, curr_len, next_len);

    //     size_t expected_usable_size = sizeof(int) * (next_len);
    //     is_passing = is_passing && (malloc_usable_size(test_2) == expected_usable_size);
    // }

    free(test);
    // free(test_2);
    print_test_result(__FILE__, __func__, is_passing);
}

/**
 * @brief A unit test for array_parallel_sort.
 *
 * The function generates two arrays, one with an ordered sequence of
 * numbers and the other with a random sequence of numbers. The
 * function then sorts the arrays in parallel. The function then
 * checks if the arrays are sorted correctly.
 *
 * The test is considered passing if the arrays are sorted correctly.
 *
 * @note There needs to be a better, more robust way to test this.
 */
void test_array_parallel_sort_2() {
    bool is_passing = true;

    int initial_A[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int initial_B[] = {8, 1, 9, 2, 3, 5, 6, 0, 4, 7};

    int expected_A[] = {7, 1, 3, 4, 8, 5, 6, 9, 0, 2};
    int expected_B[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    array_parallel_sort_2(initial_A, initial_B, NUM_ELEMS, NUM_ELEMS, true);

    is_passing = is_passing && array_is_equal(initial_A, expected_A, NUM_ELEMS, NUM_ELEMS);
    is_passing = is_passing && array_is_equal(initial_B, expected_B, NUM_ELEMS, NUM_ELEMS);

    print_test_result(__FILE__, __func__, is_passing);
}

void test_array_parallel_sort_3() {
    bool is_passing = true;

    int initial_A[] = {1, 7, 2, 8, 4, 8, 5, 9};
    int initial_B[] = {4, 1, 9, 3, 6, 4, 6, 7};
    int initial_C[] = {1, 2, 3, 4, 5, 6, 7, 8};

    int expected_A[] = {1, 2, 4, 5, 7, 8, 8, 9};
    int expected_B[] = {4, 9, 6, 6, 1, 3, 4, 7};
    int expected_C[] = {1, 3, 5, 7, 2, 4, 6, 8};

    int len_A = sizeof(initial_A) / sizeof(int);
    int len_B = sizeof(initial_B) / sizeof(int);
    int len_C = sizeof(initial_C) / sizeof(int);

    array_parallel_sort_3(initial_A, initial_B, initial_C, len_A, len_B, len_C, true);

    is_passing = is_passing && array_is_equal(initial_A, expected_A, len_A, len_A);
    is_passing = is_passing && array_is_equal(initial_B, expected_B, len_B, len_B);
    is_passing = is_passing && array_is_equal(initial_C, expected_C, len_C, len_C);

    print_test_result(__FILE__, __func__, is_passing);
}

// End Manipulation Unit Tests
// Begin Entry Function

void test_array_util() {
    // Create/ Delete Functions
    test_array_generate_sequence();
    test_array_generate_sequence_shuffled();

    // Search Functions (Linear)
    test_array_linear_search();
    test_array_linear_search_range();
    test_array_linear_search_or_closest();
    test_array_linear_search_range_or_closest();

    // Search Functions (Binary)
    test_array_binary_search();
    test_array_binary_search_range();
    test_array_binary_search_or_closest();
    test_array_binary_search_range_or_closest();

    // Maniuplation Functions
    test_array_shift_out();

    // Turns out realloc can return the same block of memory and
    // malloc_usable_size will return the same value. This is why
    // the test_array_resize function is failing.
    // test_array_resize();

    test_array_parallel_sort_2();
    test_array_parallel_sort_3();
}