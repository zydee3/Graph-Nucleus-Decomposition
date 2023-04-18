#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

typedef struct Node {
    int data;
    int meta;
    struct Node* next;
    struct LinkedList* children;
} Node;

Node* node_new(int data, int meta);

Node* node_copy(void* ref_node);
void node_delete(void** ptr_node);
bool node_compare(void* node_a, void* node_b);
void node_print(void* ref_node);

#endif