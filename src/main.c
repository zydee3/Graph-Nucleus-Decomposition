#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "algorithms/clique.h"
#include "algorithms/nucleus_decomposition.h"
#include "collections/clique_set.h"
#include "collections/graph.h"
#include "utilities/stopwatch.h"

int main() {
    printf("\n\n");

    Stopwatch* stopwatch = stopwatch_new();

    char** graphs = (char**)malloc(sizeof(char*) * 5);
    graphs[1] = "data/input/sample";
    graphs[2] = "data/input/ca-netscience";
    graphs[3] = "data/input/facebook_caltech";
    graphs[4] = "data/input/facebook";

    for (int i = 1; i < 5; i++) {
        Graph* graph = graph_new_from_path(graphs[i]);
        printf("Generated CSR Graph in %.2f seconds (Directed: %s, Vertices: %d, Edges: %d).\n", stopwatch_lap(stopwatch), graph->is_directed ? "True" : "False", graph->num_vertices, graph->num_edges);

        run_nucleus_decomposition_3_4(graph);
        printf("Ran Nucleus Decomposition in %.2f seconds.\n", stopwatch_lap(stopwatch));
    }

    // Graph* graph = graph_new_from_path("data/input/facebook_caltech");
    // printf("Generated CSR Graph in %.2f seconds (Directed: %s, Vertices: %d, Edges: %d).\n", stopwatch_lap(stopwatch), graph->is_directed ? "True" : "False", graph->num_vertices, graph->num_edges);

    // run_nucleus_decomposition_3_4(graph);
    // printf("Ran Nucleus Decomposition in %.2f seconds.\n", stopwatch_lap(stopwatch));

    return EXIT_SUCCESS;
}