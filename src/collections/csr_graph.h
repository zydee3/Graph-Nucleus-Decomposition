#ifndef CRS_GRAPH_H_INCLUDED
#define CRS_GRAPH_H_INCLUDED

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../utilities/array_util.h"
#include "../utilities/file_io.h"
#include "csr.h"

typedef struct CSRGraph {
    int num_vertices;
    int num_edges;
    bool is_directed;
    CompressedSparseRow* adjacency_matrix;
} CSRGraph;

// Create and Delete Functions
CSRGraph* csr_graph_new(int num_vertices, int num_edges, bool is_directed);
CSRGraph* csr_graph_new_from_path(const char* file_path);
void csr_graph_delete(CSRGraph** graph);

// Manipulator Functions
CSRGraph* csr_graph_make_directed(CSRGraph* graph, int (*f)(int, int, int*), int* meta_data);
CSRGraph* csr_graph_reduce(CSRGraph* graph, bool* removed_vertices);

// Getter Functions
int csr_graph_get_edge(CSRGraph* graph, int row_idx, int col_idx);
int* csr_graph_get_degrees(CSRGraph* graph);

// Utility Functions
void csr_graph_print(CSRGraph* graph, bool should_print_newline);

#endif