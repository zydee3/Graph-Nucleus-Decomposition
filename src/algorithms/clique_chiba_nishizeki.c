#include "clique_chiba_nishizeki.h"

void enumerate_k_cliques_chiba_nishizeki(CSRGraph* graph, int k, int current_vertex, int depth, OrderedSet* clique, OrderedSet* candidates) {
    if (depth == k) {
        ordered_set_print(clique, true);
        return;
    }

    OrderedSet* neighbors = csr_graph_get_neighbors(graph, current_vertex);
    OrderedSet* new_candidates = ordered_set_intersection(candidates, neighbors);

    for (int i = 0; i < new_candidates->size; i++) {
        int candidate = new_candidates->elements[i];
        if (candidate > current_vertex) {
            ordered_set_insert(clique, candidate);
            enumerate_k_cliques_chiba_nishizeki(graph, k, candidate, depth + 1, clique, new_candidates);
            ordered_set_remove(clique, candidate);
        }
    }

    ordered_set_delete(&neighbors);
    ordered_set_delete(&new_candidates);
}

OrderedSet** find_k_cliques(CSRGraph* graph, int k) {
    assert(graph != NULL);
    assert(k >= 1);

    OrderedSet* clique = ordered_set_new(k);

    for (int v = 0; v < graph->num_vertices; v++) {
        ordered_set_insert(clique, v);
        OrderedSet* candidates = csr_graph_get_neighbors(graph, v);
        enumerate_k_cliques_chiba_nishizeki(graph, k, v, 1, clique, candidates);
        ordered_set_remove(clique, v);
        ordered_set_delete(&candidates);
    }

    ordered_set_delete(&clique);

    return NULL;
}