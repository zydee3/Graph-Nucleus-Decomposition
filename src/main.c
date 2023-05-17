#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "algorithms/nucleus_decomposition.h"
#include "collections/graph.h"
#include "collections/three_four_cliques.h"

int main() {
    printf("\n\n");

    char* path = "data/input/sample";

    Stopwatch* stopwatch = stopwatch_new();

    Graph* graph = graph_new_from_file(path);
    printf("Generated CSR Graph in %.2f seconds (Directed: %s, Vertices: %d, Edges: %d).\n", stopwatch_lap(stopwatch), graph->is_directed ? "True" : "False", graph->num_vertices, graph->num_edges);

    ThreeFourCliques* collector = three_four_cliques_new(true);
    enumerate_four_cliques(graph, collector, three_four_cliques_record);
    printf("Enumerated 3,4-Cliques in %.2f seconds (3-Cliques: %d, 4-Cliques: %d).\n\n", stopwatch_lap(stopwatch), collector->num_three_cliques, collector->num_four_cliques);

    three_four_cliques_print(collector, true);

    three_four_cliques_delete(&collector);
    graph_delete(&graph);
    stopwatch_delete(&stopwatch);

    return EXIT_SUCCESS;
}