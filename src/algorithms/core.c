#include "core.h"

/**
 * @brief Sets the degree of a vertex to zero and queues it for
 * removal.
 *
 * @param vertex_id
 * @param k
 * @param vertex_degrees
 * @param removed_vertices
 * @param queue
 */
static inline void _remove_less_than_k(int vertex_id, int k, int* vertex_degrees, bool* removed_vertices, Queue* queue) {
    assert(vertex_degrees != NULL);
    if (vertex_degrees[vertex_id] >= k) {
        return;
    }

    if (removed_vertices[vertex_id] == true) {
        return;
    }

    printf("Removing %d (Degree: %d)\n", vertex_id, vertex_degrees[vertex_id]);

    vertex_degrees[vertex_id] = 0;
    queue_enqueue(queue, vertex_id);
    removed_vertices[vertex_id] = true;
}

/**
 * @brief Decreaments the degree of the vertex and queues it for
 * removal.
 *
 * Once the vertex is queued for removal, it will be removed from the
 * graph and it's neighbors will be checked to see if they should be
 * removed based on their degree.
 *
 * @param vertex_id
 * @param k
 * @param vertex_degrees
 * @param removed_vertices
 * @param queue
 */
static inline void _decrease_and_remove_less_than_k(int vertex_id, int k, int* vertex_degrees, bool* removed_vertices, Queue* queue) {
    if (removed_vertices[vertex_id] == true) {
        return;
    }

    vertex_degrees[vertex_id]--;

    _remove_less_than_k(vertex_id, k, vertex_degrees, removed_vertices, queue);
}

/**
 * @brief Finds the vertices not present in the k-core of the param
 * graph and returns a boolean array.
 *
 * @param graph The graph to find the k-core of.
 * @param k The min degree of any vertex in the k-core of param graph.
 * @return bool* A boolean array indexed by vertices in the graph
 * where each index is true if the vertex is not in the k-core or
 * false otherwise.
 */
bool* get_vertices_not_in_k_core(CSRGraph* graph, int k) {
    // Record for each vertex if it has been removed.
    bool* removed_vertices = calloc(graph->num_vertices, sizeof(bool));

    // Initial degree of each vertex.
    int* vertex_degrees = csr_graph_get_degrees(graph);

    // Queue of vertices to be removed. Need to visit each neighbor.
    Queue* queue = queue_new(graph->num_vertices);

    // Enqueue all initial vertices with degree less than k.
    for (int i = 0; i < graph->num_vertices; i++) {
        if (vertex_degrees[i] < k && removed_vertices[i] == false) {
            queue_enqueue(queue, i);
            vertex_degrees[i] = 0;
            removed_vertices[i] = true;
        }
        _remove_less_than_k(i, k, vertex_degrees, removed_vertices, queue);
    }

    CompressedSparseRow* adjacency_matrix = graph->adjacency_matrix;
    int* ptr_rows = adjacency_matrix->ptr_rows;
    int* idx_cols = adjacency_matrix->idx_cols;

    // Continuously remove vertices with degree less than k, adding
    // their neighbors to the queue if the neighbor's degree becomes
    // less than k after the removal.
    while (queue->size > 0) {
        int removed_vertex = queue_dequeue(queue);
        int idx_begin_read = ptr_rows[removed_vertex];
        int idx_end_read = ptr_rows[removed_vertex + 1];

        for (int i = idx_begin_read; i < idx_end_read; i++) {
            int neighbor = idx_cols[i];
            vertex_degrees[neighbor]--;

            if (vertex_degrees[neighbor] < k && removed_vertices[neighbor] == false) {
                queue_enqueue(queue, neighbor);
                vertex_degrees[neighbor] = 0;
                removed_vertices[neighbor] = true;
            }
        }
    }

    free(vertex_degrees);
    queue_delete(&queue);

    return removed_vertices;
}

/**
 * @brief Computes the k-core of the param graph and returns a new
 * graph with only the vertices and edges in the k-core.
 *
 * @param graph The graph to find the k-core of.
 * @param k The min degree of any vertex in the k-core of param graph.
 * @return CSRGraph* The computed k-core of param graph.
 */
CSRGraph* compute_k_core(CSRGraph* graph, int k) {
    assert(graph != NULL);
    assert(k > 0);

    // Record for each vertex if it has been removed.
    bool* removed_vertices = get_vertices_not_in_k_core(graph, k);

    // Reduce the graph to only the vertices that are in the k-core.
    CSRGraph* k_core = csr_graph_reduce(graph, removed_vertices);

    free(removed_vertices);

    return k_core;
}