#include "test_linked_list.h"

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
bool _has_valid_sequence_ll(LinkedList* list, int start) {
    int counter = start;
    Node* node = list->head;

    while (node != NULL) {
        if (node->data != counter) {
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
void _insert_parents_ll(LinkedList* list, int end) {
    for (int i = 0; i < end; i++) {
        linked_list_insert(list, i, 0);
    }
}

/**
 * @brief Inserts the range [0, param end] into the param list.
 *
 * @param list The list to insert into.
 * @param end The last value of the range to insert.
 */
void _insert_values_reversed_ll(LinkedList* list, int end) {
    for (int i = end - 1; i >= 0; i--) {
        linked_list_insert(list, i, 0);
    }
}

/**
 * @brief Inserts the range [0, param end] as children into the param
 * list.
 *
 * This function assumes that the corresponding parents exists.
 *
 * @param list The list to insert into.
 * @param end The last value of the range to insert.
 */
void _insert_children_ll(LinkedList* list, int end) {
    for (int i = 0; i < end; i++) {
        for (int j = 0; j < end; j++) {
            linked_list_insert_child(list, i, j, 0);
        }
    }
}

// Tests

/**
 * @brief Tests linked_list_insert.
 *
 * The test inserts a range of numbers into the list. The test then
 * checks if the list contains an ordered set of integers starting at
 * 0. The test passes if the list contains the correct sequence.
 */
void test_linked_list_insert() {
    LinkedList* list = linked_list_new();
    _insert_parents_ll(list, 10);
    print_test_result(__FILE__, __func__, _has_valid_sequence_ll(list, 0));
    linked_list_delete(&list);
}

/**
 * @brief Tests linked_list_insert.
 *
 * The test inserts a range of numbers into the list. The test then
 * checks if the list contains an ordered set of integers starting at
 * 0. The test passes if the list contains the correct sequence.
 */
void test_linked_list_insert_reversed() {
    LinkedList* list = linked_list_new();
    _insert_values_reversed_ll(list, 10);
    print_test_result(__FILE__, __func__, _has_valid_sequence_ll(list, 0));
    linked_list_delete(&list);
}

/**
 * @brief Tests linked_list_copy.
 *
 * The test inserts a range of numbers into the list. The list is
 * copied and then checked. The test passes if the copy contains the
 * same sequence of numbers as the original list.
 */
void test_linked_list_copy() {
    LinkedList* list = linked_list_new();
    _insert_parents_ll(list, 10);
    LinkedList* copy = linked_list_copy(list);
    print_test_result(__FILE__, __func__, _has_valid_sequence_ll(copy, 0));
    linked_list_delete(&list);
    linked_list_delete(&copy);
}

/**
 * @brief Tests linked_list_contains.
 *
 * The test inserts a range of numbers into the list. The test then
 * loops and checks if the list contains each element in the range.
 * The test passes if the list contains all elements in the range.
 */
void test_linked_list_contains() {
    LinkedList* list = linked_list_new();

    const int num_elements = 10;
    _insert_parents_ll(list, num_elements);

    bool has_all_elements = true;
    for (int i = 0; i < num_elements; i++) {
        has_all_elements = has_all_elements && linked_list_contains(list, i);
    }

    print_test_result(__FILE__, __func__, has_all_elements);
    linked_list_delete(&list);
}

/**
 * @brief Tests linked_list_remove.
 *
 * The test inserts a range of numbers into the list. The test then
 * loops, removing an element and checking if the removed element is
 * still in the list, at each iteration. The test passes if the
 * element at each iteration is properly removed.
 *
 */
void test_linked_list_remove() {
    LinkedList* list = linked_list_new();

    const int num_elements = 10;
    _insert_parents_ll(list, num_elements);

    bool is_passing = true;
    for (int i = 0; i < num_elements; i++) {
        linked_list_remove(list, i);
        is_passing = is_passing && _has_valid_sequence_ll(list, i + 1);
    }

    print_test_result(__FILE__, __func__, is_passing);
    linked_list_delete(&list);
}

/**
 * @brief Tests linked_list_compare.
 *
 * The test creates two lists and inserts different ranges into each
 * list. A copy of the first list is created. The test passes if the
 * first and second list are not evaluated as equal and the first and
 * copied list are evaluated as equal.
 */
void test_linked_list_compare() {
    LinkedList* list1 = linked_list_new();
    LinkedList* list2 = linked_list_new();

    const int num_elements = 10;
    _insert_parents_ll(list1, num_elements);
    _insert_parents_ll(list2, num_elements / 2);

    LinkedList* list1_copy = linked_list_copy(list1);

    bool is_valid_difference = !linked_list_compare(list1, list2);
    bool is_valid_similar = linked_list_compare(list1, list1_copy);
    print_test_result(__FILE__, __func__, is_valid_difference && is_valid_similar);

    linked_list_delete(&list1);
    linked_list_delete(&list2);
    linked_list_delete(&list1_copy);
}

/**
 * @brief Tests linked_list_insert_child.
 *
 * The test creates a list and inserts a specified number of parents
 * and children at each parent to the list. The test then loops
 * through the list and checks if each parent within the specified
 * range of numbers exist. The test also checks if each parent has
 * the correct number of children. The test passes if the list
 * contains the correct sequence of parents and children to each
 * parent.
 *
 */
void test_linked_list_insert_child() {
    const int num_elements = 3;

    LinkedList* list = linked_list_new();
    _insert_parents_ll(list, num_elements);
    _insert_children_ll(list, num_elements);

    bool has_valid_children = true;

    Node* curr_node = list->head;
    while (curr_node != NULL) {
        has_valid_children = has_valid_children && _has_valid_sequence_ll(curr_node->children, 0);
        curr_node = curr_node->next;
    }

    print_test_result(__FILE__, __func__, has_valid_children);
    linked_list_delete(&list);
}

/**
 * @brief Tests linked_list_contains_child.
 *
 * The test creates a list and inserts a specified number of parents
 * and children at each parent to the list. The test then loops
 * through the list and checks if each parent has the specified
 * number of children. The test also checks for invalid children. The
 * test passes if the list contains the correct sequence of children
 * to each parent.
 *
 */
void test_linked_list_contains_child() {
    const int num_elements = 3;

    LinkedList* list = linked_list_new();
    _insert_parents_ll(list, num_elements);
    _insert_children_ll(list, num_elements);

    bool has_valid_children = true;

    for (int i = 0; i < num_elements; i++) {
        for (int j = 0; j < num_elements; j++) {
            has_valid_children = has_valid_children && linked_list_contains_child(list, i, j);
        }
    }

    for (int i = num_elements; i < num_elements * 2; i++) {
        for (int j = num_elements; j < num_elements * 2; j++) {
            has_valid_children = has_valid_children && !linked_list_contains_child(list, i, j);
        }
    }

    print_test_result(__FILE__, __func__, has_valid_children);
    linked_list_delete(&list);
}

/**
 * @brief Tests linked_list_remove_child.
 *
 * The test creates a list and inserts a specified number of parents
 * and children at each parent to the list. The test then removes
 * children from each parent one by one until the current parent has
 * no more children. At each iteration, the test checks if the child
 * removed was the correct child. The test passes if the list contains
 * the correct sequence of children to each parent after each removal.
 *
 */
void test_linked_list_delete_child() {
    const int num_elements = 3;

    LinkedList* list = linked_list_new();
    _insert_parents_ll(list, num_elements);
    _insert_children_ll(list, num_elements);

    bool has_valid_children = true;

    for (int i = 0; i < num_elements; i++) {
        for (int j = 0; j < num_elements; j++) {
            has_valid_children = has_valid_children && linked_list_remove_child(list, i, j);
            has_valid_children = has_valid_children && !linked_list_contains_child(list, i, j);
        }
    }

    print_test_result(__FILE__, __func__, has_valid_children);
    linked_list_delete(&list);
}

/**
 * @brief Tests linked_list_get_child.
 *
 * The test creates a list and inserts a specified number of parents
 * and children at each parent to the list. The test then loops and
 * gets a specified child. The test passes if all the children
 * returned is the correct child.
 */
void test_linked_list_get_child() {
    const int num_elements = 3;

    LinkedList* list = linked_list_new();
    _insert_parents_ll(list, num_elements);
    _insert_children_ll(list, num_elements);

    bool has_valid_gets = true;

    for (int i = 0; i < num_elements; i++) {
        for (int j = 0; j < num_elements; j++) {
            Node* child = linked_list_get_child(list, i, j);
            has_valid_gets = has_valid_gets && child->data == j;
        }
    }

    print_test_result(__FILE__, __func__, has_valid_gets);
    linked_list_delete(&list);
}

// Entry

/**
 * @brief Runs all tests for the generic linked list.
 */
void test_linked_list() {
    test_linked_list_insert();
    test_linked_list_insert_reversed();
    test_linked_list_copy();
    test_linked_list_contains();
    test_linked_list_remove();
    test_linked_list_compare();
    test_linked_list_insert_child();
    test_linked_list_contains_child();
    test_linked_list_delete_child();
    test_linked_list_get_child();
}