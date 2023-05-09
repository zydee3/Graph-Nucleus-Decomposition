#include "clique_triangles.h"

// Begin Generic Linked List Triangle Functions

void* triangle_copy(void* ref) {
    assert(ref != NULL);

    int* triangle = (int*)ref;
    int* copy = calloc(3, sizeof(int));
    memcpy(copy, triangle, 3 * sizeof(int));

    return copy;
}

void triangle_delete(void** array) {
    assert(array != NULL);
    assert(*array != NULL);

    free(*array);
    *array = NULL;
}

bool triangle_is_equal(void* array_a, void* array_b) {
    assert(array_a != NULL);
    assert(array_b != NULL);

    int* triangle_a = (int*)array_a;
    int* triangle_b = (int*)array_b;

    return memcmp(triangle_a, triangle_b, 3 * sizeof(int)) == 0;
}

void triangle_print(void* array) {
    assert(array != NULL);

    int* triangle = (int*)array;
    printf("(%d, %d, %d)", triangle[0], triangle[1], triangle[2]);
}

// End Generic Linked List Triangle Functions
// Begin Helpers

/**
 * @brief This function returns the vertex with the highest degree.
 * Ties are broken by choosing the vertex with the higher index.
 *
 * @param row_idx
 * @param col_idx
 * @param degrees
 * @return int
 */
int _compare_degrees(int row_idx, int col_idx, int* degrees) {
    int degree_u = degrees[row_idx];
    int degree_v = degrees[col_idx];

    if (degree_u == degree_v) {
        return row_idx > col_idx ? row_idx : col_idx;
    }

    return degree_u > degree_v ? row_idx : col_idx;
}

// End Helpers
// Begin Main Functions

GenericLinkedList* compute_triangles(Graph* graph) {
    assert(graph != NULL);
    assert(graph->is_directed == false);
    assert(graph->adjacency_matrix != NULL);
    assert(graph->adjacency_matrix->is_set);

    // Benchmarking
    Stopwatch* stopwatch = stopwatch_new();

    // Triangle results list
    GenericLinkedList* triangles = generic_linked_list_new(triangle_copy, triangle_delete, triangle_is_equal, triangle_print);

    // Generate a degree oriented graph
    int* undirected_degrees = graph_get_degrees(graph);
    Graph* directed_graph = graph_make_directed(graph, _compare_degrees, undirected_degrees);
    printf("Generated directed graph in %.2f seconds (Directed: %s, Vertices: %d, Edges: %d).\n", stopwatch_lap(stopwatch), directed_graph->is_directed ? "True" : "False", directed_graph->num_vertices, directed_graph->num_edges);
    free(undirected_degrees);

    // Store these variables for easy access
    CompressedSparseRow* adjacency_matrix = directed_graph->adjacency_matrix;
    int* ptr_rows = adjacency_matrix->ptr_rows;
    int* idx_cols = adjacency_matrix->idx_cols;

    // Loop over the (filtered) out-edges of v
    for (vertex v = 0; v < graph->num_vertices; v++) {
        int idx_v_begin_read = ptr_rows[v];
        int idx_v_end_read = ptr_rows[v + 1];

        for (int idx_u_nnz = idx_v_begin_read; idx_u_nnz < idx_v_end_read; idx_u_nnz++) {
            vertex u = idx_cols[idx_u_nnz];

            for (int idx_w_nnz = idx_u_nnz + 1; idx_w_nnz < idx_v_end_read; idx_w_nnz++) {
                vertex w = idx_cols[idx_w_nnz];

                if (graph_get_edge(graph, u, w) > 0) {
                    int* triangle = calloc(3, sizeof(int));
                    triangle[0] = min_3(u, v, w);
                    triangle[2] = max_3(u, v, w);
                    triangle[1] = u + v + w - triangle[0] - triangle[2];

                    generic_linked_list_insert(triangles, triangle);
                }
            }
        }
    }

    printf("Found triangles in %.2f seconds (Count: %d).\n", stopwatch_lap(stopwatch), triangles->size);

    stopwatch_delete(&stopwatch);
    return triangles;
}

// End Main Functions