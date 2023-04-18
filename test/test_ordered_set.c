#include "test_ordered_set.h"

// Begin Create and Delete Function Unit Tests
/**
 * @brief Tests the ordered_set_new function.
 *
 * The function generates an unordered list of integers and inserts
 * them into an ordered set.
 *
 * The function is considered passing if the elements in the ordered
 * set are in ascending order.
 */
void test_ordered_set_insert() {
    int* test = array_generate_sequence_shuffled(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);
    int* answer = array_generate_sequence(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);

    OrderedSet* set = ordered_set_new(NUM_RESIZE_AMOUNT);
    ordered_set_insert_all(set, test, NUM_ELEMS);

    bool is_passing = memcmp(set->elements, answer, sizeof(int) * NUM_ELEMS) == 0;

    free(test);
    free(answer);
    ordered_set_delete(&set);

    print_test_result(__FILE__, __func__, is_passing);
}

// End Create and Delete Function Unit Tests
// Begin Manipulation Function Unit Tests

/**
 * @brief Tests the ordered_set_insert_all function.
 *
 * The function generates an unordered list of integers and inserts
 * them into an ordered set.
 *
 * The function is considered passing if the elements in the ordered
 * set are in ascending order.
 */
void test_ordered_set_insert_all() {
    int* test = array_generate_sequence_shuffled(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);
    int* answer = array_generate_sequence(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);

    OrderedSet* set = ordered_set_new(NUM_RESIZE_AMOUNT);
    ordered_set_insert_all(set, test, NUM_ELEMS);

    bool is_passing = memcmp(set->elements, answer, sizeof(int) * NUM_ELEMS) == 0;

    free(test);
    free(answer);
    ordered_set_delete(&set);

    print_test_result(__FILE__, __func__, is_passing);
}

/**
 * @brief Tests the ordered_set_remove function.
 *
 * The function generates an unordered list of integers and an ordered
 * set with the same integers as the unordered list. The function then
 * removes the integers from the ordered set in the same order as the
 * unordered list. At each step, the function checks if an element was
 * removed and if the removed element still exists in the ordered set.
 *
 * The function is considered passing if the correct element was
 * removed and if no other elements were removed.
 */
void test_ordered_set_remove() {
    bool is_passing = true;
    int* test = array_generate_sequence(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);
    int* remove_order = array_generate_sequence_shuffled(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);

    OrderedSet* set = ordered_set_new(NUM_RESIZE_AMOUNT);
    ordered_set_insert_all(set, test, NUM_ELEMS);

    for (int i = 0; i < NUM_ELEMS; i++) {
        is_passing = is_passing && ordered_set_remove(set, remove_order[i]);
        is_passing = is_passing && (array_binary_search(set->elements, set->size, remove_order[i]) == -1);
    }

    free(test);
    free(remove_order);
    ordered_set_delete(&set);

    print_test_result(__FILE__, __func__, is_passing);
}

/**
 * @brief Tests the ordered_set_fit function.
 *
 * The function generates an ordered list of integers and inserts them
 * into an ordered set. The function then iteratively removes the
 * elements from the ordered set one by one and fits the ordered set
 * after each removal. The function checks if the capacity of the
 * ordered set is equal to the size of the ordered set after each
 * removal.
 *
 * The function is considered passing if the capacity of the ordered
 * set is equal to the size of the ordered set after each removal and
 * each fit using ordered_set_fit.
 */
void test_ordered_set_fit() {
    bool is_passing = true;
    int* test = array_generate_sequence(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);

    OrderedSet* set = ordered_set_new(NUM_RESIZE_AMOUNT);
    ordered_set_insert_all(set, test, NUM_ELEMS);

    for (int i = 0; i < NUM_ELEMS; i++) {
        ordered_set_remove(set, i);
        ordered_set_fit(set);
        is_passing = is_passing && (set->capacity == set->size);
    }

    free(test);
    ordered_set_delete(&set);

    print_test_result(__FILE__, __func__, is_passing);
}

// End Manipulation Function Unit Tests
// Begin Operations Function Unit Tests

/**
 * @brief Tests the ordered_set_contains function.
 *
 * For two unique ordered sets A, B and an empty set E, the function
 * computes (AuB), (AuA), (BuA), (AuE), (EuA), and (EuE).
 *
 * The function is considered passing if the results of the six
 * computations are correct.
 */
void test_ordered_set_union() {
    bool is_passing = true;
    const int num_elements = 5;

    // Generate a set containing 0, 2, 4, 6, 8
    int* vals_A = array_generate_sequence(RANGE_ELEMS_S1_LOW, 2, num_elements);
    OrderedSet* A = ordered_set_new(NUM_RESIZE_AMOUNT);
    ordered_set_insert_all(A, vals_A, num_elements);

    // Generate a set containing 0, 3, 6, 9, 12
    int* vals_B = array_generate_sequence(RANGE_ELEMS_S1_LOW, 3, num_elements);
    OrderedSet* B = ordered_set_new(NUM_RESIZE_AMOUNT);
    ordered_set_insert_all(B, vals_B, num_elements);

    // Generate an empty set
    OrderedSet* E = ordered_set_new(NUM_RESIZE_AMOUNT);

    // Generate the union of A and B
    OrderedSet* AuB = ordered_set_union(A, B);
    int answer_AuB[] = {0, 2, 3, 4, 6, 8, 9, 12};
    int len_answer_AuB = sizeof(answer_AuB) / sizeof(int);
    is_passing = is_passing && (memcmp(AuB->elements, answer_AuB, sizeof(int) * len_answer_AuB) == 0);
    is_passing = is_passing && (AuB->size == len_answer_AuB);
    is_passing = is_passing && (AuB->capacity == len_answer_AuB);

    // Generate the union of A and A
    OrderedSet* AuA = ordered_set_union(A, A);
    is_passing = is_passing && (memcmp(AuA->elements, A->elements, sizeof(int) * A->size) == 0);
    is_passing = is_passing && (AuA->size == A->size);
    is_passing = is_passing && (AuA->capacity == A->size);

    // Generate the union of B and A
    OrderedSet* BuA = ordered_set_union(B, A);
    is_passing = is_passing && (memcmp(BuA->elements, answer_AuB, sizeof(int) * len_answer_AuB) == 0);
    is_passing = is_passing && (BuA->size == len_answer_AuB);
    is_passing = is_passing && (BuA->capacity == len_answer_AuB);

    // Generate the union of A and E
    OrderedSet* AuE = ordered_set_union(A, E);
    is_passing = is_passing && (memcmp(AuE->elements, A->elements, sizeof(int) * A->size) == 0);
    is_passing = is_passing && (AuE->size == A->size);
    is_passing = is_passing && (AuE->capacity == A->size);

    // Generate the union of E and A
    OrderedSet* EuA = ordered_set_union(E, A);
    is_passing = is_passing && (memcmp(EuA->elements, A->elements, sizeof(int) * A->size) == 0);
    is_passing = is_passing && (EuA->size == A->size);
    is_passing = is_passing && (EuA->capacity == A->size);

    // Generate the union of E and E
    OrderedSet* EuE = ordered_set_union(E, E);
    is_passing = is_passing && EuE->size == 0;
    is_passing = is_passing && EuE->capacity == 0;

    // Clean Up
    free(vals_A);
    free(vals_B);
    ordered_set_delete(&A);
    ordered_set_delete(&B);
    ordered_set_delete(&E);
    ordered_set_delete(&AuB);
    ordered_set_delete(&AuA);
    ordered_set_delete(&BuA);
    ordered_set_delete(&AuE);
    ordered_set_delete(&EuA);
    ordered_set_delete(&EuE);

    print_test_result(__FILE__, __func__, is_passing);
}

/**
 * @brief Tests the ordered_set_intersection function.
 *
 * For two unique ordered sets A, B and an empty set E, the function
 * computes (AnB), (AnA), (BnA), (AnE), (EnA), and (EnE).
 *
 * The function is considered passing if the results of the six
 * computations are correct.
 */
void test_ordered_set_intersection() {
    bool is_passing = true;
    const int num_elements = 5;

    // Generate a set containing 0, 2, 4, 6, 8
    int* vals_A = array_generate_sequence(RANGE_ELEMS_S1_LOW, 2, num_elements);
    OrderedSet* A = ordered_set_new(NUM_RESIZE_AMOUNT);
    ordered_set_insert_all(A, vals_A, num_elements);

    // Generate a set containing 0, 3, 6, 9, 12
    int* vals_B = array_generate_sequence(RANGE_ELEMS_S1_LOW, 3, num_elements);
    OrderedSet* B = ordered_set_new(NUM_RESIZE_AMOUNT);
    ordered_set_insert_all(B, vals_B, num_elements);

    // Generate an empty set
    OrderedSet* E = ordered_set_new(NUM_RESIZE_AMOUNT);

    // Compute and check result of (AnB)
    OrderedSet* AnB = ordered_set_intersection(A, B);
    int answer_AnB[] = {0, 6};
    int len_answer_AnB = sizeof(answer_AnB) / sizeof(int);
    is_passing = is_passing && memcmp(AnB->elements, answer_AnB, sizeof(int) * len_answer_AnB) == 0;
    is_passing = is_passing && AnB->size == len_answer_AnB;
    is_passing = is_passing && AnB->capacity == len_answer_AnB;

    // Compute and check result of (AnA)
    OrderedSet* AnA = ordered_set_intersection(A, A);
    is_passing = is_passing && memcmp(AnA->elements, A->elements, sizeof(int) * A->size) == 0;
    is_passing = is_passing && AnA->size == A->size;
    is_passing = is_passing && AnA->capacity == A->size;

    // Compute and check result of (BnA)
    OrderedSet* BnA = ordered_set_intersection(B, A);
    is_passing = is_passing && memcmp(BnA->elements, answer_AnB, sizeof(int) * len_answer_AnB) == 0;
    is_passing = is_passing && BnA->size == len_answer_AnB;
    is_passing = is_passing && BnA->capacity == len_answer_AnB;

    // Compute and check result of (AnE)
    OrderedSet* AnE = ordered_set_intersection(A, E);
    is_passing = is_passing && AnE->size == 0;
    is_passing = is_passing && AnE->capacity == 0;

    // Compute and check result of (EnA)
    OrderedSet* EnA = ordered_set_intersection(E, A);
    is_passing = is_passing && EnA->size == 0;
    is_passing = is_passing && EnA->capacity == 0;

    // Compute and check result of (EnE)
    OrderedSet* EnE = ordered_set_intersection(E, E);
    is_passing = is_passing && EnE->size == 0;
    is_passing = is_passing && EnE->capacity == 0;

    // Clean Up
    free(vals_A);
    free(vals_B);
    ordered_set_delete(&A);
    ordered_set_delete(&B);
    ordered_set_delete(&E);
    ordered_set_delete(&AnB);
    ordered_set_delete(&AnA);
    ordered_set_delete(&BnA);
    ordered_set_delete(&AnE);
    ordered_set_delete(&EnA);
    ordered_set_delete(&EnE);

    print_test_result(__FILE__, __func__, is_passing);
}

/**
 * @brief Tests the ordered_set_difference function.
 *
 * For two unique ordered sets A, B and an empty set E, the function
 * computes (A-B), (A-A), (B-A), (A-E), (E-A), and (E-E).
 *
 * The function is considered passing if the results of the six
 * computations are correct.
 */
void test_ordered_set_difference() {
    bool is_passing = true;
    const int len_A = 10;
    const int len_B = 5;

    // Generate a set containing 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    int* vals_A = array_generate_sequence(RANGE_ELEMS_S1_LOW, 1, len_A);
    OrderedSet* A = ordered_set_new(NUM_RESIZE_AMOUNT);
    ordered_set_insert_all(A, vals_A, len_A);

    // Generate a set containing 0, 2, 4, 6, 8, 9, 10
    int* vals_B = array_generate_sequence(RANGE_ELEMS_S1_LOW, 2, len_B);
    OrderedSet* B = ordered_set_new(NUM_RESIZE_AMOUNT);
    ordered_set_insert_all(B, vals_B, len_B);
    ordered_set_insert(B, 9);
    ordered_set_insert(B, 10);

    // Generate an empty set
    OrderedSet* E = ordered_set_new(NUM_RESIZE_AMOUNT);

    // Compute and check result of (A-B)
    int answer_A_B[] = {1, 3, 5, 7};
    int len_answer_A_B = sizeof(answer_A_B) / sizeof(int);
    OrderedSet* difference_set_A_B = ordered_set_difference(A, B);
    is_passing = is_passing && (memcmp(difference_set_A_B->elements, answer_A_B, sizeof(int) * len_answer_A_B) == 0);
    is_passing = is_passing && (difference_set_A_B->size == len_answer_A_B);
    is_passing = is_passing && (difference_set_A_B->capacity == len_answer_A_B);

    // Compute and check result of (A-A)
    int len_answer_A_A = 0;
    OrderedSet* difference_set_A_A = ordered_set_difference(A, A);
    is_passing = is_passing && (difference_set_A_A->size == len_answer_A_A);
    is_passing = is_passing && (difference_set_A_A->capacity == len_answer_A_A);

    // Compute and check result of (B-A)
    int answer_B_A[] = {10};
    int len_answer_B_A = sizeof(answer_B_A) / sizeof(int);
    OrderedSet* difference_set_B_A = ordered_set_difference(B, A);
    is_passing = is_passing && (memcmp(difference_set_B_A->elements, answer_B_A, sizeof(int) * len_answer_B_A) == 0);
    is_passing = is_passing && (difference_set_B_A->size == len_answer_B_A);
    is_passing = is_passing && (difference_set_B_A->capacity == len_answer_B_A);

    // Compute and check result of (A-E)
    int answer_A_E[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int len_answer_A_E = sizeof(answer_A_E) / sizeof(int);
    OrderedSet* difference_set_A_E = ordered_set_difference(A, E);
    is_passing = is_passing && (memcmp(difference_set_A_E->elements, answer_A_E, sizeof(int) * len_answer_A_E) == 0);
    is_passing = is_passing && (difference_set_A_E->size == len_answer_A_E);
    is_passing = is_passing && (difference_set_A_E->capacity == len_answer_A_E);

    // Compute and check result of (E-A)
    int len_answer_E_A = 0;
    OrderedSet* difference_set_E_A = ordered_set_difference(E, A);
    is_passing = is_passing && (difference_set_E_A->size == len_answer_E_A);
    is_passing = is_passing && (difference_set_E_A->capacity == len_answer_E_A);

    // Compute and check result of (E-E)
    int len_answer_E_E = 0;
    OrderedSet* difference_set_E_E = ordered_set_difference(E, E);
    is_passing = is_passing && (difference_set_E_E->size == len_answer_E_E);
    is_passing = is_passing && (difference_set_E_E->capacity == len_answer_E_E);

    // Free memory
    free(vals_A);
    free(vals_B);
    ordered_set_delete(&A);
    ordered_set_delete(&B);
    ordered_set_delete(&E);
    ordered_set_delete(&difference_set_A_B);
    ordered_set_delete(&difference_set_A_A);
    ordered_set_delete(&difference_set_B_A);
    ordered_set_delete(&difference_set_A_E);
    ordered_set_delete(&difference_set_E_A);
    ordered_set_delete(&difference_set_E_E);

    print_test_result(__FILE__, __func__, is_passing);
}

/**
 * @brief Tests the ordered_set_symmetric_difference function.
 *
 *
 * For two unique ordered sets A, B and an empty set E, the function
 * computes (AdB), (AdA), (BdA), (AdE), (EdA), and (EdE) where d
 * represents the symmetric difference operator delta.
 *
 * The function is considered passing if the results of the six
 * computations are correct.
 */
void test_ordered_set_symmetric_difference() {
    bool is_passing = true;

    // Generate a set containing 0, 3, 6, 9, 12, 15, 18, 21, 24, 27
    int* vals_A = array_generate_sequence(RANGE_ELEMS_S1_LOW, 3, NUM_ELEMS);
    OrderedSet* A = ordered_set_new(NUM_RESIZE_AMOUNT);
    ordered_set_insert_all(A, vals_A, NUM_ELEMS);

    // Generate a set containing 0, 2, 4, 6, 8, 10, 12, 14, 16, 18
    int* vals_B = array_generate_sequence(RANGE_ELEMS_S1_LOW, 2, NUM_ELEMS);
    OrderedSet* B = ordered_set_new(NUM_RESIZE_AMOUNT);
    ordered_set_insert_all(B, vals_B, NUM_ELEMS);

    // Generate an empty set
    OrderedSet* E = ordered_set_new(NUM_RESIZE_AMOUNT);

    // Compute and check result of (AdB)
    int answer_AdB[] = {2, 3, 4, 8, 9, 10, 14, 15, 16, 21, 24, 27};
    int len_answer_AdB = sizeof(answer_AdB) / sizeof(int);
    OrderedSet* symmetric_difference_set_AdB = ordered_set_symmetric_difference(A, B);
    is_passing = is_passing && (memcmp(symmetric_difference_set_AdB->elements, answer_AdB, sizeof(int) * len_answer_AdB) == 0);
    is_passing = is_passing && (symmetric_difference_set_AdB->size == len_answer_AdB);
    is_passing = is_passing && (symmetric_difference_set_AdB->capacity == len_answer_AdB);

    // Compute and check result of (AdA)
    OrderedSet* symmetric_difference_set_AdA = ordered_set_symmetric_difference(A, A);
    is_passing = is_passing && (symmetric_difference_set_AdA->size == 0);
    is_passing = is_passing && (symmetric_difference_set_AdA->capacity == 0);

    // Compute and check result of (BdA)
    OrderedSet* symmetric_difference_set_BdA = ordered_set_symmetric_difference(B, A);
    is_passing = is_passing && (memcmp(symmetric_difference_set_BdA->elements, answer_AdB, sizeof(int) * len_answer_AdB) == 0);
    is_passing = is_passing && (symmetric_difference_set_BdA->size == len_answer_AdB);
    is_passing = is_passing && (symmetric_difference_set_BdA->capacity == len_answer_AdB);

    // Compute and check result of (AdE)
    OrderedSet* symmetric_difference_set_AdE = ordered_set_symmetric_difference(A, E);
    is_passing = is_passing && (memcmp(symmetric_difference_set_AdE->elements, vals_A, sizeof(int) * A->size) == 0);
    is_passing = is_passing && (symmetric_difference_set_AdE->size == A->size);
    is_passing = is_passing && (symmetric_difference_set_AdE->capacity == A->size);

    // Compute and check result of (EdA)
    OrderedSet* symmetric_difference_set_EdA = ordered_set_symmetric_difference(E, A);
    is_passing = is_passing && (memcmp(symmetric_difference_set_EdA->elements, vals_A, sizeof(int) * A->size) == 0);
    is_passing = is_passing && (symmetric_difference_set_EdA->size == A->size);
    is_passing = is_passing && (symmetric_difference_set_EdA->capacity == A->size);

    // Compute and check result of (EdE)
    OrderedSet* symmetric_difference_set_EdE = ordered_set_symmetric_difference(E, E);
    is_passing = is_passing && (symmetric_difference_set_EdE->size == 0);
    is_passing = is_passing && (symmetric_difference_set_EdE->capacity == 0);

    // Clean up
    free(vals_A);
    free(vals_B);
    ordered_set_delete(&A);
    ordered_set_delete(&B);
    ordered_set_delete(&E);
    ordered_set_delete(&symmetric_difference_set_AdB);
    ordered_set_delete(&symmetric_difference_set_AdA);
    ordered_set_delete(&symmetric_difference_set_BdA);
    ordered_set_delete(&symmetric_difference_set_AdE);
    ordered_set_delete(&symmetric_difference_set_EdA);
    ordered_set_delete(&symmetric_difference_set_EdE);

    print_test_result(__FILE__, __func__, is_passing);
}

// End Operations Function Unit Tests
// Begin Utility Function Unit Tests

/**
 * @brief Tests the ordered_set_is_equal function.
 *
 * For two identical ordered sets A, B and an empty set E, the
 * function computes (A=B), (A=A), (B=A), (A=E), (E=A), and (E=E)
 * where = represents the compare operator.
 *
 *
 *
 */
void test_ordered_set_is_equal() {
    bool is_passing = true;

    // Generate a set containing 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    int* vals_A = array_generate_sequence(RANGE_ELEMS_S1_LOW, 3, NUM_ELEMS);
    OrderedSet* A = ordered_set_new(NUM_RESIZE_AMOUNT);
    ordered_set_insert_all(A, vals_A, NUM_ELEMS);

    // Generate a set containing 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    int* vals_B = array_generate_sequence(RANGE_ELEMS_S1_LOW, 3, NUM_ELEMS);
    OrderedSet* B = ordered_set_new(NUM_RESIZE_AMOUNT);
    ordered_set_insert_all(B, vals_B, NUM_ELEMS);

    OrderedSet* E = ordered_set_new(NUM_ELEMS);

    // Compute and check result of (A=B)
    is_passing = is_passing && ordered_set_is_equal(A, B);

    // Compute and check result of (A=A)
    is_passing = is_passing && ordered_set_is_equal(A, A);

    // Compute and check result of (B=A)
    is_passing = is_passing && ordered_set_is_equal(B, A);

    // Compute and check result of (A=E)
    is_passing = is_passing && !ordered_set_is_equal(A, E);

    // Compute and check result of (E=A)
    is_passing = is_passing && !ordered_set_is_equal(E, A);

    // Compute and check result of (E=E)
    is_passing = is_passing && ordered_set_is_equal(E, E);

    // Clean up
    free(vals_A);
    free(vals_B);
    ordered_set_delete(&A);
    ordered_set_delete(&B);
    ordered_set_delete(&E);

    print_test_result(__FILE__, __func__, is_passing);
}

// End Utility Function Unit Tests

void test_ordered_set() {
    test_ordered_set_insert();
    test_ordered_set_insert_all();
    test_ordered_set_remove();
    test_ordered_set_fit();
    test_ordered_set_union();
    test_ordered_set_intersection();
    test_ordered_set_difference();
    test_ordered_set_symmetric_difference();
    test_ordered_set_is_equal();
}