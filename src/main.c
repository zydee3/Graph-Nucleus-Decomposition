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
#include "algorithms/clique_approximate.h"

int main() {
    CSRGraph* graph = csr_graph_new_from_path("data/input/sample");
    // CSRGraph* k_core = compute_k_core(graph, 3);

    // csr_graph_delete(&k_core);
    csr_graph_delete(&graph);

    return EXIT_SUCCESS;
}