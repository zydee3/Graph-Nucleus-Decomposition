#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../utilities/array_util.h"
#include "../utilities/file_io.h"
#include "compressed_sparse_row.h"
#include "ordered_set.h"

typedef int vertex;

typedef struct Graph {
    int num_vertices;
    int num_edges;
    bool is_directed;
    CompressedSparseRow* adjacency_matrix;
} Graph;

// Create and Delete Functions
Graph* graph_new(int num_vertices, int num_edges, bool is_directed);
Graph* graph_new_from_path(const char* file_path);
void graph_delete(Graph** graph);

// Manipulator Functions
Graph* graph_make_directed(Graph* graph, int (*f)(int, int, int*), int* meta_data);
Graph* graph_reduce(Graph* graph, bool* removed_vertices);

// Getter Functions
int graph_get_edge(Graph* graph, int row_idx, int col_idx);
int* graph_get_degrees(Graph* graph);
OrderedSet* graph_get_neighbors(Graph* graph, int idx_vertex_u);

// Utility Functions
void graph_print(Graph* graph, bool should_print_newline);

#endif