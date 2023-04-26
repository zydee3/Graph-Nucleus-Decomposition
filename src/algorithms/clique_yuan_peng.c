#include "clique_yuan_peng.h"

// Begin Helper Functions

/**
 * @brief This function returns the vertex with the highest degree.
 *
 * @param row_idx
 * @param col_idx
 * @param degrees
 * @return int
 */
int _direct_by_degree(int row_idx, int col_idx, int* degrees) {
    int degree_u = degrees[row_idx];
    int degree_v = degrees[col_idx];

    if (degree_u == degree_v) {
        return row_idx > col_idx ? row_idx : col_idx;
    }

    return degree_u > degree_v ? row_idx : col_idx;
}

/**
 * @brief This function marks all vertices in the param
 * visited_vertices array as visited if they are marked as visited in
 * the param recently_visited_vertices array.
 *
 * @param visited_vertices
 * @param recently_visited_vertices
 * @param num_vertices
 */
void _update_visited_vertices(bool* visited_vertices, bool* recently_visited_vertices, int num_vertices) {
    for (int idx_vertex = 0; idx_vertex < num_vertices; idx_vertex++) {
        if (recently_visited_vertices[idx_vertex] == true) {
            visited_vertices[idx_vertex] = true;
        }
    }
}

/**
 * @brief This function resizes and adds an element to the end of an
 * array of ordered sets.
 *
 * @param set
 * @param element
 * @param curr_len
 */
void _inc_and_add_ordered_set_array(OrderedSet** set, OrderedSet* element, int curr_len) {
    if (curr_len == 0) {
        set = (OrderedSet**)malloc(sizeof(OrderedSet*));
    } else {
        set = (OrderedSet**)realloc(set, sizeof(OrderedSet*) * (curr_len + 1));
    }

    set[curr_len] = element;
}

/**
 * @brief This function checks to see if the connected component
 * containing param vertex u is a clique.
 *
 * BFS is used to iterate through the connected component.
 * If the number of edges discovered in the connected component is
 * equal to the expected number of edges in a k-clique (k choose 2),
 * then the connected component is a clique and the function returns
 * an ordered set containing the vertices in the clique. Otherwise,
 * the function returns an empty ordered set indicating that the
 * connected component is not a clique. The function only traverses
 * vertices that have no been removed. As vertices are explored with
 * BFS, param removed_vertices is updated indicating that future
 * iterations of the function will not re-explore the same vertices.
 *
 * @param graph
 * @param vertex_u
 * @param removed_vertices
 * @return OrderedSet*
 */
OrderedSet* _find_relative_clique(CSRGraph* graph, int vertex_u, bool* removed_vertices) {
    assert(graph != NULL);
    assert(vertex_u >= 0);
    assert(vertex_u < graph->num_vertices);
    assert(removed_vertices != NULL);

    OrderedSet* relative_clique = ordered_set_new(graph->num_vertices);
    ordered_set_insert(relative_clique, vertex_u);

    int num_edges_in_clique = 0;

    CompressedSparseRow* adjacency_matrix = graph->adjacency_matrix;
    int* ptr_rows = adjacency_matrix->ptr_rows;
    int* idx_cols = adjacency_matrix->idx_cols;

    Queue* queue = queue_new(graph->num_vertices);
    queue_enqueue(queue, vertex_u);

    while (queue->size > 0) {
        // queue_print(queue, true);
        int idx_row = queue_dequeue(queue);
        int idx_begin_read = ptr_rows[idx_row];
        int idx_end_read = ptr_rows[idx_row + 1];

        for (int idx_nnz = idx_begin_read; idx_nnz < idx_end_read; idx_nnz++) {
            int idx_col = idx_cols[idx_nnz];
            num_edges_in_clique++;

            if (removed_vertices[idx_col] == false) {
                removed_vertices[idx_col] = true;
                ordered_set_insert(relative_clique, idx_col);
                queue_enqueue(queue, idx_col);
            }
        }
    }

    int num_vertices_in_clique = relative_clique->size;
    int expected_num_edges_in_clique = num_vertices_in_clique * (num_vertices_in_clique - 1) / 2;
    if (num_edges_in_clique != expected_num_edges_in_clique) {
        ordered_set_clear(relative_clique);
    }

    return relative_clique;
}

/**
 * @brief Returns the neighbors of param vertex u in an ordered set.
 *
 * @param graph
 * @param vertex_u
 * @return OrderedSet*
 */
OrderedSet* _get_neighbors(CSRGraph* graph, int vertex_u) {
    assert(graph != NULL);
    assert(vertex_u >= 0);
    assert(vertex_u < graph->num_vertices);

    OrderedSet* neighbors = ordered_set_new(graph->num_vertices);

    CompressedSparseRow* adjacency_matrix = graph->adjacency_matrix;
    int* ptr_rows = adjacency_matrix->ptr_rows;
    int* idx_cols = adjacency_matrix->idx_cols;

    int idx_begin_read = ptr_rows[vertex_u];
    int idx_end_read = ptr_rows[vertex_u + 1];

    for (int idx_nnz = idx_begin_read; idx_nnz < idx_end_read; idx_nnz++) {
        ordered_set_insert(neighbors, idx_cols[idx_nnz]);
    }

    return neighbors;
}

/**
 * @brief Verifies that the param clique is indeed a clique.
 *
 * @attention Horrible Time Complexity: O(|Clique|^2 * log(|V|))
 *
 * @param graph
 * @param clique
 * @return true
 * @return false
 */
bool _is_clique(CSRGraph* graph, OrderedSet* clique) {
    for (int i = 0; i < clique->size; i++) {
        for (int j = 0; j < clique->size; j++) {
            if (i == j) {
                continue;
            }

            if (csr_graph_get_edge(graph, clique->elements[i], clique->elements[j]) == -1) {
                return false;
            }
        }
    }

    return true;
}

// End Helper Functions

/**
 * Link:   https://arxiv.org/pdf/2203.13512.pdf
 * Title:  Efficient k-clique Listing with Set Intersection Speedup
 * Author: Yuan et al.
 */

// l = k value
// R = clique being built
// C = candidate vertices for R
/**
 * @brief Implentation of Algorithm 5 from Yuan et al.
 *
 * @attention This function does not work, however it may have to do
 * with the degrees array not being updated. I will revisit this.
 *
 * @param cliques The set of all cliques found.
 * @param num_cliques The number of cliques found.
 * @param l The current k-value.
 * @param R The current clique being built.
 * @param C The candidate vertices for R.
 * @param directed_graph
 * @param degrees
 */
void s_degree_list(OrderedSet** cliques, int num_cliques, int l, OrderedSet* R, OrderedSet* C, CSRGraph* undirected_graph, CSRGraph* directed_graph, int* degrees) {
    assert(l >= 0);
    assert(R != NULL);
    assert(directed_graph != NULL);
    assert(directed_graph->is_directed == true);

    for (int idx_candidate = 0; idx_candidate < C->size; idx_candidate++) {
        int candidate = C->elements[idx_candidate];

        if (degrees[candidate] <= l - 2) {
            continue;
        }

        OrderedSet* candidate_neighbors = _get_neighbors(directed_graph, candidate);
        OrderedSet* new_candidates = ordered_set_intersection(candidate_neighbors, C);

        if (l <= 2) {
            for (int idx_candidate = 0; idx_candidate < new_candidates->size; idx_candidate++) {
                ordered_set_insert(R, candidate);

                for (int idx_new_candidates = 0; idx_new_candidates < new_candidates->size; idx_new_candidates++) {
                    ordered_set_insert(R, new_candidates->elements[idx_new_candidates]);
                }

                if (_is_clique(undirected_graph, R) == true) {
                    // ordered_set_print(R, true);
                }
            }
        } else {
            ordered_set_insert(R, candidate);
            s_degree_list(cliques, num_cliques, l - 1, R, new_candidates, undirected_graph, directed_graph, degrees);
        }
    }

    // return S;
}

OrderedSet** find_k_cliques_yuan_peng(CSRGraph* graph, int k) {
    assert(graph != NULL);
    assert(graph->is_directed == false);
    assert(k > 0);

    // Algorithm 3: PRE-CORE(G, k)
    bool* visited_vertices = get_vertices_not_in_k_core(graph, k - 1);
    assert(visited_vertices != NULL);

    // Algorithm 4: PRE-LIST(G, k)
    OrderedSet** cliques = NULL;
    int num_cliques = 0;

    bool* traversed_vertices = calloc(graph->num_vertices, sizeof(bool));
    assert(traversed_vertices != NULL);
    memcpy(traversed_vertices, visited_vertices, graph->num_vertices * sizeof(bool));

    for (int i = 0; i < graph->num_vertices; i++) {
        if (traversed_vertices[i] == false) {
            // Check if the component vertex i is in is a clique
            OrderedSet* relative_clique = _find_relative_clique(graph, i, traversed_vertices);

            // If the relative clique is not empty, we found a clique
            if (relative_clique->size > 0) {
                // Mark all vertices in the clique as visited
                // _update_visited_vertices(visited_vertices, traversed_vertices, graph->num_vertices);

                // Add the clique to the list of cliques
                _inc_and_add_ordered_set_array(cliques, relative_clique, num_cliques);
                num_cliques++;
            }

            ordered_set_delete(&relative_clique);
            traversed_vertices[i] = true;
        }
    }

    // Algorithm 5: SDEGREE(G, k)
    int* degrees = csr_graph_get_degrees(graph);
    CSRGraph* directed_graph = csr_graph_make_directed(graph, _direct_by_degree, degrees);

    for (int i = 0; i < graph->num_vertices; i++) {
        if (visited_vertices[i] == false && degrees[i] >= k - 1) {
            // R <- {u}
            OrderedSet* R = ordered_set_new(graph->num_vertices);
            ordered_set_insert(R, i);

            OrderedSet* C = _get_neighbors(graph, i);

            s_degree_list(cliques, num_cliques, k - 1, R, C, graph, directed_graph, degrees);

            visited_vertices[i] = true;
        }
    }

    // csr_graph_delete(&k_core);
    // csr_graph_delete(&graph);

    return NULL;
}
