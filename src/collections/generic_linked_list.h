#ifndef GENERIC_LINKED_LIST_H_INCLUDED
#define GENERIC_LINKED_LIST_H_INCLUDED

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "generic_node.h"

typedef struct GenericLinkedList {
    GenericNode* head;
    int size;

    void* (*_copy_data)(void*);
    void (*_delete_data)(void**);
    bool (*_compare_data)(void*, void*);
    void (*_print_data)(void*);
} GenericLinkedList;

GenericLinkedList* generic_linked_list_new(void* (*_copy_data)(void*), void (*_delete_data)(void**), bool (*_compare_data)(void*, void*), void (*_print_data)(void*));

GenericLinkedList* generic_linked_list_copy(GenericLinkedList* list);
void generic_linked_list_delete(GenericLinkedList** list);
bool generic_linked_list_compare(GenericLinkedList* list_a, GenericLinkedList* list_b);
void generic_linked_list_print(GenericLinkedList* list);

bool generic_linked_list_insert(GenericLinkedList* list, void* data);
bool generic_linked_list_remove(GenericLinkedList* list, void* data);
bool generic_linked_list_contains(GenericLinkedList* list, void* data);
bool generic_linked_list_is_empty(GenericLinkedList* list);

#endif