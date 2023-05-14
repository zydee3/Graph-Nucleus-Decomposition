/**
 * @brief Set the k
 *
 * @param graph
 * @param r_cliques
 * @param s_cliques
 * @return int*
 */
int* set_k(Graph* graph, int r, int s) {
    GenericLinkedList* list_r_cliques = find_k_cliques(graph, r);
    GenericLinkedList* list_s_cliques = expand_cliques(graph, list_r_cliques);

    int num_r_cliques = list_r_cliques->size;
    int num_s_cliques = list_s_cliques->size;

    OrderedSet** r_cliques = ordered_set_as_array(&list_r_cliques);
    OrderedSet** s_cliques = ordered_set_as_array(&list_s_cliques);

    int* s_degrees = _compute_all_s_degrees(r_cliques, s_cliques, num_r_cliques, num_s_cliques);

    int* k_values = (int*)calloc(num_r_cliques, sizeof(int));

    bool* is_processed = calloc(num_r_cliques, sizeof(bool));

    // Map r-cliques to s-cliques and vice versa
    OrderedSet* r_clique = NULL;
    OrderedSet* s_clique = NULL;

    OrderedSet** r_map = (OrderedSet**)calloc(num_r_cliques, sizeof(OrderedSet*));
    OrderedSet** s_map = (OrderedSet**)calloc(num_s_cliques, sizeof(OrderedSet*));

    for (int idx_r = 0; idx_r < num_r_cliques; idx_r++) {
        r_clique = r_cliques[idx_r];
        if (r_map[idx_r] == NULL) {
            r_map[idx_r] = ordered_set_new(r);
        }

        for (int idx_s = 0; idx_s < num_s_cliques; idx_s++) {
            s_clique = s_cliques[idx_s];
            if (s_map[idx_s] == NULL) {
                s_map[idx_s] = ordered_set_new(s);
            }

            if (array_count_symmetric_difference(r_clique->elements, s_clique->elements, r_clique->size, s_clique->size) == (s - r)) {
                ordered_set_insert(r_map[idx_r], idx_s);
                ordered_set_insert(s_map[idx_s], idx_r);
            }
        }
    }

    // For each unprocessed R with minimum S-degree
    int num_processed = 0;

    while (num_processed < num_r_cliques) {
        int idx_min_r = array_filtered_argmin(s_degrees, num_r_cliques, is_processed);
        k_values[idx_min_r] = s_degrees[idx_min_r];

        r_clique = r_cliques[idx_min_r];

        OrderedSet* current_r_map = r_map[idx_min_r];

        // For each s-clique containing R
        for (int idx_r_map = 0; idx_r_map < current_r_map->size; idx_r_map++) {
            int idx_s = current_r_map->elements[idx_r_map];
            OrderedSet* current_s_map = s_map[idx_s];

            if (_is_s_clique_processed(current_s_map, is_processed)) {
                continue;
            }

            for (int idx_s_map = 0; idx_s_map < current_s_map->size; idx_s_map++) {
                int idx_neighbor_r = current_s_map->elements[idx_s_map];
                if (s_degrees[idx_neighbor_r] > s_degrees[idx_min_r]) {
                    s_degrees[idx_neighbor_r]--;
                }
            }
        }

        is_processed[idx_min_r] = true;
        num_processed++;
    }

    // Just Printing
    int num_printed = 0;
    int k_value = 0;

    while (num_printed < num_r_cliques) {
        int counter = 0;

        for (int idx_k = 0; idx_k < num_r_cliques; idx_k++) {
            if (k_values[idx_k] == k_value) {
                counter++;
                num_printed++;
            }
        }

        printf("%d: %d\n", k_value, counter);
        k_value++;
    }

    return k_values;
}

int* approximate_k(Graph* graph, int r, int s) {
    GenericLinkedList* list_r_cliques = find_k_cliques(graph, r);
    GenericLinkedList* list_s_cliques = expand_cliques(graph, list_r_cliques);

    int num_r_cliques = list_r_cliques->size;
    int num_s_cliques = list_s_cliques->size;

    OrderedSet** r_cliques = ordered_set_as_array(&list_r_cliques);
    OrderedSet** s_cliques = ordered_set_as_array(&list_s_cliques);

    int* s_degrees = _compute_all_s_degrees(r_cliques, s_cliques, num_r_cliques, num_s_cliques);

    int* k_values = (int*)calloc(num_r_cliques, sizeof(int));

    // Map r-cliques to s-cliques and vice versa
    OrderedSet* r_clique = NULL;
    OrderedSet* s_clique = NULL;

    OrderedSet** r_map = (OrderedSet**)calloc(num_r_cliques, sizeof(OrderedSet*));
    OrderedSet** s_map = (OrderedSet**)calloc(num_s_cliques, sizeof(OrderedSet*));

    for (int idx_r = 0; idx_r < num_r_cliques; idx_r++) {
        r_clique = r_cliques[idx_r];
        if (r_map[idx_r] == NULL) {
            r_map[idx_r] = ordered_set_new(r);
        }

        for (int idx_s = 0; idx_s < num_s_cliques; idx_s++) {
            s_clique = s_cliques[idx_s];
            if (s_map[idx_s] == NULL) {
                s_map[idx_s] = ordered_set_new(s);
            }

            if (array_count_symmetric_difference(r_clique->elements, s_clique->elements, r_clique->size, s_clique->size) == (s - r)) {
                ordered_set_insert(r_map[idx_r], idx_s);
                ordered_set_insert(s_map[idx_s], idx_r);
            }
        }
    }

    int* r_clique_order = array_generate_sequence(0, 1, num_r_cliques);
    array_parallel_sort_2(r_clique_order, s_degrees, num_r_cliques, num_r_cliques, true);

    for (int idx_r = 0; idx_r < num_r_cliques; idx_r++) {
        int idx_min_r = r_clique_order[idx_r];
        k_values[idx_min_r] = s_degrees[idx_min_r];
        r_clique = r_cliques[idx_min_r];

        OrderedSet* current_r_map = r_map[idx_min_r];

        // For each s-clique containing R
        for (int idx_r_map = 0; idx_r_map < current_r_map->size; idx_r_map++) {
            int idx_s = current_r_map->elements[idx_r_map];
            OrderedSet* current_s_map = s_map[idx_s];

            bool is_processed = false;

            for (int idx_s_map = 0; idx_s_map < current_s_map->size; idx_s_map++) {
                int idx_neighbor_r = current_s_map->elements[idx_s_map];
                if (idx_neighbor_r > idx_min_r) {
                    is_processed = true;
                    break;
                }
            }

            if (is_processed) {
                continue;
            }

            for (int idx_s_map = 0; idx_s_map < current_s_map->size; idx_s_map++) {
                int idx_neighbor_r = current_s_map->elements[idx_s_map];
                if (s_degrees[idx_neighbor_r] > s_degrees[idx_min_r]) {
                    s_degrees[idx_neighbor_r]--;
                }
            }
        }
    }

    // Just Printing
    int num_printed = 0;
    int k_value = 0;

    while (num_printed < num_r_cliques) {
        int counter = 0;

        for (int idx_k = 0; idx_k < num_r_cliques; idx_k++) {
            if (k_values[idx_k] == k_value) {
                counter++;
                num_printed++;
            }
        }

        printf("%d: %d\n", k_value, counter);
        k_value++;
    }

    return k_values;
}