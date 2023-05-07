#include "clique_chiba_nishizeki.h"

void enumerate_k_cliques_chiba_nishizeki(CSRGraph* graph, int target_k, int current_k, int current_vertex, OrderedSet* clique, OrderedSet* candidates, GenericLinkedList* cliques) {
    assert(graph != NULL);
    assert(graph->adjacency_matrix != NULL);
    assert(graph->adjacency_matrix->is_set);
    assert(current_vertex >= 0);
    assert(current_vertex < graph->num_vertices);

    if (current_k == target_k) {
        generic_linked_list_insert(cliques, ordered_set_generic_copy(clique));
        return;
    }

    OrderedSet* neighbors = csr_graph_get_neighbors(graph, current_vertex);
    OrderedSet* new_candidates = ordered_set_intersection(candidates, neighbors);

    for (int i = 0; i < new_candidates->size; i++) {
        int candidate = new_candidates->elements[i];
        if (candidate > current_vertex) {
            ordered_set_insert(clique, candidate);
            enumerate_k_cliques_chiba_nishizeki(graph, target_k, current_k + 1, candidate, clique, new_candidates, cliques);
            ordered_set_remove(clique, candidate);
        }
    }

    ordered_set_delete(&neighbors);
    ordered_set_delete(&new_candidates);
}

GenericLinkedList* find_k_cliques(CSRGraph* graph, int k) {
    assert(graph != NULL);
    assert(k >= 1);

    bool* is_vertex_removed = get_vertices_not_in_k_core(graph, k);

    GenericLinkedList* cliques = generic_linked_list_new(ordered_set_generic_copy, ordered_set_generic_delete, ordered_set_generic_is_equal, ordered_set_generic_print);
    OrderedSet* clique = ordered_set_new(k);

    for (int v = 0; v < graph->num_vertices; v++) {
        if (is_vertex_removed[v] == true) {
            continue;
        }

        ordered_set_insert(clique, v);
        OrderedSet* candidates = csr_graph_get_neighbors(graph, v);
        enumerate_k_cliques_chiba_nishizeki(graph, k, 1, v, clique, candidates, cliques);
        ordered_set_remove(clique, v);
        ordered_set_delete(&candidates);
    }

    ordered_set_delete(&clique);
    free(is_vertex_removed);

    return cliques;
}