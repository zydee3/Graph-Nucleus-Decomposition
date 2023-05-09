// Comparator function for sorting vertices based on degree
int compare_vertices(const void* a, const void* b, void* degrees) {
    int* degs = (int*)degrees;
    int deg_a = degs[*(int*)a];
    int deg_b = degs[*(int*)b];

    if (deg_a < deg_b) return -1;
    if (deg_a > deg_b) return 1;
    if (*(int*)a < *(int*)b) return -1;
    if (*(int*)a > *(int*)b) return 1;
    return 0;
}

void compute_triangles(Graph* graph) {
    assert(graph != NULL);
    assert(graph->is_directed == false);
    assert(graph->adjacency_matrix != NULL);
    assert(graph->adjacency_matrix->is_set);

    int* undirected_degrees = graph_get_degrees(graph);
    int num_triangles = 0;

    for (vertex v = 0; v < graph->num_vertices; v++) {
        OrderedSet* neighbors = graph_get_neighbors(graph, v);

        int neighbor_count = ordered_set_cardinality(neighbors);
        int* neighbor_list = (int*)malloc(neighbor_count * sizeof(int));

        for (int i = 0; i < neighbor_count; i++) {
            neighbor_list[i] = ordered_set_get(neighbors, i);
        }

        qsort_r(neighbor_list, neighbor_count, sizeof(int), compare_vertices, undirected_degrees);

        for (int i = 0; i < neighbor_count; i++) {
            vertex u = neighbor_list[i];
            if (undirected_degrees[v] < undirected_degrees[u] || (undirected_degrees[v] == undirected_degrees[u] && v < u)) {
                for (int j = i + 1; j < neighbor_count; j++) {
                    vertex w = neighbor_list[j];
                    int uw = graph_get_edge(graph, u, w);
                    if (uw > 0) {
                        num_triangles++;
                    }
                }
            }
        }

        free(neighbor_list);
        ordered_set_delete(&neighbors);
    }

    free(undirected_degrees);
    printf("Number of triangles: %d\n", num_triangles);
}

template <bool isDOG = false, bool doDegreeOrdering_ = true, bool isUndirected = false, class F>
void triangleProgram(const CGraph* gOutList, F& functor, const CGraph* gInList = 0) {
    EdgeIdx* totalDegrees;  // used to cache vertex degrees, only used if doDegreeOrdering
    VertexIdx* tmpNbors;    // temporary filtered and re-sorted list of neighbors for a vertex.
    EdgeIdx* tmpOrigEdges;  // track original edge indices in the reordered edge list.
    EdgeIdx tmpnEdges;      // number of edges filtered by degree criterion

    // this just says that if isDOG, we will not do degree ordering regardless of
    // what was specified.
    constexpr bool doDegreeOrdering = !isDOG && doDegreeOrdering_;

    for (VertexIdx v = 0; v < gOutList->nVertices; ++v) {
        tmpNbors = gOutList->nbors + gOutList->offsets[v];
        tmpnEdges = gOutList->degree(v);

        // Now loop over the (filtered) out-edges of v
        for (EdgeIdx eu = 0; eu < tmpnEdges; ++eu) {
            VertexIdx u = tmpNbors[eu];

            for (EdgeIdx ew = eu + 1; ew < tmpnEdges; ++ew) {
                VertexIdx w = tmpNbors[ew];

                // check if u -> w is an edge.  For u -> w to be an edge in the
                // degree-ordered graph, either u -> w or u <- w must exist in
                // the original graph.
                // If the input graph is already degree-ordered, the check w -> u is unnecessary
                // if the input graph is undirected, the check w -> u is unnecessary
                auto uw = gOutList->getEdge(u, w);
                EdgeIdx vu = eu + gOutList->offsets[v];
                EdgeIdx vw = ew + gOutList->offsets[v];

                auto wu = gOutList->getEdge(w, u);

                // flip orientation in functor call.
                if (wu != invalidEdge)
                    functor(w, v, u, vw, vu, wu);
            }
        }
    }

    if (doDegreeOrdering) {
        delete[] tmpNbors;
        delete[] tmpOrigEdges;
        delete[] totalDegrees;
    }
}

void triangleProgram(Graph* gOutList) {
    CompressedSparseRow* adjacencyMatrix = gOutList->adjacency_matrix;
    int* ptr_rows = adjacencyMatrix->ptr_rows;
    int* idx_cols = adjacencyMatrix->idx_cols;

    for (vertex v = 0; v < gOutList->num_vertices; ++v) {
        int idx_v_begin_read = ptr_rows[v];
        int idx_v_end_read = ptr_rows[v + 1];

        // Now loop over the (filtered) out-edges of v
        for (int idx_v_nnz = idx_v_begin_read; idx_v_nnz < idx_v_end_read; ++idx_v_nnz) {
            vertex u = idx_cols[idx_u_nnz];

            for (int idx_w_nnz = idx_v_nnz + 1; idx_w_nnz < idx_v_end_read; ++idx_w_nnz) {
                vertex w = idx_cols[idx_w_nnz];

                // check if u -> w is an edge. For u -> w to be an edge in the
                // degree-ordered graph, either u -> w or u <- w must exist in
                // the original graph.
                int uw = graph_get_edge(gOutList, u, w);

                if (uw > 0) {
                    // call your triangle processing function here
                    // processTriangle(u, v, w);
                    printf("Triangle: %d-%d-%d\n", u, v, w);
                }
            }
        }
    }
}