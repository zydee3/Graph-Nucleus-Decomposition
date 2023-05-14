#include "clique.h"

// Begin Helper Functions

/**
 * @brief This function returns the vertex with the highest degree.
 * Ties are broken by choosing the vertex with the higher index.
 *
 * @param row_idx
 * @param col_idx
 * @param degrees
 * @return int
 */
int _compare_degrees(int row_idx, int col_idx, int* degrees) {
    int degree_u = degrees[row_idx];
    int degree_v = degrees[col_idx];

    if (degree_u == degree_v) {
        return row_idx > col_idx ? row_idx : col_idx;
    }

    return degree_u > degree_v ? row_idx : col_idx;
}

// End Helper Functions
// Begin Specialized Clique Functions (k=1,2,3)

CliqueSet* _find_one_cliques(Graph* graph) {
    CliqueSet* vertices = clique_set_new(1, graph->num_vertices);
    clique* elements = vertices->cliques;

    for (vertex i = 0; i < graph->num_vertices; i++) {
        clique c = calloc(1, sizeof(int));
        c[0] = i;
        elements[i] = c;
    }

    return vertices;
}

/**
 * @brief This function returns the set of all 2-cliques in the graph.
 * This is synonymous to finding all edges in the graph.
 *
 * This function
 *
 * @param graph
 * @return CliqueSet*
 */
CliqueSet* _find_two_cliques(Graph* graph) {
    int num_edges = graph->adjacency_matrix->num_nnzs;
    CliqueSet* edges = clique_set_new(2, num_edges);

    clique* elements = edges->cliques;
    int idx_elements = 0;

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

            edge edge = calloc(2, sizeof(int));
            if (row_idx < col_idx) {
                edge[0] = row_idx;
                edge[1] = col_idx;
            } else {
                edge[0] = col_idx;
                edge[1] = row_idx;
            }

            elements[idx_elements++] = edge;
        }
    }

    return edges;
}

CliqueSet* _find_three_cliques(Graph* graph) {
    assert(graph != NULL);
    assert(graph->is_directed == false);
    assert(graph->adjacency_matrix != NULL);
    assert(graph->adjacency_matrix->is_set);

    // Triangle results list
    int clique_size = 3;
    int resize_value = 5;
    CliqueSet* triangles = clique_set_new(clique_size, resize_value);

    // Generate a degree oriented graph
    int* undirected_degrees = graph_get_degrees(graph);
    Graph* directed_graph = graph_make_directed(graph, _compare_degrees, undirected_degrees);

    // Store these variables for easy access
    CompressedSparseRow* adjacency_matrix = directed_graph->adjacency_matrix;
    int* ptr_rows = adjacency_matrix->ptr_rows;
    int* idx_cols = adjacency_matrix->idx_cols;

    // Loop over the (filtered) out-edges of v
    for (vertex v = 0; v < graph->num_vertices; v++) {
        int idx_v_begin_read = ptr_rows[v];
        int idx_v_end_read = ptr_rows[v + 1];

        for (int idx_u_nnz = idx_v_begin_read; idx_u_nnz < idx_v_end_read; idx_u_nnz++) {
            vertex u = idx_cols[idx_u_nnz];

            for (int idx_w_nnz = idx_u_nnz + 1; idx_w_nnz < idx_v_end_read; idx_w_nnz++) {
                vertex w = idx_cols[idx_w_nnz];

                if (graph_get_edge(graph, u, w) > 0) {
                    int* triangle = calloc(3, sizeof(int));
                    triangle[0] = u;
                    triangle[2] = v;
                    triangle[1] = w;

                    clique_set_insert(triangles, triangle);
                }
            }
        }
    }

    free(undirected_degrees);
    graph_delete(&directed_graph);

    return triangles;
}

// End Specialized Clique Functions (k=1,2,3)
// Begin Generalized Clique Functions (k>3)

/**
 * @brief This function is the recursive part of Chibi-Nishizeki's
 * algorithm for finding k-cliques in a graph.
 *
 * @param graph
 * @param target_k
 * @param current_k
 * @param current_vertex
 * @param current_clique
 * @param candidates
 * @param cliques
 */
void _find_neighbors(Graph* graph, int target_k, int current_k, int current_vertex, OrderedSet* current_clique, OrderedSet* candidates, CliqueSet* cliques) {
    assert(current_vertex >= 0);
    assert(current_vertex < graph->num_vertices);

    // If the current clique is of size k, add it to the list of cliques
    if (current_k == target_k) {
        clique clique_copy = calloc(target_k, sizeof(int));
        memcpy(clique_copy, current_clique->elements, target_k * sizeof(int));
        clique_set_insert(cliques, clique_copy);
        return;
    }

    OrderedSet* neighbors = graph_get_neighbors(graph, current_vertex);
    OrderedSet* new_candidates = ordered_set_intersection(candidates, neighbors);

    for (int i = 0; i < new_candidates->size; i++) {
        vertex candidate = new_candidates->elements[i];
        if (candidate > current_vertex) {
            ordered_set_insert(current_clique, candidate);
            _find_neighbors(graph, target_k, current_k + 1, candidate, current_clique, new_candidates, cliques);
            ordered_set_remove(current_clique, candidate);
        }
    }

    ordered_set_delete(&neighbors);
    ordered_set_delete(&new_candidates);
}

/**
 * @brief This function returns the set of all k-cliques in the graph.
 *
 * When k <= 3, this function uses specific functions to find each
 * k-clique. when k >= 4, this function uses Chibi-Nishizeki to find
 * each k-clique. The k-core of the input graph is computed to reduce
 * the search space of Chibi-Nishizeki.
 *
 * @param graph The graph to search.
 * @param k The size of the cliques to find.
 * @return CliqueSet* Clique set containing an nxk 2D array of
 * cliques, where n is the number of k-cliques found.
 */
CliqueSet* find_k_cliques(Graph* graph, int k) {
    assert(graph != NULL);
    assert(graph->is_directed == false);
    assert(graph->adjacency_matrix != NULL);
    assert(graph->adjacency_matrix->is_set);

    switch (k) {
        case 1:
            return _find_one_cliques(graph);
        case 2:
            return _find_two_cliques(graph);
        case 3:
            return _find_three_cliques(graph);
    }

    // Use chiba nishizeki to find all k-cliques when k >= 4.
    // Create the resulting set of k-cliques
    int resize_value = 5;
    CliqueSet* cliques = clique_set_new(k, resize_value);

    // Compute and k-core to reduce search space
    bool* is_vertex_removed = get_vertices_not_in_k_core(graph, k);

    // Create clique of size k to compute k-cliques
    OrderedSet* current_clique = ordered_set_new(k);

    for (vertex v = 0; v < graph->num_vertices; v++) {
        if (is_vertex_removed[v] == true) {
            continue;
        }

        ordered_set_insert(current_clique, v);
        OrderedSet* candidates = graph_get_neighbors(graph, v);
        _find_neighbors(graph, k, 1, v, current_clique, candidates, cliques);
        ordered_set_remove(current_clique, v);
        ordered_set_delete(&candidates);
    }

    ordered_set_delete(&current_clique);
    free(is_vertex_removed);

    return cliques;
}

// End Generalized Clique Functions (k>3)
// Begin Clique Expansion Functions

/**
 * @brief Inserts param _clique into the clique set param cliques if
 * the symmetric difference of _clique to any clique in cliques is 2.
 *
 * The symmetric difference of two cliques of two cliques is 0 if they
 * are equal, 1 if they differ by one vertex, and 2 if they differ by
 * two vertices. A symmetric difference of 1 is forbidden since both
 * cliques should be of size k. A symmetric difference of 0 is also
 * forbidden since the clique sets where these cliques are derived
 * from should not contain duplicates.
 *
 * If the size of this clique set is k, then the clique set consists
 * of vertices which are contained in a k+1 clique.
 *
 * @param cliques
 * @param _clique
 * @return bool, True if the clique was insertted, false otherwise.
 */
bool insert_clique_into_group(CliqueSet* cliques, clique _clique) {
    assert(cliques != NULL);
    assert(_clique != NULL);

    int k = cliques->k;

    for (int i = 0; i < cliques->size; i++) {
        clique curr_clique = cliques->cliques[i];

        int symm_diff = array_count_symmetric_difference(curr_clique, _clique, k, k);

        // if the size is 0, then the cliques are equal. this should
        // not happen since clique sets should not contain duplicates
        assert(symm_diff != 0);

        // if the size is 1, then we're taking the symmetric
        // difference of two cliques with different sizes, which is
        // impossible as all cliques should be of size k.
        assert(symm_diff != 1);

        // if the size is not 2, then the cliques are not adjacent.
        // actually, this is a redundant check as this would have
        // failed in any iteration (can be shown inductively)
        if (symm_diff > 2) {
            return false;
        }

        // symm_diff is 2, so the curr_clique and _clique are adjacent
    }

    clique_set_insert(cliques, _clique);
    return true;
}

GenericLinkedList* group_cliques(CliqueSet* cliques) {
    assert(cliques != NULL);

    void* (*cpy)(void*) = (void* (*)(void*))clique_set_copy;
    void (*del)(void**) = (void (*)(void**))clique_set_delete;
    bool (*eq)(void*, void*) = (bool (*)(void*, void*))clique_set_is_equal;
    void (*print)(void*) = (void (*)(void*))clique_set_print_n;

    GenericLinkedList* groups = generic_linked_list_new(cpy, del, eq, print);

    GenericNode* last_group = NULL;

    for (int i = 0; i < cliques->size; i++) {
        clique clique = cliques->cliques[i];
        GenericNode* curr_group = groups->head;
        bool was_inserted = false;

        while (curr_group != NULL) {
            CliqueSet* group = curr_group->data;

            was_inserted = insert_clique_into_group(group, clique);
            if (was_inserted) {
                break;
            }

            curr_group = curr_group->next;
        }

        if (was_inserted) {
            continue;
        }

        CliqueSet* new_group = clique_set_new(cliques->k, 1);
        clique_set_insert(new_group, clique);

        if (last_group != NULL) {
            last_group->next = generic_node_new(new_group);
            last_group = last_group->next;
        } else {
            groups->head = generic_node_new(new_group);
            last_group = groups->head;
        }

        groups->size++;
    }

    return groups;
}

CliqueSet* reduce_groups(GenericLinkedList* groups, int k) {
    assert(groups != NULL);
    assert(k >= 3);

    CliqueSet* reduced_cliques = clique_set_new(k + 1, groups->size);

    GenericNode* curr_group = groups->head;

    while (curr_group != NULL) {
        CliqueSet* group = curr_group->data;

        // if the group size is > k+1, then a duplicate must exist
        // which is forbidden by the algorithm
        // assert(group->size <= k + 1);

        // if the group size is <= k, then the set of k-cliques cannot
        // be reduced to a (k+1)-clique
        if (group->size <= k) {
            curr_group = curr_group->next;
            continue;
        }

        // group size must be k+1 by now
        int len_reduced_clique = 0;
        clique reduced_clique = NULL;

        // since the symmetric difference of any two cliques in this
        // group is 2, we can take the union of any two cliques to
        // obtain the (k+1)-clique
        array_union(group->cliques[0], group->cliques[1], k, k, &reduced_clique, &len_reduced_clique);

        // make sure the union has length (k+1)
        assert(len_reduced_clique == k + 1);

        // insert the expanded clique into the reduced clique set
        clique_set_insert(reduced_cliques, reduced_clique);

        curr_group = curr_group->next;
    }

    return reduced_cliques;
}

CliqueSet* expand_cliques(Graph* graph, CliqueSet* k_cliques) {
    assert(graph != NULL);
    assert(k_cliques != NULL);

    if (k_cliques->size == 0) {
        return clique_set_new(k_cliques->k + 1, 0);
    }

    int k = k_cliques->k;

    if (k < 3) {
        return find_k_cliques(graph, k + 1);
    }

    GenericLinkedList* groups = group_cliques(k_cliques);

    CliqueSet* reduced_groups = reduce_groups(groups, k);

    GenericNode* curr_group = groups->head;
    while (curr_group != NULL) {
        CliqueSet* group = curr_group->data;
        group->size = 0;
        curr_group = curr_group->next;
    }

    generic_linked_list_delete(&groups);

    return reduced_groups;
}

// End Clique Expansion Functions