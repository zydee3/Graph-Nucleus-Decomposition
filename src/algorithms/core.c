#include "core.h"

/**
 * This class contains the partial implementation of the k-core
 * algorithm. The k-core algorithm finds the k-core of a graph, which
 * is the subgraph of the graph where each vertex has degree at least
 * k. The provided function returns a boolean array where each index
 * is true if the vertex is not in the k-core or false otherwise.
 *
 * It is often unnecessary to generate a new csr graph for the k-core
 * algorithm as knowing which vertices are in the k-core is sufficient
 * for many applications. This class provides a function to find the
 * vertices not in the k-core of a graph.
 */

/**
 * @brief Sets the degree of a vertex to zero and queues it for
 * removal.
 *
 * Once the vertex is queued for removal, it will be removed from the
 * graph and it's neighbors will be checked to see if they should be
 * removed based on their updateddegree.
 *
 * @param vertex_id
 * @param k
 * @param vertex_degrees
 * @param removed_vertices
 * @param queue
 *
 *
 */
static inline void _remove_less_than_k(vertex u, int k, int* vertex_degrees, bool* removed_vertices, Queue* queue) {
    assert(vertex_degrees != NULL);
    if (vertex_degrees[u] >= k) {
        return;
    }

    if (removed_vertices[u] == true) {
        return;
    }

    vertex_degrees[u] = 0;
    queue_enqueue(queue, u);
    removed_vertices[u] = true;
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
bool* get_vertices_not_in_k_core(Graph* graph, int k) {
    // Record for each vertex if it has been removed.
    bool* removed_vertices = calloc(graph->num_vertices, sizeof(bool));

    // Initial degree of each vertex.
    int* vertex_degrees = graph_get_out_degrees(graph);

    // Queue of vertices to be removed. Need to visit each neighbor.
    Queue* queue = queue_new(graph->num_vertices);

    // Enqueue all initial vertices with degree less than k.
    for (vertex u = 0; u < graph->num_vertices; u++) {
        if (vertex_degrees[u] < k && removed_vertices[u] == false) {
            queue_enqueue(queue, u);
            vertex_degrees[u] = 0;
            removed_vertices[u] = true;
        }
        _remove_less_than_k(u, k, vertex_degrees, removed_vertices, queue);
    }

    CompressedSparseRow* adjacency_matrix = graph->adjacency_matrix;
    int* ptr_rows = adjacency_matrix->ptr_rows;
    int* idx_cols = adjacency_matrix->idx_cols;

    // Continuously remove vertices with degree less than k, adding
    // their neighbors to the queue if the neighbor's degree becomes
    // less than k after the removal.
    while (queue->size > 0) {
        vertex removed = queue_dequeue(queue);
        int idx_begin_read = ptr_rows[removed];
        int idx_end_read = ptr_rows[removed + 1];

        for (int i = idx_begin_read; i < idx_end_read; i++) {
            vertex neighbor = idx_cols[i];
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