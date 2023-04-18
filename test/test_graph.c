#include "test_graph.h"

// Helper functions

/**
 * @brief Checks if the param list contains an ordered set of integers
 * starting at param start.
 *
 * @param list The list to check.
 * @param start The first value in the sequence to check.
 * @return true If the list represents an ordered set of integers
 * starting at param start.
 * @return false otherwise.
 */
bool _has_valid_sequence_gll(LinkedList* list, int start) {
    int counter = start;
    Node* node = list->head;

    while (node != NULL) {
        if (node->data != counter) {
            return false;
        }

        counter++;
        node = node->next;
    }

    return true;
}

/**
 * @brief Adds param num_verticies verticies to the graph.
 *
 * @param graph The graph to add the verticies to.
 * @param num_vertices The number of verticies to add.
 */
void _add_vertices(Graph* graph, int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        graph_add_vertex(graph, i);
    }
}

/**
 * @brief Adds param num_edges edges to each vertex in the graph.
 *
 * @param graph The graph to add the edges to.
 * @param num_vertices The number of verticies in the graph.
 * @param num_edges The number of edges to add to each vertex.
 */
void _add_edges(Graph* graph, int num_vertices, int num_edges) {
    for (int source_vertex = 0; source_vertex < num_vertices; source_vertex++) {
        for (int target_vertex = source_vertex; target_vertex < source_vertex + num_edges; target_vertex++) {
            graph_add_edge(graph, source_vertex, target_vertex % num_vertices, 0);
        }
    }
}

/**
 * @brief Generates a graph with 5 verticies and 3 edges.
 *
 * @param is_directed Whether the graph is directed or not.
 * @return Graph* The generated graph.
 */
Graph* _generate_simple_graph(bool is_directed) {
    const int num_verticies = 5;
    const int num_edges = 3;

    Graph* graph = graph_new(is_directed);
    _add_vertices(graph, num_verticies);
    _add_edges(graph, num_verticies, num_edges);
    return graph;
}

// Graph test functions for directed and undirected variant calls

/**
 * @brief Tests the graph_add_vertex function.
 *
 * This function serves as a template for both the undirected and
 * directed graph_remove_vertex testing functions. The test creates a
 * graph with 5 verticies. It then iteratively removes verticies one
 * by one and checks if the removed vertex is no longer in the graph.
 * The test passes if the list contains only non-removed verticies
 * relative to the iteration.
 *
 * @param function_name The name of the function calling this.
 * @param is_directed Whether the graph is directed or not.
 */
void _test_undirected_graph_remove_vertex(const char* function_name, bool is_directed) {
    Graph* graph = _generate_simple_graph(is_directed);

    int num_vertices = graph->num_vertices;
    bool has_valid_vertices = true;

    for (int i = 0; i < num_vertices; i++) {
        graph_remove_vertex(graph, i);
        has_valid_vertices = has_valid_vertices && graph->num_vertices == num_vertices - i - 1;
        has_valid_vertices = has_valid_vertices && _has_valid_sequence_gll(graph->vertices, i + 1);
    }

    print_test_result(__FILE__, function_name, has_valid_vertices);
    graph_delete((void**)&graph);
}

/**
 * @brief Tests the graph_add_edge function.
 *
 * This function serves as a template for both the undirected and
 * directed graph_add_edge testing functions. The test creates a
 * graph with 5 verticies and 3 edges. It then iteratively checks if
 * the graph contains the correct edges. The test passes if the graph
 * contains the expected edges.
 *
 * @param function_name The name of the function calling this.
 * @param is_directed Whether the graph is directed or not.
 */
void _test_graph_add_edge(const char* function_name, bool is_directed) {
    Graph* graph = _generate_simple_graph(is_directed);

    int num_vertices = graph->num_vertices;
    int num_edges = graph->num_edges / num_vertices;

    bool has_valid_edges = true;
    for (int source_vertex = 0; source_vertex < num_vertices; source_vertex++) {
        for (int target_vertex = source_vertex; target_vertex < source_vertex + num_edges; target_vertex++) {
            if (source_vertex == target_vertex) {
                continue;
            }

            int target_vertex_value = target_vertex % num_vertices;
            Node* target_edge = graph_get_edge(graph, source_vertex, target_vertex_value);
            has_valid_edges = has_valid_edges && target_edge != NULL;
            has_valid_edges = has_valid_edges && target_edge->data == target_vertex_value;
        }
    }

    print_test_result(__FILE__, function_name, has_valid_edges);
    graph_delete((void**)&graph);
}

/**
 * @brief Tests the graph_remove_edge function.
 *
 * This function serves as a template for both the undirected and
 * directed graph_remove_edge testing functions. The test creates a
 * graph with 5 verticies and 3 edges. It then iteratively removes
 * edges one by one and checks if the removed edge is no longer in
 * the graph. The test passes if the graph contains only non-removed
 * edges relative to the iteration.
 *
 * @param function_name The name of the function calling this.
 * @param is_directed Whether the graph is directed or not.
 */
void _test_undirected_graph_remove_edge(const char* function_name, bool is_directed) {
    Graph* graph = _generate_simple_graph(is_directed);

    int num_vertices = graph->num_vertices;
    int num_edges = graph->num_edges / num_vertices;

    bool has_valid_edges = true;

    for (int source_vertex = 0; source_vertex < num_vertices; source_vertex++) {
        for (int target_vertex = source_vertex; target_vertex < source_vertex + num_edges; target_vertex++) {
            if (source_vertex == target_vertex) {
                continue;
            }

            int target_vertex_value = target_vertex % num_vertices;
            graph_remove_edge(graph, source_vertex, target_vertex_value);

            Node* target_edge = graph_get_edge(graph, source_vertex, target_vertex_value);
            has_valid_edges = has_valid_edges && target_edge == NULL;
        }
    }

    print_test_result(__FILE__, function_name, has_valid_edges);
    graph_delete((void**)&graph);
}

// Graph test functions

/**
 * @brief Tests the graph_new function.
 *
 * The test creates a graph with 5 verticies and 3 edges. It then
 * checks if the verticies that were added are correct. The test
 * passes if the graph contains the expected verticies.
 */
void test_graph_add_vertex() {
    Graph* graph = _generate_simple_graph(false);
    bool has_valid_vertices = graph->num_vertices == 5 && _has_valid_sequence_gll(graph->vertices, 0);
    print_test_result(__FILE__, __func__, has_valid_vertices);
    graph_delete((void**)&graph);
}

/**
 * @brief Tests the graph_remove_vertex function.
 *
 * This function calls the template function
 * _test_graph_remove_vertex. The test creates a graph with 5
 * verticies. It then iteratively removes verticies one by one and
 * checks if the removed vertex is no longer in the graph. The test
 * passes if the list contains only non-removed verticies relative to
 * the iteration.
 */
void test_undirected_graph_remove_vertex() {
    _test_undirected_graph_remove_vertex(__func__, false);
}

/**
 * @brief Tests the graph_remove_vertex function.
 *
 * This function calls the template function
 * _test_graph_remove_vertex. The test creates a graph with 5
 * verticies. It then iteratively removes verticies one by one and
 * checks if the removed vertex is no longer in the graph. The test
 * passes if the list contains only non-removed verticies relative to
 * the iteration.
 */
void test_directed_graph_remove_vertex() {
    _test_undirected_graph_remove_vertex(__func__, true);
}

/**
 * @brief Tests the graph_get_edge function.
 *
 * The test creates a graph with 5 verticies and 3 edges. It then
 * iteratively checks if the graph contains the correct edges. A
 * correct edge is an edge (u,v) over verticies u,v with weight w
 * where u,v,w are expected. The test passes if the graph contains the
 * expected edges.
 *
 */
void test_graph_get_edge() {
    Graph* graph = _generate_simple_graph(false);

    int num_vertices = graph->num_vertices;
    int num_edges = graph->num_edges / num_vertices;

    bool has_valid_edges = true;
    for (int source_vertex = 0; source_vertex < num_vertices; source_vertex++) {
        for (int target_vertex = source_vertex; target_vertex < source_vertex + num_edges; target_vertex++) {
            if (source_vertex == target_vertex) {
                continue;
            }

            int target_vertex_value = target_vertex % num_vertices;
            Node* target_edge = graph_get_edge(graph, source_vertex, target_vertex_value);
            has_valid_edges = has_valid_edges && target_edge != NULL;  // && target_edge->data == target_vertex % num_vertices;
            has_valid_edges = has_valid_edges && target_edge->data == target_vertex_value;
        }
    }

    has_valid_edges = has_valid_edges && graph_get_edge(graph, 1011, 1011) == NULL;

    print_test_result(__FILE__, __func__, has_valid_edges);
    graph_delete((void**)&graph);
}

/**
 * @brief Tests the graph_add_edge function.
 *
 * This function calls the template function test_graph_add_edge. This
 * function serves as a template for both the undirected and directed
 * graph_add_edge testing functions. The test creates a graph with 5
 * verticies and 3 edges. It then iteratively checks if the graph
 * contains the correct edges. The test passes if the graph contains
 * the expected edges.
 */
void test_undirected_graph_add_edge() {
    _test_graph_add_edge(__func__, false);
}

/**
 * @brief Tests the graph_add_edge function.
 *
 * This function calls the template function test_graph_add_edge. This
 * function serves as a template for both the undirected and directed
 * graph_add_edge testing functions. The test creates a graph with 5
 * verticies and 3 edges. It then iteratively checks if the graph
 * contains the correct edges. The test passes if the graph contains
 * the expected edges.
 */
void test_directed_graph_add_edge() {
    _test_graph_add_edge(__func__, true);
}

/**
 * @brief Tests the graph_remove_edge function.
 *
 * This function calls the template function
 * _test_undirected_graph_remove_edge. The test creates a graph with
 * 5 verticies and 3 edges. It then iteratively removes edges one by
 * one and checks if the removed edge is no longer in the graph. The
 * test passes if the list contains only non-removed edges relative to
 * the iteration.
 */
void test_undirected_graph_remove_edge() {
    _test_undirected_graph_remove_edge(__func__, false);
}

/**
 * @brief Tests the graph_remove_edge function.
 *
 * This function calls the template function
 * _test_undirected_graph_remove_edge. The test creates a graph with
 * 5 verticies and 3 edges. It then iteratively removes edges one by
 * one and checks if the removed edge is no longer in the graph. The
 * test passes if the list contains only non-removed edges relative to
 * the iteration.
 */
void test_directed_graph_remove_edge() {
    _test_undirected_graph_remove_edge(__func__, true);
}

void test_graph() {
    test_graph_add_vertex();
    test_undirected_graph_remove_vertex();
    test_directed_graph_remove_vertex();
    test_graph_get_edge();
    test_undirected_graph_add_edge();
    test_directed_graph_add_edge();
    test_undirected_graph_remove_edge();
    test_directed_graph_remove_edge();
}