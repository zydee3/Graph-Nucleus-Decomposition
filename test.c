int fourCliqueCounter(Graph* graph) {
    int ret = 0;                                                           // return value
    int* triends = (int*)malloc((graph->num_vertices + 1) * sizeof(int));  // array to store triangle ends

    for (int i = 0; i < graph->num_vertices; ++i)  // loop over vertices
    {
        for (int posj = graph->adjacency_matrix->ptr_rows[i]; posj < graph->adjacency_matrix->ptr_rows[i + 1]; ++posj)  // loop over out-neighbors of i
        {
            int j = graph->adjacency_matrix->idx_cols[posj];  // j is current out-neighbor
            int count = 0;

            for (int posk = posj + 1; posk < graph->adjacency_matrix->ptr_rows[i + 1]; ++posk)  // loop over another out-neighbor of i, that is "ahead" of j in list of out-neighbors
            {
                int k = graph->adjacency_matrix->idx_cols[posk];  // k is next out-neighbor
                if (graph_get_edge(graph, j, k) != -1)            // check if edge (j,k) is present
                {
                    triends[count] = k;  // so (i,j,k) form a triangle. we store the fact that k forms a triangle with edge (i,j) in graph
                    ++count;
                }
            }

            for (int posk = 0; posk < count; ++posk)  // loop over all pairs of triangles formed by (i,j)
            {
                int k = triends[posk];                                                                       // k is vertex as index posk in triends
                int degk = graph->adjacency_matrix->ptr_rows[k + 1] - graph->adjacency_matrix->ptr_rows[k];  // getting degree of k in graph
                int remaining = count - posk;                                                                // number of vertices that k needs to be checked with

                if (degk >= remaining) {
                    // We will search all other vertices in triends in k's adj list
                    for (int posell = posk + 1; posell < count; ++posell) {
                        int ell = triends[posell];
                        if (graph_get_edge(graph, k, ell) != -1)  // (k,ell) is an end, thus (i,j,k,ell) form a 4-clique
                        {
                            ++ret;
                        }
                    }
                } else {
                    // We will search all vertices in k's adj list in the remaining portion of triends
                    for (int posell = graph->adjacency_matrix->ptr_rows[k]; posell < graph->adjacency_matrix->ptr_rows[k + 1]; posell++) {
                        int ell = graph->adjacency_matrix->idx_cols[posell];
                        if (binarySearch(triends + posk + 1, count - posk - 1, ell) != -1) {
                            ++ret;
                        }
                    }
                }
            }
        }
    }

    free(triends);
    return ret;
}

CliqueSet* _find_four_cliques(Graph* graph) {
    assert(graph != NULL);
    assert(graph->is_directed == false);
    assert(graph->adjacency_matrix != NULL);
    assert(graph->adjacency_matrix->is_set);

    // 4-clique results list
    int clique_size = 4;
    int resize_value = 5;
    CliqueSet* four_cliques = clique_set_new(clique_size, resize_value);

    // Generate a degree oriented graph
    int* undirected_degrees = graph_get_out_degrees(graph);
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

                // Added loop to form a 4-clique
                for (int idx_x_nnz = idx_w_nnz + 1; idx_x_nnz < idx_v_end_read; idx_x_nnz++) {
                    vertex x = idx_cols[idx_x_nnz];

                    if (graph_get_edge(graph, u, w) > 0 && graph_get_edge(graph, u, x) > 0 && graph_get_edge(graph, w, x) > 0) {
                        int* four_clique = (int*)calloc(4, sizeof(int));
                        four_clique[0] = u;
                        four_clique[1] = v;
                        four_clique[2] = w;
                        four_clique[3] = x;

                        clique_set_insert(four_cliques, four_clique);
                    }
                }
            }
        }
    }

    free(undirected_degrees);
    graph_delete(&directed_graph);

    return four_cliques;
}

EdgeIdx fourCliqueCounter(CGraph* gout) {
    EdgeIdx ret = 0;                                          // return value
    VertexIdx* triends = new VertexIdx[gout->nVertices + 1];  // array to store triangle ends

    for (VertexIdx i = 0; i < gout->nVertices; ++i)                                   // loop over vertices
        for (VertexIdx posj = gout->offsets[i]; posj < gout->offsets[i + 1]; ++posj)  // loop over out-neighbors of i
        {
            VertexIdx j = gout->nbors[posj];  // j is current out-neighbor
            VertexIdx count = 0;
            for (VertexIdx posk = posj + 1; posk < gout->offsets[i + 1]; ++posk)  // loop over another out-neighbor of i, that is "ahead" of j in list of out-neighbors
            {
                VertexIdx k = gout->nbors[posk];  // k is next out-neighbor
                                                  //                 printf("looking at tri %ld %ld %ld\n",i,j,k);
                if (gout->isEdgeBinary(j, k))     // check if edge (j,k) is present
                {
                    triends[count] = k;  // so (i,j,k) form a triangle. we store the fact that k forms a triangle with edge (i,j) in digraph gout
                    ++count;
                }
            }

            for (VertexIdx posk = 0; posk < count; ++posk)  // loop over all pairs of triangles formed by (i,j)
            {
                VertexIdx k = triends[posk];                               // k is vertex as index posk in triends
                VertexIdx degk = gout->offsets[k + 1] - gout->offsets[k];  // gettting degree of k in gout
                VertexIdx remaining = count - posk;                        // number of vertices that k needs to be checked with

                //                 printf("looking at %ld %ld %ld\n",i,j,k);
                if (degk >= remaining) {
                    // We will search all other vertices in triends in k's adj list
                    for (VertexIdx posell = posk + 1; posell < count; ++posell) {
                        VertexIdx ell = triends[posell];
                        //                         printf("Going to triends %ld %ld %ld %ld\n",i,j,k,ell);
                        if (gout->isEdgeBinary(k, ell))  // (k,ell) is an end, thus (i,j,k,ell) form a 4-clique
                        {
                            ++ret;
                        }
                    }
                } else {
                    // We will search all vertices in k's adj list in the remaining portion of triends
                    for (EdgeIdx posell = gout->offsets[k]; posell < gout->offsets[k + 1]; posell++) {
                        VertexIdx ell = gout->nbors[posell];
                        if (binarySearch(triends + posk + 1, count - posk - 1, ell) != -1) {
                            ++ret;
                        }
                    }
                }
            }
        }
    return ret;
}

EdgeIdx fourCliqueCounter(CGraph* gout) {
    EdgeIdx ret = 0;                                          // return value
    VertexIdx* triends = new VertexIdx[gout->nVertices + 1];  // array to store triangle ends

    vertex* triangle_ends = (vertex*)malloc(gout->num_vertices * sizeof(vertex));

    for (VertexIdx i = 0; i < gout->nVertices; ++i)                                   // loop over vertices
        for (VertexIdx posj = gout->offsets[i]; posj < gout->offsets[i + 1]; ++posj)  // loop over out-neighbors of i
        {
            VertexIdx j = gout->nbors[posj];  // j is current out-neighbor
            VertexIdx count = 0;
            for (VertexIdx posk = posj + 1; posk < gout->offsets[i + 1]; ++posk)  // loop over another out-neighbor of i, that is "ahead" of j in list of out-neighbors
            {
                VertexIdx k = gout->nbors[posk];  // k is next out-neighbor
                                                  //                 printf("looking at tri %ld %ld %ld\n",i,j,k);
                if (gout->isEdgeBinary(j, k))     // check if edge (j,k) is present
                {
                    triends[count] = k;  // so (i,j,k) form a triangle. we store the fact that k forms a triangle with edge (i,j) in digraph gout
                    ++count;
                }
            }

            for (VertexIdx posk = 0; posk < count; ++posk)  // loop over all pairs of triangles formed by (i,j)
            {
                VertexIdx k = triends[posk];                               // k is vertex as index posk in triends
                VertexIdx degk = gout->offsets[k + 1] - gout->offsets[k];  // gettting degree of k in gout
                VertexIdx remaining = count - posk;                        // number of vertices that k needs to be checked with

                //                 printf("looking at %ld %ld %ld\n",i,j,k);
                if (degk >= remaining) {
                    // We will search all other vertices in triends in k's adj list
                    for (VertexIdx posell = posk + 1; posell < count; ++posell) {
                        VertexIdx ell = triends[posell];
                        //                         printf("Going to triends %ld %ld %ld %ld\n",i,j,k,ell);
                        if (gout->isEdgeBinary(k, ell))  // (k,ell) is an end, thus (i,j,k,ell) form a 4-clique
                        {
                            ++ret;
                        }
                    }
                } else {
                    // We will search all vertices in k's adj list in the remaining portion of triends
                    for (EdgeIdx posell = gout->offsets[k]; posell < gout->offsets[k + 1]; posell++) {
                        VertexIdx ell = gout->nbors[posell];
                        if (binarySearch(triends + posk + 1, count - posk - 1, ell) != -1) {
                            ++ret;
                        }
                    }
                }
            }
        }
    return ret;
}
