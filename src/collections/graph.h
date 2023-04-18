#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../utilities/file_io.h"
#include "linked_list.h"

typedef struct Graph {
    bool is_directed;
    int num_vertices;
    int num_edges;
    LinkedList* vertices;
} Graph;

Graph* graph_new(bool is_directed);

void* graph_copy(void* ref_graph);
void graph_delete(void** ptr_graph);
bool graph_compare(void* graph_a, void* graph_b);
void graph_print(void* ref_graph);

bool graph_add_vertex(Graph* graph, int vertex_value);
bool graph_remove_vertex(Graph* graph, int vertex_value);
Node* graph_get_vertex(Graph* graph, int vertex_value);

bool graph_add_edge(Graph* graph, int vertex_a_value, int vertex_b_value, int edge_weight);
bool graph_remove_edge(Graph* graph, int vertex_a_value, int vertex_b_value);
Node* graph_get_edge(Graph* graph, int vertex_a_value, int vertex_b_value);

#endif