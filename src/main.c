#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "algorithms/peeling.h"
#include "algorithms/clique.h"
#include "algorithms/clique_bron_kerbosch.h"
#include "algorithms/core.h"
#include "collections/csr_graph.h"
// #include "utilities/file_io.h"

int main() {
    CSRGraph* graph = csr_graph_new_from_path("data/input/sample");
    OrderedSet** cliques = find_k_cliques_bron_kerbosch(graph, 3);

    return EXIT_SUCCESS;
}