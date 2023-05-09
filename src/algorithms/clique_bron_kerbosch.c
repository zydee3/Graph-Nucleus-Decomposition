#include "clique_bron_kerbosch.h"

void _compute_all_k_cliques(Graph* graph, int* degrees, int min_k, OrderedSet* clique, OrderedSet* frontier, OrderedSet* processed) {
    if (clique->size >= min_k) {
        ordered_set_print(clique, true);
    }

    if (frontier->size == 0) {
        return;
    }

    vertex pivot = -1;
    OrderedSet* union_set = ordered_set_union(frontier, processed);
    for (int i = 0; i < union_set->size; i++) {
        if (pivot == -1 || degrees[union_set->elements[i]] > degrees[pivot]) {
            pivot = union_set->elements[i];
        }
    }

    OrderedSet* pivot_neighbors = graph_get_neighbors(graph, pivot);
    OrderedSet* non_pivot_frontier = ordered_set_difference(frontier, pivot_neighbors);

    for (int i = 0; i < non_pivot_frontier->size; i++) {
        int vertex_v = non_pivot_frontier->elements[i];
        OrderedSet* v_neighbors = graph_get_neighbors(graph, vertex_v);
        OrderedSet* next_frontier = ordered_set_intersection(frontier, v_neighbors);
        OrderedSet* next_processed = ordered_set_intersection(processed, v_neighbors);

        ordered_set_insert(clique, vertex_v);
        _compute_all_k_cliques(graph, degrees, min_k, clique, next_frontier, next_processed);
        ordered_set_remove(clique, vertex_v);

        ordered_set_delete(&next_frontier);
        ordered_set_delete(&v_neighbors);

        ordered_set_remove(frontier, vertex_v);
        ordered_set_insert(processed, vertex_v);
    }

    ordered_set_delete(&non_pivot_frontier);
    ordered_set_delete(&pivot_neighbors);
}

OrderedSet** find_k_cliques_bron_kerbosch(Graph* graph, int min_k) {
    assert(graph != NULL);

    OrderedSet** k_cliques = NULL;

    int* degrees = graph_get_degrees(graph);
    OrderedSet* clique = ordered_set_new(min_k);

    OrderedSet* frontier = ordered_set_new(graph->num_vertices);
    OrderedSet* processed = ordered_set_new(5);

    for (int idx_vertex = 0; idx_vertex < graph->num_vertices; idx_vertex++) {
        ordered_set_insert(frontier, idx_vertex);
    }

    _compute_all_k_cliques(graph, degrees, min_k, clique, frontier, processed);

    return k_cliques;
}