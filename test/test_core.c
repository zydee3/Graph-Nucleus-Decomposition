#include "test_core.h"

void test_core() {
    Graph* undirected_graph = graph_new_from_path("data/input/sample");
    Graph* k3_core = compute_k_core(undirected_graph, 3);

    int expected_row_ptrs[] = {0, 3, 8, 11, 15, 21, 28, 31, 36, 39, 43, 46};
    int expected_col_idxs[] = {1, 3, 4, 0, 2, 4, 5, 9, 1, 5, 9, 0, 4, 6, 7, 0, 1, 3, 5, 6, 7, 1, 2, 4, 7, 8, 9, 10, 3, 4, 7, 3, 4, 5, 6, 8, 5, 7, 10, 1, 2, 5, 10, 5, 8, 9};

    int len_row_ptrs = sizeof(expected_row_ptrs) / sizeof(expected_row_ptrs[0]);
    int len_col_idxs = sizeof(expected_col_idxs) / sizeof(expected_col_idxs[0]);

    bool is_passing = true;
    is_passing &= array_is_equal(k3_core->adjacency_matrix->ptr_rows, expected_row_ptrs, k3_core->num_vertices + 1, len_row_ptrs);
    is_passing &= array_is_equal(k3_core->adjacency_matrix->idx_cols, expected_col_idxs, k3_core->adjacency_matrix->num_nnzs, len_col_idxs);

    graph_delete(&k3_core);
    graph_delete(&undirected_graph);

    print_test_result(__FILE__, __func__, is_passing);
}