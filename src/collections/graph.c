#include "graph.h"

// Helper functions

bool _add_out_edge(Graph* graph, Node* source_vertex, Node* target_vertex, int edge_weight) {
    if (source_vertex->children == NULL) {
        source_vertex->children = linked_list_new();
    }

    if (linked_list_insert(source_vertex->children, target_vertex->data, edge_weight)) {
        graph->num_edges++;
        return true;
    }

    return false;
}

void _remove_directed_in_edges(Graph* graph, Node* target_vertex) {
    Node* source_vertex = graph->vertices->head;

    while (source_vertex != NULL) {
        if (linked_list_remove(source_vertex->children, target_vertex->data)) {
            graph->num_edges--;
        }

        source_vertex = source_vertex->next;
    }
}

void _remove_undirected_in_edges(Graph* graph, Node* target_vertex) {
    if (target_vertex->children == NULL) {
        return;
    }

    Node* ref_source_vertex = target_vertex->children->head;

    while (ref_source_vertex != NULL) {
        Node* source_vertex = graph_get_vertex(graph, ref_source_vertex->data);
        if (linked_list_remove(source_vertex->children, target_vertex->data)) {
            graph->num_edges--;
        }

        ref_source_vertex = ref_source_vertex->next;
    }
}

// Graph functions

Graph* graph_new(bool is_directed) {
    Graph* graph = malloc(sizeof(Graph));
    graph->is_directed = is_directed;
    graph->num_vertices = 0;
    graph->num_edges = 0;
    graph->vertices = linked_list_new();

    return graph;
}

void* graph_copy(void* ref_graph) {
    assert(ref_graph != NULL);

    Graph* graph = (Graph*)ref_graph;
    Graph* copy = graph_new(graph->is_directed);

    copy->vertices = linked_list_copy(graph->vertices);
    copy->num_vertices = graph->num_vertices;
    copy->num_edges = graph->num_edges;

    return copy;
}

void graph_delete(void** ptr_graph) {
    assert(ptr_graph != NULL && *ptr_graph != NULL);

    Graph* graph = (Graph*)*ptr_graph;
    linked_list_delete(&graph->vertices);

    free(*ptr_graph);
    *ptr_graph = NULL;
}

bool graph_compare(void* graph_a, void* graph_b) {
    assert(graph_a != NULL && graph_b != NULL);

    Graph* a = (Graph*)graph_a;
    Graph* b = (Graph*)graph_b;
    return a->is_directed == b->is_directed && a->num_vertices == b->num_vertices && a->num_edges == b->num_edges && linked_list_compare(a->vertices, b->vertices);
}

void graph_print(void* ref_graph) {
    assert(ref_graph != NULL);

    Graph* graph = (Graph*)ref_graph;
    printf("Graph: num_edges(%s): %d, vertices(%d): ", graph->is_directed ? "directed" : "undirected", graph->num_edges, graph->num_vertices);
    linked_list_print(graph->vertices);
}

bool graph_add_vertex(Graph* graph, int vertex_value) {
    assert(graph != NULL);

    if (linked_list_insert(graph->vertices, vertex_value, 0)) {
        graph->num_vertices++;
        return true;
    }

    return false;
}

bool graph_remove_vertex(Graph* graph, int vertex_value) {
    assert(graph != NULL);

    Node* target_vertex = graph_get_vertex(graph, vertex_value);
    if (target_vertex == NULL) {
        return false;
    }

    if (graph->is_directed) {
        _remove_directed_in_edges(graph, target_vertex);
    } else {
        _remove_undirected_in_edges(graph, target_vertex);
    }

    linked_list_remove(graph->vertices, vertex_value);

    graph->num_vertices--;

    return true;
}

Node* graph_get_vertex(Graph* graph, int vertex_value) {
    assert(graph != NULL);

    return linked_list_get(graph->vertices, vertex_value);
}

bool graph_add_edge(Graph* graph, int vertex_a_value, int vertex_b_value, int edge_weight) {
    assert(graph != NULL);

    if (vertex_a_value == vertex_b_value) {
        return false;
    }

    Node* vertex_a = graph_get_vertex(graph, vertex_a_value);
    Node* vertex_b = graph_get_vertex(graph, vertex_b_value);

    if (vertex_a == NULL || vertex_b == NULL) {
        return false;
    }

    // int initial_num_edges = graph->num_edges;
    // int expected_updated_num_edges = initial_num_edges + 1 + (graph->is_directed ? 0 : 1);

    if (!_add_out_edge(graph, vertex_a, vertex_b, edge_weight)) {
        return false;
    }

    if (!graph->is_directed) {
        if (!_add_out_edge(graph, vertex_b, vertex_a, edge_weight)) {
            return false;
        }
    }

    // assert(graph->num_edges == expected_updated_num_edges);
    return true;
}

bool graph_remove_edge(Graph* graph, int vertex_a_value, int vertex_b_value) {
    assert(graph != NULL);

    Node* vertex_a = graph_get_vertex(graph, vertex_a_value);
    Node* vertex_b = graph_get_vertex(graph, vertex_b_value);

    if (vertex_a == NULL || vertex_b == NULL) {
        return false;
    }

    // int initial_num_edges = graph->num_edges;
    // int expected_updated_num_edges = initial_num_edges - 1 - (graph->is_directed ? 0 : 1);

    if (linked_list_remove(vertex_a->children, vertex_b_value)) {
        graph->num_edges--;
    } else {
        return false;
    }

    if (!graph->is_directed) {
        if (linked_list_remove(vertex_b->children, vertex_a_value)) {
            graph->num_edges--;
        } else {
            return false;
        }
    }

    // assert(graph->num_edges == expected_updated_num_edges);
    return true;
}

Node* graph_get_edge(Graph* graph, int vertex_a_value, int vertex_b_value) {
    assert(graph != NULL);

    Node* vertex_a = graph_get_vertex(graph, vertex_a_value);
    Node* vertex_b = graph_get_vertex(graph, vertex_b_value);

    if (vertex_a == NULL || vertex_b == NULL) {
        return NULL;
    }

    return linked_list_get(vertex_a->children, vertex_b_value);
}
