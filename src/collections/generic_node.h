#ifndef GENERIC_NODE_H_INCLUDED
#define GENERIC_NODE_H_INCLUDED

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct GenericNode {
    void* data;
    struct GenericNode* next;
} GenericNode;

GenericNode* generic_node_new(void* data);
GenericNode* generic_node_copy(void* ref_node, void* (*_copy_data)(void*), bool (*_compare_data)(void*, void*));
void generic_node_delete(void** ptr_node, void (*_delete_data)(void**));
bool generic_node_compare(void* node_a, void* node_b, bool (*_compare_data)(void*, void*));
void generic_node_print(void* ref_node, void (*_print_data)(void*));

#endif