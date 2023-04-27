#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "algorithms/clique_approximate.h"
#include "algorithms/clique_chiba_nishizeki.h"
#include "algorithms/clique_expansion.h"
#include "algorithms/core.h"
#include "collections/csr_graph.h"
#include "collections/generic_linked_list.h"

int main() {
    clock_t start_time = clock();

    CSRGraph* graph = csr_graph_new_from_path("data/input/facebook_caltech");
    // CSRGraph* graph = csr_graph_new_from_path("data/input/ca-netscience");

    int k = 3;

    // Find k-cliques
    GenericLinkedList* k_cliques = find_k_cliques(graph, k);
    printf("\nFound %d %d-cliques:\n", k_cliques->size, k);
    // generic_linked_list_print(k_cliques);

    // Expand k-cliques to (k + 1)-cliques
    GenericLinkedList* expanded_k_cliques = expand_cliques(graph, k_cliques);
    printf("Found %d %d-cliques:\n", expanded_k_cliques->size, k + 1);
    // generic_linked_list_print(expanded_k_cliques);

    clock_t end_time = clock();

    // // Clean
    generic_linked_list_delete(&k_cliques);
    generic_linked_list_delete(&expanded_k_cliques);
    csr_graph_delete(&graph);

    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Elapsed time: %.2f seconds\n", elapsed_time);

    return EXIT_SUCCESS;
}