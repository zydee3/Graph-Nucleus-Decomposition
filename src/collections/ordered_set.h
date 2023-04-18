#ifndef ORDERED_SET_H_INCLUDED
#define ORDERED_SET_H_INCLUDED

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utilities/array_util.h"

typedef struct OrderedSet {
    int* elements;
    int capacity;
    int size;
    int num_resize_amount;
} OrderedSet;

// Create and Delete Functions
OrderedSet* ordered_set_new(int num_resize_amount);
OrderedSet* ordered_set_copy(OrderedSet* ref);
void ordered_set_delete(OrderedSet** set);

// Manipulation Functions
bool ordered_set_insert(OrderedSet* set, int element);
int ordered_set_insert_all(OrderedSet* set, int* elements, int num_elements);
bool ordered_set_remove(OrderedSet* set, int element);
void ordered_set_fit(OrderedSet* set);  // todo fit all operation functions
void ordered_set_clear(OrderedSet* set);

// Operations Functions
OrderedSet* ordered_set_union(OrderedSet* set_1, OrderedSet* set_2);
OrderedSet* ordered_set_intersection(OrderedSet* set_1, OrderedSet* set_2);
OrderedSet* ordered_set_difference(OrderedSet* set_1, OrderedSet* set_2);
OrderedSet* ordered_set_symmetric_difference(OrderedSet* set_1, OrderedSet* set_2);

// Utility Functions
bool ordered_set_contains(OrderedSet* set, int element);
bool ordered_set_is_equal(OrderedSet* set_1, OrderedSet* set_2);
void ordered_set_print(OrderedSet* set, bool should_print_newline);

#endif