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

    // Turns out realloc can return the same block of memory and
    // malloc_usable_size will return the same value. This is why
    // the test_array_resize function is failing.
    // test_array_resize();

    test_array_parallel_sort_2();
}