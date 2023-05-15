#include "ordered_set.h"

// Begin Generic Functions

void* ordered_set_generic_copy(void* ref) {
    return ordered_set_copy((OrderedSet*)ref);
}

void ordered_set_generic_delete(void** set) {
    ordered_set_delete((OrderedSet**)set);
}

bool ordered_set_generic_is_equal(void* set_a, void* set_b) {
    return ordered_set_is_equal((OrderedSet*)set_a, (OrderedSet*)set_b);
}

void ordered_set_generic_print(void* set) {
    ordered_set_print((OrderedSet*)set, true);
}

// Begin Create and Delete Functions
/**
 * @brief Creates a new OrderedSet with the given resize amount.
 *
 * @param num_resize_amount The value to increase or decrease the
 * capacity of the ordered set by as values are inserted or removed.
 * @return OrderedSet* The newly created OrderedSet.
 */
OrderedSet* ordered_set_new(int num_resize_amount) {
    OrderedSet* set = malloc(sizeof(OrderedSet));

    set->elements = calloc(num_resize_amount, sizeof(int));
    set->capacity = num_resize_amount;
    set->size = 0;
    set->num_resize_amount = num_resize_amount;

    return set;
}

/**
 * @brief Creates a copy of the given OrderedSet.
 *
 * A new ordered set is created and initialized with the same
 * parameters as the referenced ordered set. The elements are then
 * copied from the referenced set to the new set using memcpy.
 *
 * @param ref The OrderedSet to copy.
 * @return OrderedSet* A deep copy of the referenced OrderedSet.
 */
OrderedSet* ordered_set_copy(OrderedSet* ref) {
    OrderedSet* set = malloc(sizeof(OrderedSet));

    set->elements = calloc(ref->capacity, sizeof(int));
    set->capacity = ref->capacity;
    set->size = ref->size;
    set->num_resize_amount = ref->num_resize_amount;

    memcpy(set->elements, ref->elements, ref->size * sizeof(int));

    return set;
}

/**
 * @brief Deletes the given OrderedSet and all members.
 *
 * The elements array is freed, then the OrderedSet is freed.
 *
 * @param set The OrderedSet to delete.
 */
void ordered_set_delete(OrderedSet** set) {
    assert(set != NULL);
    assert(*set != NULL);

    free((*set)->elements);
    free(*set);
    *set = NULL;
}

// End Create and Delete Functions
// Begin Manipulation Functions

/**
 * @brief Inserts the given element into the set.
 *
 * Binary search is used to find the index of the element. If the
 * element is already in the set, false is returned. If the set is
 * full, the capacity is increased by the resize amount. If the
 * element needs to be inserted in the middle of the set, the
 * elements to the right of where the element is to be inserted are
 * shifted to the right. The element is then inserted at the index
 * of the element. The size is increased by 1 and true is returned.
 *
 * @param set The OrderedSet to insert the param element into.
 * @param element The element to insert into the set.
 * @return bool True if the element was inserted, false otherwise.
 */
bool ordered_set_insert(OrderedSet* set, int element) {
    assert(set != NULL);

    // Find the index of the element if it exists or the position to
    // insert it if it does not.
    int idx_element = 0;
    if (set->size > 0) {
        idx_element = array_binary_search_or_closest(set->elements, set->size, element);
    }

    // If the element is already in the set, return false.
    if (idx_element < set->size && set->elements[idx_element] == element) {
        return false;
    }

    // If the set is full, resize it.
    if (set->size == set->capacity) {
        int new_capacity = set->capacity + set->num_resize_amount;

        set->elements = realloc(set->elements, new_capacity * sizeof(int));
        assert(set->elements != NULL);

        set->capacity = new_capacity;
    }

    // If the element needs to be insertted in the middle of the set,
    // shift the elements to the right.
    memmove(&set->elements[idx_element + 1], &set->elements[idx_element], (set->size - idx_element) * sizeof(int));
    // Insert the element.
    set->elements[idx_element] = element;

    // Increment the size.
    set->size++;

    return true;
}

/**
 * @brief Inserts all elements in the given array into the set.
 *
 * Using a for-loop, the elements are inserted into the ordered set
 * using the ordered_set_insert function. After each successful
 * insertion, the number of inserted elements is incremented. The
 * number of inserted elements is returned.
 *
 * @param set The OrderedSet to insert the elements into.
 * @param elements The elements to insert into the set.
 * @param num_elements The number of elements to insert into the set.
 * @return int The number of elements inserted into the set.
 */
int ordered_set_insert_all(OrderedSet* set, int* elements, int num_elements) {
    assert(set != NULL);
    assert(elements != NULL);

    int num_inserted = 0;

    for (int i = 0; i < num_elements; i++) {
        if (ordered_set_insert(set, elements[i])) {
            num_inserted++;
        }
    }

    return num_inserted;
}

/**
 * @brief Removes the given element from the set.
 *
 * Binary search is used to find the index of the element. If the
 * element is not in the set, false is returned. If the element
 * exists in the set, the element is removed by shifting all elements
 * to the right of the element to the left. The size is decremented
 * by 1. If the set is too large, the capacity is decreased by the
 * resize amount specified at initialization. True is returned.
 *
 * @param set The OrderedSet to remove the param element from.
 * @param element The element to remove from the set.
 * @return bool True if the element was removed, false otherwise.
 */
bool ordered_set_remove(OrderedSet* set, int element) {
    assert(set != NULL);

    int idx_element = array_binary_search(set->elements, set->size, element);

    if (idx_element == -1) {
        return false;
    }

    // Shift the elements to the left.
    // array_shift(&set->elements, set->capacity, -1, idx_element, false);
    memmove(&set->elements[idx_element], &set->elements[idx_element + 1], (set->size - idx_element - 1) * sizeof(int));
    set->elements[set->size - 1] = 0;
    // Decrement the size.
    set->size--;

    // If the set is too large, resize it.
    if (set->size < set->capacity - set->num_resize_amount) {
        int new_capacity = set->capacity - set->num_resize_amount;

        set->elements = realloc(set->elements, new_capacity * sizeof(int));
        assert(set->elements != NULL);

        set->capacity = new_capacity;
    }

    return true;
}

void ordered_set_fit(OrderedSet* set) {
    assert(set != NULL);

    set->elements = realloc(set->elements, set->size * sizeof(int));

    set->capacity = set->size;
}

/**
 * @brief Removes all elements from the set.
 *
 * The size is set to 0 and the elements array is cleared by setting
 * each element to zero using memset. The capacity is set to the
 * initial capacity (the resize amount specified at initialization).
 *
 * @param set
 */
void ordered_set_clear(OrderedSet* set) {
    assert(set != NULL);

    set->size = 0;
    set->capacity = set->num_resize_amount;

    memset(set->elements, 0, set->capacity * sizeof(int));
}

// End Manipulation Functions
// Begin Operations Functions

/**
 * @brief Computes the union of param set_1 and param set_2.
 *
 * If either set is empty, a fitted copy of the other set is returned.
 * The function then uses a while-loop to iterate through both sets to
 * find the union. The union is stored in a new OrderedSet and fitted
 * before being returned.
 *
 * @param set_1 The first set to compute the union of.
 * @param set_2 The second set to compute the union of.
 * @return OrderedSet* The union of the two sets.
 */
OrderedSet* ordered_set_union(OrderedSet* set_1, OrderedSet* set_2) {
    assert(set_1 != NULL);
    assert(set_2 != NULL);

    // If either set is empty, a fitted copy of the other set can be
    // immediately returned.
    if (set_1->size == 0) {
        OrderedSet* copy = ordered_set_copy(set_2);
        ordered_set_fit(copy);
        return copy;
    }

    if (set_2->size == 0) {
        OrderedSet* copy = ordered_set_copy(set_1);
        ordered_set_fit(copy);
        return copy;
    }

    // Create the new set. If the two sets are disjoint, the new set
    // will have a capacity equal to the sum of the two sets.
    OrderedSet* union_set = ordered_set_new(set_1->size + set_2->size);

    // Iterate through both sets to find the union.
    int idx_set_1 = 0;
    int idx_set_2 = 0;

    while (idx_set_1 < set_1->size && idx_set_2 < set_2->size) {
        if (set_1->elements[idx_set_1] == set_2->elements[idx_set_2]) {
            ordered_set_insert(union_set, set_1->elements[idx_set_1]);
            idx_set_1++;
            idx_set_2++;
        } else if (set_1->elements[idx_set_1] < set_2->elements[idx_set_2]) {
            ordered_set_insert(union_set, set_1->elements[idx_set_1]);
            idx_set_1++;
        } else {
            ordered_set_insert(union_set, set_2->elements[idx_set_2]);
            idx_set_2++;
        }
    }

    // If there are any remaining elements in either set, add them to
    // the union set. Only one of the two memcpy will add anything.
    memcpy(&union_set->elements[union_set->size], &set_1->elements[idx_set_1], (set_1->size - idx_set_1) * sizeof(int));
    union_set->size += set_1->size - idx_set_1;

    memcpy(&union_set->elements[union_set->size], &set_2->elements[idx_set_2], (set_2->size - idx_set_2) * sizeof(int));
    union_set->size += set_2->size - idx_set_2;

    // Free up unused memory at the tail of the set. Realloc for
    // reducing the array size is fast as the flag is only released.
    ordered_set_fit(union_set);

    return union_set;
}

/**
 * @brief Computes the intersection of param set_1 and param set_2.
 *
 * If either set is empty, an empty set is returned. The function then
 * uses a while-loop to iterate through both sets to find the
 * intersection. The intersection is stored in a new OrderedSet and
 * fitted before being returned.
 *
 * @param set_1 The first set to compute the intersection of.
 * @param set_2 The second set to compute the intersection of.
 * @return OrderedSet* The intersection of the two sets.
 */
OrderedSet* ordered_set_intersection(OrderedSet* set_1, OrderedSet* set_2) {
    assert(set_1 != NULL);
    assert(set_2 != NULL);

    // If either set is empty, an empty set can be immediately
    // returned as the intersection is the empty set.
    if (set_1->size == 0 || set_2->size == 0) {
        return ordered_set_new(0);
    }

    OrderedSet* smaller_set;
    OrderedSet* larger_set;

    if (set_1->size < set_2->size) {
        smaller_set = set_1;
        larger_set = set_2;
    } else {
        smaller_set = set_2;
        larger_set = set_1;
    }

    OrderedSet* intersection_set = ordered_set_new(smaller_set->size);
    int idx_intersection_set = 0;

    for (int i = 0; i < smaller_set->size; i++) {
        int element = smaller_set->elements[i];
        if (ordered_set_contains(larger_set, element)) {
            intersection_set->elements[idx_intersection_set] = element;
            idx_intersection_set++;
            intersection_set->size++;
        }
    }

    ordered_set_fit(intersection_set);

    return intersection_set;
}

/**
 * @brief Computes the difference of param set_1 and param set_2.
 *
 * @note The result of (set_1 - set_2) != (set_2 - set_1).
 *
 * If set_1 is empty, an empty set is returned. If set_2 is empty, a
 * fitted copy of set_1 is returned. The function then uses a
 * while-loop to iterate through both sets to find the difference. The
 * difference is stored in a new OrderedSet and fitted before being
 * returned.
 *
 * @param set_1 The set to be subtracted from.
 * @param set_2 The set to subtract from set_1.
 * @return OrderedSet* The difference of the two sets.
 */
OrderedSet* ordered_set_difference(OrderedSet* set_1, OrderedSet* set_2) {
    assert(set_1 != NULL);
    assert(set_2 != NULL);

    // If set_1 is empty, an empty set can be immediately returned.
    if (set_1->size == 0) {
        return ordered_set_new(0);
    }

    // If set_2 is empty, a fitted copy of set_1 can be immediately
    // returned.
    if (set_2->size == 0) {
        OrderedSet* copy = ordered_set_copy(set_1);
        ordered_set_fit(copy);
        return copy;
    }

    // The maximum size the difference set can have is set_1->size.
    OrderedSet* difference_set = ordered_set_new(set_1->size);

    // Iterate through both sets, removing elements from set_1 that
    // are also in set_2, to find the difference.
    int idx_set_1 = 0;
    int idx_set_2 = 0;

    while (idx_set_1 < set_1->size && idx_set_2 < set_2->size) {
        if (set_1->elements[idx_set_1] == set_2->elements[idx_set_2]) {
            idx_set_1++;
            idx_set_2++;
        } else if (set_1->elements[idx_set_1] < set_2->elements[idx_set_2]) {
            ordered_set_insert(difference_set, set_1->elements[idx_set_1]);
            idx_set_1++;
        } else {
            idx_set_2++;
        }
    }

    // If there are any elements left in set_1, add them to the
    // difference set. This can be done since set_1 has items
    // remaining and set_2 has been exhausted.
    memcpy(&difference_set->elements[difference_set->size], &set_1->elements[idx_set_1], (set_1->size - idx_set_1) * sizeof(int));
    difference_set->size += set_1->size - idx_set_1;

    // Free up unused memory at the tail of the set. Realloc for
    // reducing the array size is fast as the flag is only released.
    ordered_set_fit(difference_set);

    return difference_set;
}

/**
 * @brief Computes the symmetric difference of param set_1 and
 * param set_2.
 *
 * If both sets are empty, an empty set is returned. If either set is
 * empty, but the other is not, a fitted copy of the non-empty set is
 * returned. The function then uses a while-loop to iterate through
 * both sets to find the symmetric difference. The symmetric
 * difference is stored in a new OrderedSet and fitted before being
 * returned.
 *
 * @param set_1 The first set to compute the symmetric difference of.
 * @param set_2 The second set to compute the symmetric difference of.
 * @return OrderedSet* The symmetric difference of the two sets.
 */
OrderedSet* ordered_set_symmetric_difference(OrderedSet* set_1, OrderedSet* set_2) {
    assert(set_1 != NULL);
    assert(set_2 != NULL);

    if (set_1->size == 0 && set_2->size == 0) {
        return ordered_set_new(0);
    }

    if (set_1->size == 0) {
        OrderedSet* copy = ordered_set_copy(set_2);
        ordered_set_fit(copy);
        return copy;
    }

    if (set_2->size == 0) {
        OrderedSet* copy = ordered_set_copy(set_1);
        ordered_set_fit(copy);
        return copy;
    }

    // If the two sets are disjoint, the symmetric difference is the
    // set containing all elements from both sets (complete union).
    OrderedSet* symmetric_difference_set = ordered_set_new(set_1->size + set_2->size);

    // Iterate through both sets, removing elements from set_1 that
    // are also in set_2, to find the symmetric difference.
    int idx_set_1 = 0;
    int idx_set_2 = 0;

    while (idx_set_1 < set_1->size && idx_set_2 < set_2->size) {
        if (set_1->elements[idx_set_1] == set_2->elements[idx_set_2]) {
            idx_set_1++;
            idx_set_2++;
        } else if (set_1->elements[idx_set_1] < set_2->elements[idx_set_2]) {
            ordered_set_insert(symmetric_difference_set, set_1->elements[idx_set_1]);
            idx_set_1++;
        } else {
            ordered_set_insert(symmetric_difference_set, set_2->elements[idx_set_2]);
            idx_set_2++;
        }
    }

    // can't use memcpy here because we need to insert the elements
    // into the set in order; memcpy would just copy the elements.
    for (int i = idx_set_1; i < set_1->size; i++) {
        ordered_set_insert(symmetric_difference_set, set_1->elements[i]);
    }

    for (int i = idx_set_2; i < set_2->size; i++) {
        ordered_set_insert(symmetric_difference_set, set_2->elements[i]);
    }

    // Free up unused memory at the tail of the set. Realloc for
    // reducing the array size is fast as the flag is only released.
    ordered_set_fit(symmetric_difference_set);

    return symmetric_difference_set;
}

// End Operations Functions
// Begin Morphing  Functions

OrderedSet** ordered_set_as_array(GenericLinkedList** sets) {
    assert(sets != NULL);
    assert(*sets != NULL);

    int num_sets = (*sets)->size;
    OrderedSet** sets_array = calloc(num_sets, sizeof(OrderedSet*));

    int idx_array = 0;
    GenericNode* current_node = (*sets)->head;

    while (current_node != NULL) {
        OrderedSet* current_set = (OrderedSet*)current_node->data;
        sets_array[idx_array] = current_set;

        current_node->data = NULL;

        current_node = current_node->next;
        idx_array++;
    }

    generic_linked_list_delete(sets);

    return sets_array;
}

// End Morphing Functions
// Begin Utility Functions

/**
 * @brief Returns true if the set contains the element, otherwise
 * false.
 *
 * @param set
 * @param element
 * @return true
 * @return false
 */
bool ordered_set_contains(OrderedSet* set, int element) {
    assert(set != NULL);

    return array_binary_search(set->elements, set->size, element) >= 0;
}

/**
 * @brief Compares the two sets for equality.
 *
 * If both sets are NULL, true is returned. If one set is NULL and the
 * other is not, false is returned. If the two sets have different
 * capacities, false is returned. Otherwise, the function returns the
 * result of calling array_is_equal on the two sets' elements.
 * array_is_equal checks the size of the arrays and then compares the
 * elements in the arrays using memcmp.
 *
 * @param set_1 The first set to compare.
 * @param set_2 The second set to compare.
 * @return bool True if the two sets are equal, false otherwise.
 */
bool ordered_set_is_equal(OrderedSet* set_1, OrderedSet* set_2) {
    if (set_1 == NULL && set_2 == NULL) {
        return true;
    }

    if (set_1 == NULL || set_2 == NULL) {
        return false;
    }

    if (set_1->capacity != set_2->capacity) {
        return false;
    }

    return array_is_equal(set_1->elements, set_2->elements, set_1->size, set_2->size);
}

/**
 * @brief Prints the size, capacity and elements of the set to stdout.
 * If should_print_newline is true, a newline is printed at the end of
 * the function.
 *
 * @param set The set to print.
 * @param should_print_newline True if a newline should be printed
 * at the end of all print statements, false otherwise.
 */
void ordered_set_print(OrderedSet* set, bool should_print_newline) {
    assert(set != NULL);

    printf(" SET { size: %d, capacity: %d, elements: [", set->size, set->capacity);
    array_print(set->elements, set->size, false);
    printf("] }");

    if (should_print_newline) {
        printf("\n");
    }
}

// End Utility Functions