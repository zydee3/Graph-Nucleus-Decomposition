#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "algorithms/nucleus_decomposition.h"
#include "collections/csr_graph.h"

void print_time_elapsed(char* fmt, clock_t start_time) {
    clock_t end_time = clock();

    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf(fmt, elapsed_time);
}

int main() {
    printf("\n\n");

    clock_t start_time = clock();

    CSRGraph* graph = csr_graph_new_from_path("data/input/facebook_caltech");
    print_time_elapsed("Created CSR Graph in %.2f seconds (", start_time);
    printf("Directed: %s, Vertices: %d, Edges: %d).\n", graph->is_directed ? "True" : "False", graph->num_vertices, graph->num_edges);

    start_time = clock();
    run_nucleus_decomposition_3_4(graph);
    print_time_elapsed("Ran nucleus decomposition in %.2f seconds.\n", start_time);

    csr_graph_delete(&graph);

    return EXIT_SUCCESS;
}