#include "clique_approximate.h"

void _random_sampling_k_cliques(CSRGraph* graph, int* degrees, int k, OrderedSet* clique, OrderedSet* frontier, OrderedSet* processed) {
    if (clique->size == k) {
        ordered_set_print(clique, true);
        return;
    }

    if (frontier->size == 0) {
        return;
    }

    int pivot_vertex = -1;
    OrderedSet* union_set = ordered_set_union(frontier, processed);
    for (int i = 0; i < union_set->size; i++) {
        if (pivot_vertex == -1 || degrees[union_set->elements[i]] > degrees[pivot_vertex]) {
            pivot_vertex = union_set->elements[i];
        }
    }

    OrderedSet* pivot_u_neighbors = csr_graph_get_neighbors(graph, pivot_vertex);
    OrderedSet* non_pivot_frontier = ordered_set_difference(frontier, pivot_u_neighbors);

    for (int i = 0; i < non_pivot_frontier->size; i++) {
        int vertex_v = non_pivot_frontier->elements[i];
        OrderedSet* v_neighbors = csr_graph_get_neighbors(graph, vertex_v);
        OrderedSet* next_frontier = ordered_set_intersection(frontier, v_neighbors);
        OrderedSet* next_processed = ordered_set_intersection(processed, v_neighbors);

        ordered_set_insert(clique, vertex_v);
        _random_sampling_k_cliques(graph, degrees, k, clique, next_frontier, next_processed);
        ordered_set_remove(clique, vertex_v);

        ordered_set_delete(&next_frontier);
        ordered_set_delete(&v_neighbors);

        ordered_set_remove(frontier, vertex_v);
        ordered_set_insert(processed, vertex_v);
    }

    ordered_set_delete(&non_pivot_frontier);
    ordered_set_delete(&pivot_u_neighbors);
    ordered_set_delete(&union_set);
}

/**
 * @brief Approximate k-cliques using the Bron-Kerbosch algorithm as
 * inspiration. This algorithm is not guaranteed to find all k-cliques.
 *
 * @param graph
 * @param k
 * @return OrderedSet**
 */
OrderedSet** _approximate_k_cliques_bron_kerbosch(CSRGraph* graph, int k) {
    assert(graph != NULL);
    assert(k >= 1);

    int* degrees = csr_graph_get_degrees(graph);
    int* vertex_ids = array_generate_sequence(0, graph->num_vertices - 1, 1);
    int* vertex_visit_order = array_shuffle(vertex_ids, graph->num_vertices, true);

    OrderedSet* clique = ordered_set_new(k);
    OrderedSet* frontier = ordered_set_new(graph->num_vertices);
    OrderedSet* processed = ordered_set_new(5);

    for (int i = 0; i < graph->num_vertices; i++) {
        int idx_vertex = vertex_visit_order[i];
        ordered_set_insert(frontier, idx_vertex);
    }

    _random_sampling_k_cliques(graph, degrees, k, clique, frontier, processed);

    free(degrees);
    free(vertex_ids);
    ordered_set_delete(&clique);
    ordered_set_delete(&frontier);
    ordered_set_delete(&processed);

    return NULL;
}

void _random_neighbor_visit(CSRGraph* graph, int* degrees, int k, int m, OrderedSet* clique, OrderedSet* neighbors) {
    if (clique->size == k) {
        ordered_set_print(clique, true);
        return;
    }

    if (neighbors->size == 0) {
        return;
    }

    for (int i = 0; i < neighbors->size; i++) {
        int vertex = neighbors->elements[i];
        OrderedSet* vertex_neighbors = csr_graph_get_neighbors(graph, vertex);

        OrderedSet* intersection = ordered_set_intersection(neighbors, vertex_neighbors);

        ordered_set_insert(clique, vertex);
        _random_neighbor_visit(graph, degrees, k, m, clique, intersection);
        ordered_set_remove(clique, vertex);

        ordered_set_delete(&intersection);
        ordered_set_delete(&vertex_neighbors);
    }
}

void _approximate_k_cliques(CSRGraph* graph, int k, int m) {
    int* degrees = csr_graph_get_degrees(graph);
    int* vertex_ids = array_generate_sequence(0, graph->num_vertices - 1, 1);
    int* vertex_visit_order = array_shuffle(vertex_ids, graph->num_vertices, true);
    OrderedSet* clique = ordered_set_new(k);

    for (int i = 0; i < graph->num_vertices; i++) {
        int vertex = vertex_visit_order[i];
        OrderedSet* neighbors = csr_graph_get_neighbors(graph, vertex);
        int* shuffled_neighbors = array_shuffle(neighbors->elements, neighbors->size, false);

        for (int j = 0; j < m && j < neighbors->size; j++) {
            int neighbor = shuffled_neighbors[j];
            OrderedSet* subgraph_vertices = csr_graph_get_neighbors(graph, neighbor);

            ordered_set_insert(clique, vertex);
            _random_neighbor_visit(graph, degrees, k, m, clique, subgraph_vertices);
            ordered_set_remove(clique, vertex);

            ordered_set_delete(&subgraph_vertices);
        }

        free(shuffled_neighbors);
        ordered_set_delete(&neighbors);
    }

    free(vertex_ids);
    ordered_set_delete(&clique);
}

OrderedSet** approximate_k_cliques(CSRGraph* graph, int k) {
    _approximate_k_cliques(graph, k, 1);
    return NULL;
}
