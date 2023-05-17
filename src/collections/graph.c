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
Graph* graph_new_from_file(const char* file_path) {
    FILE* file = file_open(file_path, FILE_READ_EXISTING);
    assert(file != NULL);

    Graph* graph = malloc(sizeof(Graph));

    // parse the first line for graph type (directed or undirected)
    _set_directedness(graph, file);

    // parse the second line for number of nodes and edges
    _set_num_vertices_edges(graph, file);

    // create and file the graph from the file
    graph->adjacency_matrix = csr_new_from_file(file, graph->num_vertices, graph->num_vertices, graph->num_edges, graph->is_directed);

    assert(graph->adjacency_matrix != NULL);

    if (graph->is_directed == false) {
        graph->num_edges *= 2;
    }

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

    // Create a deep copy of the graph. The copy will be modified
    // to create the directed graph.
    Graph* directed_graph = graph_copy(graph);
    directed_graph->is_directed = true;

    // For convenience.
    CompressedSparseRow* adjacency_matrix = directed_graph->adjacency_matrix;
    int* idx_dir_cols = adjacency_matrix->idx_cols;

    // Expand/Express the CSR row pointers into coordinate format.
    int* coord_rows = csr_get_coord_rows(directed_graph->adjacency_matrix);

    int skipped = 0;

    // Apply f to each row and column to obtain the new ordering.
    for (int i = 0; i < graph->num_edges; i++) {
        vertex u = coord_rows[i];
        vertex v = idx_dir_cols[i];

        // The CSR stores both (u, v) and (v, u) for undirected
        // graphs, so skip the second instance of the edge or else
        // the graph will contain duplicate edges as both (u, v)
        // and (v, u) will be added to the directed graph and pointed
        // in the same direction.
        if (u >= v) {
            skipped++;
            continue;
        }

        // apply f to determine the direction of the edge. f returns
        // which vertex will be directed towards by the other.
        vertex target = f(u, v, meta_data);
        vertex source = target == u ? v : u;

        // Apply the change. The skipped offset is used to account
        // for the skipped edges in the undirected graph.
        coord_rows[i - skipped] = source;
        idx_dir_cols[i - skipped] = target;
    }

    // Update the number of edges in the directed graph. The number of
    // edges in the directed graph should be exactly half of the
    // initial edge count since the CSR stores both (u, v) and (v, u).
    int num_edges = graph->num_edges - skipped;
    assert(num_edges == directed_graph->num_edges / 2);

    // Update the number of non-zero entries in the adjacency matrix.
    adjacency_matrix->num_nnzs = num_edges;
    directed_graph->num_edges = num_edges;

    // Reallocate the arrays to the new size to save memory.
    coord_rows = realloc(coord_rows, num_edges * sizeof(int));
    assert(coord_rows != NULL);

    adjacency_matrix->idx_cols = realloc(adjacency_matrix->idx_cols, num_edges * sizeof(int));
    assert(adjacency_matrix->idx_cols != NULL);

    adjacency_matrix->edge_weights = realloc(adjacency_matrix->edge_weights, num_edges * sizeof(int));
    assert(adjacency_matrix->edge_weights != NULL);

    // Parallel sort by rows then columns to prepare for the new csr
    array_parallel_sort_2(coord_rows, adjacency_matrix->idx_cols, directed_graph->num_edges, directed_graph->num_edges, true);

    // Return the coordinate format rows to the correct CSR row
    // pointers format.
    csr_compress_row_ptrs(directed_graph->adjacency_matrix, coord_rows);

    // Reallocate the row pointers to the new size to save memory.
    directed_graph->adjacency_matrix->ptr_rows = realloc(directed_graph->adjacency_matrix->ptr_rows, (directed_graph->num_vertices + 1) * sizeof(int));
    assert(directed_graph->adjacency_matrix->ptr_rows != NULL);

    free(coord_rows);

    return directed_graph;
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
 * The funcion finds the nnz value to get the index of the
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

    // Cannot have an edge to itself. Though, this should probably be
    // an assertion instead of a return value.
    if (idx_row == idx_col) {
        return -1;
    }

    // For convenience.
    int* ptr_rows = graph->adjacency_matrix->ptr_rows;
    int* idx_cols = graph->adjacency_matrix->idx_cols;

    // Compute nnz index of the target column in the specified row
    int idx_begin_read = ptr_rows[idx_row];
    int idx_end_read = ptr_rows[idx_row + 1];

    // If the row is empty, the edge must not exist.
    if (idx_begin_read == idx_end_read) {
        return -1;
    }

    // This should not happen. If this assertion failes, then the CSR
    // is not correctly storing offsets.
    assert(idx_end_read <= graph->num_edges);

    // Binary search within the given range (neighbors) to see if the
    // edge exists. If it does, we get the associated NNZ index which
    // can be used to index the weights array to get the associated
    // weight of the current edge. If the edge does not exist, then
    // the ranged binary search returns -1.
    int idx_nnz = array_binary_search_range(idx_cols, graph->num_edges, idx_begin_read, idx_end_read, idx_col);

    if (idx_nnz == -1) {
        return -1;
    }

    return graph->adjacency_matrix->edge_weights[idx_nnz];
}

/**
 * @brief Computes the out degree of each vertex in the graph and
 * returns the result as an array of size graph->num_vertices.
 *
 * The function allocates an array of size graph->num_vertices and
 * computes the out degree of each vertex in the graph. If the graph
 * is undirected, this function can also be used to compute the
 * degree of each vertex in the graph.
 *
 * @param graph The graph to compute the degrees of.
 * @return int* The array of out degrees of each vertex in the graph.
 */
int* graph_get_out_degrees(Graph* graph) {
    assert(graph != NULL);
    assert(graph->adjacency_matrix != NULL);

    int* degrees = calloc(graph->num_vertices, sizeof(int));
    assert(degrees != NULL);

    int* ptr_rows = graph->adjacency_matrix->ptr_rows;

    for (vertex u = 0; u < graph->num_vertices; u++) {
        degrees[u] = ptr_rows[u + 1] - ptr_rows[u];
    }

    return degrees;
}

/**
 * @brief Computes the in degree of each vertex in the graph and
 * returns the result as an array of size graph->num_vertices.
 *
 * The function allocates an array of size graph->num_vertices and
 * computes the out degree of each vertex in the graph.
 *
 * @param graph The graph to compute the degrees of.
 * @return int* The array of in degrees of each vertex in the graph.
 */
int* graph_get_in_degrees(Graph* graph) {
    assert(graph != NULL);
    assert(graph->adjacency_matrix != NULL);

    int* in_degrees = calloc(graph->num_vertices, sizeof(int));
    assert(in_degrees != NULL);

    int* ptr_rows = graph->adjacency_matrix->ptr_rows;
    int* idx_cols = graph->adjacency_matrix->idx_cols;

    for (vertex u = 0; u < graph->num_vertices; u++) {
        for (int idx_nnz = ptr_rows[u]; idx_nnz < ptr_rows[u + 1]; idx_nnz++) {
            vertex v = idx_cols[idx_nnz];
            in_degrees[v]++;
        }
    }

    return in_degrees;
}

/**
 * @brief Generates an ordered set of the neighbors of the specified
 * vertex in the graph.
 *
 * This function stores the neighbors of the specified vertex (out
 * neighbors if the graph is directed) in an ordered set and returns
 * the result. Since the CSR is sorted in increasing order of column
 * index with respect to each row, insertion into the ordered set is
 * done directly with the member array in the ordered set such that
 * we avoid the overhead of calling ordered_set_insert() for each
 * neighbor. This approach reduces the runtime from O(nlogn) to O(n).
 *
 * @param graph
 * @param idx_vertex_u
 * @return OrderedSet*
 */
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

    int idx_insert = 0;

    for (int idx_nnz = idx_begin_read; idx_nnz < idx_end_read; idx_nnz++) {
        int idx_vertex_v = graph->adjacency_matrix->idx_cols[idx_nnz];
        neighbors->elements[idx_insert++] = idx_vertex_v;
        neighbors->size++;
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