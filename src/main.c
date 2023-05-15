#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "algorithms/clique.h"
#include "algorithms/nucleus_decomposition.h"
#include "collections/clique_set.h"
#include "collections/graph.h"
#include "utilities/stopwatch.h"

int _compare_vertex_id2(vertex u, vertex v, int* _unused) {
    return max(u, v);
}

int _compare_degrees2(vertex u, vertex v, int* degrees) {
    int degree_u = degrees[u];
    int degree_v = degrees[v];

    if (degree_u == degree_v) {
        return u > v ? u : v;
    }

    return degree_u > degree_v ? u : v;
}

int main() {
    printf("\n\n");

    // Stopwatch* stopwatch = stopwatch_new();

    // char** graphs = (char**)malloc(sizeof(char*) * 5);
    // graphs[1] = "data/input/facebook";
    // graphs[2] = "data/input/ca-netscience";
    // graphs[3] = "data/input/facebook_caltech";
    // graphs[4] = "data/input/facebook";

    // for (int i = 1; i < 2; i++) {
    //     Graph* graph = graph_new_from_path(graphs[i]);
    //     printf("Generated CSR Graph in %.2f seconds (Directed: %s, Vertices: %d, Edges: %d).\n", stopwatch_lap(stopwatch), graph->is_directed ? "True" : "False", graph->num_vertices, graph->num_edges);

    //     run_nucleus_decomposition_3_4(graph);
    //     printf("Ran Nucleus Decomposition in %.2f seconds.\n", stopwatch_lap(stopwatch));
    // }

    // // Graph* graph = graph_new_from_path("data/input/facebook_caltech");
    // // printf("Generated CSR Graph in %.2f seconds (Directed: %s, Vertices: %d, Edges: %d).\n", stopwatch_lap(stopwatch), graph->is_directed ? "True" : "False", graph->num_vertices, graph->num_edges);

    // // run_nucleus_decomposition_3_4(graph);
    // // printf("Ran Nucleus Decomposition in %.2f seconds.\n", stopwatch_lap(stopwatch));

    Graph* graph = graph_new_from_path("data/input/sample");
    graph_print(graph, true);

    int* degrees = graph_get_degrees(graph);
    printf("Degrees: ");
    array_print(degrees, graph->num_vertices, true);

    Graph* directed_graph = graph_make_directed(graph, _compare_degrees2, degrees);
    CompressedSparseRow* csr = directed_graph->adjacency_matrix;

    printf("num_ptr_rows: %d\n", csr->num_ptr_rows);

    array_print(csr->ptr_rows, csr->num_ptr_rows, true);
    int counter = 0;

    for (int ptr = 0; ptr < csr->num_ptr_rows; ptr++) {
        int row_ptr = csr->ptr_rows[ptr];
        int row_end = csr->ptr_rows[ptr + 1];

        for (int idx = row_ptr; idx < row_end; idx++) {
            int col = csr->idx_cols[idx];

            printf("%d. %d %d \n", counter++, ptr, col);
        }
    }

    return EXIT_SUCCESS;
}