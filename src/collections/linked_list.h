#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "node.h"

typedef struct LinkedList {
    struct Node* head;
    int size;
} LinkedList;

LinkedList* linked_list_new();

LinkedList* linked_list_copy(LinkedList* list);
void linked_list_delete(LinkedList** list);
bool linked_list_compare(LinkedList* list_a, LinkedList* list_b);
void linked_list_print(LinkedList* list);

bool linked_list_insert(LinkedList* list, int data, int meta);
bool linked_list_remove(LinkedList* list, int data);
bool linked_list_contains(LinkedList* list, int data);
struct Node* linked_list_get(LinkedList* list, int data);
bool linked_list_is_empty(LinkedList* list);

bool linked_list_insert_child(LinkedList* list, int parent_data, int child_data, int child_meta);
bool linked_list_remove_child(LinkedList* list, int parent_data, int child_data);
bool linked_list_contains_child(LinkedList* list, int parent_data, int child_data);
struct Node* linked_list_get_child(LinkedList* list, int parent_data, int child_data);

#endif