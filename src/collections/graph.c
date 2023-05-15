#include "graph.h"

// Begin Parser Functions
/**
 * @brief Parses the file for the graph's directedness.
 *
 * @note The file should have the directedness on the first line.
 *
 * @param graph The graph to set the directedness for.
 * @param file The file to parse.
 */
static inline void _set_directedness(Graph* graph, FILE* file) {
    assert(graph != NULL && file != NULL);

    const int len_buffer = 50;
    char* buffer = calloc(len_buffer, sizeof(char));

    assert(fgets(buffer, len_buffer, file) != NULL);
    assert(strcmp(buffer, "% directed\n") == 0 || strcmp(buffer, "% undirected\n") == 0);

    graph->is_directed = (strcmp(buffer, "% directed\n") == 0);

    free(buffer);
}

/**
 * @brief Parses the file for the graph's vertices and edges count.
 *
 * @note The file should have these on the second line.
 *
 * @param graph The graph to set the vertices and edges count for.
 * @param file The file to parse.
 */
static inline void _set_num_vertices_edges(Graph* graph, FILE* file) {
    assert(graph != NULL && file != NULL);

    const int len_buffer = 50;
    char* buffer = calloc(len_buffer, sizeof(char));

    int* ref_num_vertices = malloc(sizeof(int));
    int* ref_num_edges = malloc(sizeof(int));

    assert(fgets(buffer, len_buffer, file) != NULL);
    assert(sscanf(buffer, "%% %d %d", ref_num_vertices, ref_num_edges) == 2);

    graph->num_vertices = *ref_num_vertices;
    graph->num_edges = *ref_num_edges;

    free(buffer);
    free(ref_num_vertices);
    free(ref_num_edges);
}

// End Parser Functions
// Begin Helper Functions

/**
 * @brief
 *
 * @deprecated This function was used when the graph was stored as an
 * a CSR that only considered the lower triangular matrix.
 *
 * @param graph
 * @return int*
 */
static inline int* _get_directed_degrees(Graph* graph) {
    int* degrees = calloc(graph->num_vertices, sizeof(int));

    int* ptr_rows = graph->adjacency_matrix->ptr_rows;
    int* idx_cols = graph->adjacency_matrix->idx_cols;

    for (int idx_row = 0; idx_row < graph->num_vertices; idx_row++) {
        int idx_begin_read = ptr_rows[idx_row];
        int idx_end_read = ptr_rows[idx_row + 1];

        for (int idx_col = idx_begin_read; idx_col < idx_end_read; idx_col++) {
            vertex v = idx_cols[idx_col];
            degrees[v]++;
        }
    }

    return degrees;
}

/**
 * @brief Computes the degrees of all vertices in the undirected
 * graph and returns them in an array of length num_vertices.
 *
 * The function uses a nested loop to loop through the compressed
 * sparse row to compute the degrees. The outer loop iterates through
 * the rows and the inner loop iterates through the columns. At each
 * iteration of the inner loop, we have a specific edge given by
 * (index of row, index of column) = (u, v). We increment the degree
 * of both u and v by 1 as the graph is undirected.
 *
 * @deprecated This function was used when the graph was stored as an
 * a CSR that only considered the lower triangular matrix.
 *
 * @param graph The graph to compute the degrees for.
 * @return int* An array of length num_vertices containing the
 * degrees of all vertices.
 */
static inline int* _get_undirected_degrees(Graph* graph) {
    int* degrees = calloc(graph->num_vertices, sizeof(int));

    int* ptr_rows = graph->adjacency_matrix->ptr_rows;
    int* idx_cols = graph->adjacency_matrix->idx_cols;

    for (int idx_row = 0; idx_row < graph->num_vertices; idx_row++) {
        int idx_begin_read = ptr_rows[idx_row];
        int idx_end_read = ptr_rows[idx_row + 1];

        for (int idx_col = idx_begin_read; idx_col < idx_end_read; idx_col++) {
            vertex u = idx_row;
            vertex v = idx_cols[idx_col];

            degrees[u]++;
            degrees[v]++;
        }
    }

    return degrees;
}

/**
 * @brief Computes the index of the target column in a specified row.
 *
 * The function first calculates the bound of the row to search in. If
 * the row is empty the function returns -1. Otherwise, the row exists
 * and the function leverges the ordered nature of the compressed
 * sparse row to perform a binary search on the row within the bounds.
 * If the target column is found, the function returns the index of
 * the target column. Otherwise, the function returns -1.
 *
 * The index returned is the relative column index in the compressed
 * sparse row and not the value for some vertex v in an edge (u,v).
 *
 * The index returned, if not -1, can be used to access the value of
 * the edge (u,v) in graph->adjacency_matrix->edge_weights.
 *
 * @param graph The graph to search for the target column in.
 * @param idx_row The index of the row to search in.
 * @param idx_col The index of the column to search for.
 * @return int The index of the target column in the specified row if
 * it exists. Otherwise, -1.
 */
static inline int _get_relative_idx_col(Graph* graph, int idx_row, int idx_col) {
    int* ptr_rows = graph->adjacency_matrix->ptr_rows;
    int* idx_cols = graph->adjacency_matrix->idx_cols;

    int idx_begin_read = ptr_rows[idx_row];
    int idx_end_read = ptr_rows[idx_row + 1];

    if (idx_begin_read == idx_end_read) {
        return -1;
    }

    return array_binary_search_range(idx_cols, graph->num_edges, idx_begin_read, idx_end_read, idx_col);
}

// End Helper Functions
// Begin Create and Delete Functions

/**
 * @brief Creates a new graph with the specified number of vertices
 * and edges.
 *
 * The graph initializes the adjacency matrix with the specified
 * number of vertices and edges. The remaining members of the graph
 * are initialized with the specified parameter values.
 *
 * @param num_vertices The number of vertices in the graph.
 * @param num_edges The number of edges in the graph.
 * @param is_directed Whether the graph is directed or not.
 * @return Graph* The newly created graph.
 */
Graph* graph_new(int num_vertices, int num_edges, bool is_directed) {
    CompressedSparseRow* csr = csr_new(num_vertices, num_vertices, num_edges);
    assert(csr != NULL);

    Graph* graph = malloc(sizeof(Graph));
    graph->num_vertices = num_vertices;
    graph->num_edges = num_edges;
    graph->is_directed = is_directed;
    graph->adjacency_matrix = csr;

    return graph;
}

/**
 * @brief Creates a new graph from the file at the specified path.
 *
 * The function opens the file then calls _set_directedness and
 * _set_num_vertices_edges to parse the first two lines of the file.
 * The first two lines of the file contain the graph directedness type
 * and the number of vertices and edges respectively. The function
 * then calls csr_new_from_file to create the adjacency matrix from
 * the file. The function then closes the file and returns the newly
 * created graph.
 *
 * @param file_path The path to the file to create the graph from.
 * @return Graph* The newly created graph.
 */
Graph* graph_new_from_path(const char* file_path) {
    FILE* file = file_open(file_path, FILE_READ_EXISTING);
    assert(file != NULL);

    Graph* graph = malloc(sizeof(Graph));

    // parse the first line for graph type (directed or undirected)
    _set_directedness(graph, file);

    // parse the second line for number of nodes and edges
    _set_num_vertices_edges(graph, file);

    // create and fille the graph from the file
    graph->adjacency_matrix = csr_new_from_file(file, graph->num_vertices, graph->num_vertices, graph->num_edges);
    assert(graph->adjacency_matrix != NULL);

    fclose(file);

    return graph;
}

/**
 * @brief Creates a copy of the specified graph.
 *
 * The function creates a deep copy of the specified graph.
 *
 * @param graph The graph to create a deep copy of.
 * @return Graph* The newly created deep copy of the graph.
 */
Graph* graph_copy(Graph* graph) {
    assert(graph != NULL);
    assert(graph->adjacency_matrix->is_set);

    Graph* copy = graph_new(graph->num_vertices, graph->num_edges, graph->is_directed);
    csr_delete(&copy->adjacency_matrix);

    copy->adjacency_matrix = csr_copy(graph->adjacency_matrix);
    return copy;
}

/**
 * @brief Deletes the specified graph.
 *
 * The function calls csr_delete on the adjacency matrix of the graph
 * then frees the graph and sets the pointer Graph** graph to NULL.
 *
 * @param graph The graph to delete.
 */
void graph_delete(Graph** graph) {
    assert(graph != NULL);
    assert(*graph != NULL);

    csr_delete(&(*graph)->adjacency_matrix);
    free(*graph);
    *graph = NULL;
}

/**
 * @brief Creates a directed graph from the specified undirected
 * graph using the specified function to determine the edge direction.
 *
 * The function creates a deep copy of the specified undirected graph
 * then decompresses the CSR representation of the graph into a
 * coordinate format. The function then iterates over the coordinate
 * format and calls the specified function to determine the direction
 * of the edge. The function then sets the edge direction in the
 * directed graph. The current state of the csr directed graph is
 * invalid and must be sorted. Parallel sort is used to sort the
 * decompressed row indices, column indices, and edge weights on the
 * row indicides then the column indices. The function then compresses
 * the sorted row indicies into the equivalent row pointers for the
 * csr directed graph. The function then returns the newly created
 * directed graph.
 *
 * @param graph The undirected graph to create a directed graph from.
 * @param f The function to determine the direction of any given edge.
 * @return Graph* The newly created directed graph.
 */
Graph* graph_make_directed(Graph* graph, int (*f)(int, int, int*), int* meta_data) {
    assert(graph != NULL);
    assert(graph->adjacency_matrix->is_set);
    assert(f != NULL);

    assert(graph->is_directed == false);

    Graph* directed_graph = graph_copy(graph);
    directed_graph->is_directed = true;

    CompressedSparseRow* adjacency_matrix = directed_graph->adjacency_matrix;
    int* idx_dir_cols = adjacency_matrix->idx_cols;

    // decompress csr into coordinate format
    int* idx_decompressed_und_rows = csr_decompress_row_ptrs(directed_graph->adjacency_matrix);

    // apply f to each row and column to obtain the new ordering
    for (int i = 0; i < graph->num_edges; i++) {
        vertex u = idx_decompressed_und_rows[i];
        vertex v = idx_dir_cols[i];

        vertex target = f(u, v, meta_data);
        vertex source = target == u ? v : u;

        idx_decompressed_und_rows[i] = source;
        idx_dir_cols[i] = target;
    }

    // parallel sort by rows then columns to prepare for the new csr
    array_parallel_sort_2(idx_decompressed_und_rows, adjacency_matrix->idx_cols, graph->num_edges, graph->num_edges, true);

    // compress coordinate format into csr
    csr_compress_row_ptrs(directed_graph->adjacency_matrix, idx_decompressed_und_rows);

    free(idx_decompressed_und_rows);

    return directed_graph;
}

Graph* graph_reduce(Graph* graph, bool* removed_vertices) {
    // cloning would cost space and would run in the same time, maybe
    // worse, than just counting the number of vertices and edges
    assert(graph != NULL);
    assert(graph->adjacency_matrix->is_set);
    assert(removed_vertices != NULL);

    CompressedSparseRow* ref_adjacency_matrix = graph->adjacency_matrix;
    int* ptr_ref_rows = ref_adjacency_matrix->ptr_rows;
    int* idx_ref_cols = ref_adjacency_matrix->idx_cols;

    int max_vertex_id = 0;
    int num_edges = 0;

    // find the max vertex id and count number of edges not removed
    for (int idx_row = 0; idx_row < graph->num_vertices; idx_row++) {
        if (removed_vertices[idx_row] == true) {
            continue;
        }

        max_vertex_id = (max_vertex_id < idx_row) ? idx_row : max_vertex_id;

        int idx_begin_read = ptr_ref_rows[idx_row];
        int idx_end_read = ptr_ref_rows[idx_row + 1];

        for (int idx_nnz = idx_begin_read; idx_nnz < idx_end_read; idx_nnz++) {
            int idx_col = idx_ref_cols[idx_nnz];

            if (removed_vertices[idx_col] == false) {
                max_vertex_id = (max_vertex_id < idx_col) ? idx_col : max_vertex_id;
                num_edges++;
            }
        }
    }

    max_vertex_id++;
    // allocate memory for the reduced csr
    CompressedSparseRow* reduced_csr = csr_reduce(graph->adjacency_matrix, max_vertex_id, max_vertex_id, num_edges, removed_vertices);

    // allocate the reduced graph
    Graph* reduced_graph = malloc(sizeof(Graph));
    assert(reduced_graph != NULL);
    reduced_graph->num_vertices = max_vertex_id;
    reduced_graph->num_edges = num_edges;
    reduced_graph->is_directed = graph->is_directed;
    reduced_graph->adjacency_matrix = reduced_csr;

    reduced_csr->is_set = true;

    return reduced_graph;
}

// End Create and Delete Functions
// Begin Getter Functions

/**
 * @brief Gets the weight of the edge (idx_row, idx_col) in the graph.
 *
 * If the graph is unweighted, the function will always return 1 for
 * any edge in the graph that exists. If the graph is weighted, the
 * function will return a weight that is strictly non-negative if the
 * edge exists. Otherwise, the function will return -1. In other
 * words, a value of 0 should not be possible.
 *
 * The function calls _get_relative_idx_col to get the index of the
 * target column in the specified row. If the target column does not
 * exist in the specified row, the edge must also not exist and the
 * function returns -1. Otherwise, if the target column exists, then
 * graph->adjacency_matrix->edge_weights can be indexed with the
 * returned relative column index to get the weight of the edge.
 *
 * @param graph The graph where the desired edge is.
 * @param idx_row The index of the row of the desired edge.
 * @param idx_col The index of the column of the desired edge.
 * @return int The weight of the desired edge if it exists. Otherwise,
 * -1. If the graph is unweighted, the function will always return 1
 * for any edge in the graph that exists.
 */
int graph_get_edge(Graph* graph, int idx_row, int idx_col) {
    assert(graph != NULL);
    assert(graph->adjacency_matrix != NULL);
    assert(graph->adjacency_matrix->is_set);
    assert(graph->adjacency_matrix->ptr_rows != NULL);
    assert(graph->adjacency_matrix->idx_cols != NULL);
    assert(idx_row >= 0);
    assert(idx_col >= 0);
    assert(idx_row < graph->num_vertices);
    assert(idx_col < graph->num_vertices);

    // Cannot have an edge to itself
    assert(idx_row != idx_col);

    // Probably should not accomodate this, but the graph only stores
    // the lower triangle of the adj matrix, so we need to swap the
    // indices if the row index is greater than the col index.
    if (graph->is_directed == false && idx_row > idx_col) {
        int temp = idx_row;
        idx_row = idx_col;
        idx_col = temp;
    }

    int idx_relative_col = _get_relative_idx_col(graph, idx_row, idx_col);

    // if the target column does not exist in the specified row, the
    // edge must also not exist
    if (idx_relative_col == -1) {
        return -1;
    }

    return graph->adjacency_matrix->edge_weights[idx_relative_col];
}

/**
 * @brief Computes the degree of each vertex in the graph and returns
 * the result as an array.
 *
 * The function allocates an array of size graph->num_vertices and
 * computes the degree of each vertex in the graph. The function then
 * returns the array.
 *
 * @param graph The graph to compute the degrees of.
 * @return int* The array of degrees of each vertex in the graph.
 */
int* graph_get_degrees(Graph* graph) {
    assert(graph != NULL);
    assert(graph->adjacency_matrix != NULL);

    if (graph->is_directed == true) {
        return _get_directed_degrees(graph);
    }

    return _get_undirected_degrees(graph);
}

OrderedSet* graph_get_neighbors(Graph* graph, int idx_vertex_u) {
    assert(graph != NULL);
    assert(graph->adjacency_matrix != NULL);
    assert(graph->adjacency_matrix->is_set);
    assert(graph->adjacency_matrix->ptr_rows != NULL);
    assert(graph->adjacency_matrix->idx_cols != NULL);
    assert(idx_vertex_u >= 0);
    assert(idx_vertex_u < graph->num_vertices);

    int idx_begin_read = graph->adjacency_matrix->ptr_rows[idx_vertex_u];
    int idx_end_read = graph->adjacency_matrix->ptr_rows[idx_vertex_u + 1];

    OrderedSet* neighbors = ordered_set_new(idx_end_read - idx_begin_read + 1);
    for (int idx_nnz = idx_begin_read; idx_nnz < idx_end_read; idx_nnz++) {
        int idx_vertex_v = graph->adjacency_matrix->idx_cols[idx_nnz];
        ordered_set_insert(neighbors, idx_vertex_v);
    }

    ordered_set_fit(neighbors);

    return neighbors;
}

// End Getter Functions
// Begin Utility Functions

/**
 * @brief Prints the specified graph.
 *
 * The function prints the specified graph by printing locale members
 * as well as calling csr_print on the adjacency matrix of the graph.
 *
 * @param graph The graph to print.
 * @param should_print_newline True if a newline should be printed
 * at the end of all print statements, false otherwise.
 */
void graph_print(Graph* graph, bool should_print_newline) {
    assert(graph != NULL);
    assert(graph->adjacency_matrix != NULL);
    assert(graph->adjacency_matrix->is_set);

    printf("Graph: { Directed: %s, Vertices: %d, Edges: %d, ", (graph->is_directed ? "True" : "False"), graph->num_vertices, graph->num_edges);
    csr_print(graph->adjacency_matrix, false);
    printf("}");

    if (should_print_newline) {
        printf("\n");
    }
}

// End Utility Functions