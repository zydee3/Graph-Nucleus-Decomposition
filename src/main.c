#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "algorithms/clique_approximate.h"
#include "algorithms/clique_chiba_nishizeki.h"
#include "algorithms/core.h"
#include "collections/csr_graph.h"
#include "collections/generic_linked_list.h"

int main() {
    CSRGraph* graph = csr_graph_new_from_path("data/input/sample");

    GenericLinkedList* cliques = find_k_cliques(graph, 3);

    printf("Found %d cliques:\n", cliques->size);
    generic_linked_list_print(cliques);

    generic_linked_list_delete(&cliques);
    csr_graph_delete(&graph);

    return EXIT_SUCCESS;
}