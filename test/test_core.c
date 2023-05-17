#include "test_core.h"

void test_core() {
    Graph* undirected_graph = graph_new_from_file("data/input/sample");

    bool expected_removed_k3[] = {false, false, false, false, false, false, false, false, false, false, false, true, true, true, true};
    bool expected_removed_k4[] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};

    bool is_passing = true;

    bool* removed_vertices_k3 = get_vertices_not_in_k_core(undirected_graph, 3);
    is_passing = is_passing && memcmp(removed_vertices_k3, expected_removed_k3, undirected_graph->num_vertices * sizeof(bool)) == 0;

    bool* removed_vertices_k4 = get_vertices_not_in_k_core(undirected_graph, 4);
    is_passing = is_passing && memcmp(removed_vertices_k4, expected_removed_k4, undirected_graph->num_vertices * sizeof(bool)) == 0;

    free(removed_vertices_k3);
    free(removed_vertices_k4);
    graph_delete(&undirected_graph);

    print_test_result(__FILE__, __func__, is_passing);
}