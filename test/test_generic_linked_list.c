#include "test_generic_linked_list.h"

// Sample Generic

/**
 * @brief Creates a new int with param value and returns a pointer to
 * it. The caller is responsible for freeing the memory allocated for
 *
 * @param value The value to set the new int to.
 * @return int* Pointer to the new int.
 */
int* _int_new(int value) {
    int* ptr = (int*)malloc(sizeof(int));
    *ptr = value;
    return ptr;
}

/**
 * @brief Creates a deep copy of the value behind param ptr and
 * returns a pointer to it.
 *
 * @param ptr Pointer to the value to copy.
 * @return int* Pointer to the new value.
 */
void* _int_copy(void* ptr) {
    assert(ptr != NULL);
    return _int_new(*(int*)ptr);
}

/**
 * @brief Deletes the value behind param ptr and sets ptr to NULL.
 *
 * @param ptr Pointer to the value to delete.
 */
void _int_delete(void** ptr) {
    assert(ptr != NULL && *ptr != NULL);

    free(*ptr);
    *ptr = NULL;
}

/**
 * @brief Compares the values behind param a and param b. Returns TRUE
 * if the values are equal, FALSE otherwise. This function only works
 * for int values (should probably generalize this function using
 * memcmp).
 *
 * @param a Value to be compared against param b.
 * @param b Value to be compared against param a.
 * @return bool TRUE if param a and param b are equal, FALSE
 * otherwise.
 */
bool _int_compare(void* a, void* b) {
    assert(a != NULL && b != NULL);
    return *(int*)a == *(int*)b;
}

/**
 * @brief Prints the value behind param ptr to stdout.
 *
 * @param ptr Pointer to the value to print.
 */
void _int_print(void* ptr) {
    printf("%d", *(int*)ptr);
}

// Helper Functions

/**
 * @brief Checks if the param list contains an ordered set of integers
 * starting at param start.
 *
 * @param list The list to check.
 * @param start The first value in the sequence to check.
 * @return true If the list represents an ordered set of integers
 * starting at param start.
 * @return false otherwise.
 */
bool _has_valid_sequence(GenericLinkedList* list, int start) {
    int counter = start;
    GenericNode* node = list->head;

    while (node != NULL) {
        if (!list->_compare_data(node->data, &counter)) {
            return false;
        }

        counter++;
        node = node->next;
    }

    return true;
}

/**
 * @brief Inserts the range [0, param end] into the param list.
 *
 * @param list The list to insert into.
 * @param end The last value of the range to insert.
 */
void _insert_values(GenericLinkedList* list, int end) {
    for (int i = 0; i < end; i++) {
        generic_linked_list_insert(list, _int_new(i));
    }
}

// Tests

/**
 * @brief Tests generic_linked_list_insert.
 *
 * The test inserts a range of numbers into the list. The test then
 * checks if the list contains an ordered set of integers starting at
 * 0. The test passes if the list contains the correct sequence.
 */
void test_generic_linked_list_insert() {
    GenericLinkedList* list = generic_linked_list_new(_int_copy, _int_delete, _int_compare, _int_print);
    _insert_values(list, 10);
    print_test_result(__FILE__, __func__, _has_valid_sequence(list, 0));
    generic_linked_list_delete(&list);
}

/**
 * @brief Tests generic_linked_list_copy.
 *
 * The test inserts a range of numbers into the list. The list is
 * copied and then checked. The test passes if the copy contains the
 * same sequence of numbers as the original list.
 */
void test_generic_linked_list_copy() {
    GenericLinkedList* list = generic_linked_list_new(_int_copy, _int_delete, _int_compare, _int_print);
    _insert_values(list, 10);
    GenericLinkedList* copy = generic_linked_list_copy(list);
    print_test_result(__FILE__, __func__, _has_valid_sequence(copy, 0));
    generic_linked_list_delete(&list);
    generic_linked_list_delete(&copy);
}

/**
 * @brief Tests generic_linked_list_contains.
 *
 * The test inserts a range of numbers into the list. The test then
 * loops and checks if the list contains each element in the range.
 * The test passes if the list contains all elements in the range.
 */
void test_generic_linked_list_contains() {
    GenericLinkedList* list = generic_linked_list_new(_int_copy, _int_delete, _int_compare, _int_print);

    const int num_elements = 10;
    _insert_values(list, num_elements);

    bool has_all_elements = true;
    for (int i = 0; i < num_elements; i++) {
        has_all_elements = has_all_elements && generic_linked_list_contains(list, &i);
    }

    print_test_result(__FILE__, __func__, has_all_elements);
    generic_linked_list_delete(&list);
}

/**
 * @brief Tests generic_linked_list_remove.
 *
 * The test inserts a range of numbers into the list. The test then
 * loops, removing an element and checking if the removed element is
 * still in the list, at each iteration. The test passes if the
 * element at each iteration is properly removed.
 *
 */
void test_generic_linked_list_remove() {
    GenericLinkedList* list = generic_linked_list_new(_int_copy, _int_delete, _int_compare, _int_print);

    const int num_elements = 10;
    _insert_values(list, num_elements);

    bool is_passing = true;
    for (int i = 0; i < num_elements; i++) {
        generic_linked_list_remove(list, &i);
        is_passing = is_passing && _has_valid_sequence(list, i + 1);
    }

    print_test_result(__FILE__, __func__, is_passing);
    generic_linked_list_delete(&list);
}

/**
 * @brief Tests generic_linked_list_compare.
 *
 * The test creates two lists and inserts different ranges into each
 * list. A copy of the first list is created. The test passes if the
 * first and second list are not evaluated as equal and the first and
 * copied list are evaluated as equal.
 */
void test_generic_linked_list_compare() {
    GenericLinkedList* list1 = generic_linked_list_new(_int_copy, _int_delete, _int_compare, _int_print);
    GenericLinkedList* list2 = generic_linked_list_new(_int_copy, _int_delete, _int_compare, _int_print);

    const int num_elements = 10;
    _insert_values(list1, num_elements);
    _insert_values(list2, num_elements / 2);

    GenericLinkedList* list1_copy = generic_linked_list_copy(list1);

    bool is_valid_difference = !generic_linked_list_compare(list1, list2);
    bool is_valid_similar = generic_linked_list_compare(list1, list1_copy);
    print_test_result(__FILE__, __func__, is_valid_difference && is_valid_similar);

    generic_linked_list_delete(&list1);
    generic_linked_list_delete(&list2);
    generic_linked_list_delete(&list1_copy);
}

// Entry

/**
 * @brief Runs all tests for the generic linked list.
 */
void test_generic_linked_list() {
    test_generic_linked_list_insert();
    test_generic_linked_list_copy();
    test_generic_linked_list_contains();
    test_generic_linked_list_remove();
    test_generic_linked_list_compare();
}