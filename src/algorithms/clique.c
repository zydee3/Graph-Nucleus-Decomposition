#include "clique.h"

GenericLinkedList* compute_one_clique(Graph* graph) {
    GenericLinkedList* one_cliques = generic_linked_list_new(ordered_set_generic_copy, ordered_set_generic_delete, ordered_set_generic_is_equal, ordered_set_generic_print);
    for (vertex i = 0; i < graph->num_vertices; i++) {
        OrderedSet* clique = ordered_set_new(1);
        ordered_set_insert(clique, i);
        generic_linked_list_insert(one_cliques, clique);
    }

    return one_cliques;
}

GenericLinkedList* compute_two_clique(Graph* graph) {
    GenericLinkedList* two_cliques = generic_linked_list_new(ordered_set_generic_copy, ordered_set_generic_delete, ordered_set_generic_is_equal, ordered_set_generic_print);

    int* ptr_rows = graph->adjacency_matrix->ptr_rows;
    int* idx_cols = graph->adjacency_matrix->idx_cols;

    for (int row_idx = 0; row_idx < graph->num_vertices; row_idx++) {
        int row_begin_read = ptr_rows[row_idx];
        int row_end_read = ptr_rows[row_idx + 1];

        for (int idx_nnz = row_begin_read; idx_nnz < row_end_read; idx_nnz++) {
            int col_idx = idx_cols[idx_nnz];

            if (graph->is_directed == false && col_idx < row_idx) {
                continue;
            }

            OrderedSet* clique = ordered_set_new(2);
            ordered_set_insert(clique, row_idx);
            ordered_set_insert(clique, col_idx);
            generic_linked_list_insert(two_cliques, clique);
        }
    }

    return two_cliques;
}