#include "nucleus_decomposition.h"

int _compute_s_degree(CliqueSet* s_cliques, clique r_clique, int r_k) {
    assert(s_cliques != NULL);
    assert(r_clique != NULL);

    int s_degree = 0;

    for (int i = 0; i < s_cliques->size; i++) {
        clique s_clique = s_cliques->cliques[i];

        if (array_count_symmetric_difference(s_clique, r_clique, s_cliques->k, r_k) == 1) {
            s_degree++;
        }
    }

    return s_degree;
}

int* _compute_all_s_degrees(CliqueSet* s_cliques, CliqueSet* r_cliques) {
    assert(s_cliques != NULL);
    assert(r_cliques != NULL);

    int* s_degrees = (int*)calloc(r_cliques->size, sizeof(int));

    for (int idx_r_clique = 0; idx_r_clique < r_cliques->size; idx_r_clique++) {
        s_degrees[idx_r_clique] = _compute_s_degree(s_cliques, r_cliques->cliques[idx_r_clique], r_cliques->k);
    }

    return s_degrees;
}

bool _is_s_clique_processed(OrderedSet* s_map, bool* is_processed) {
    for (int idx_s_map = 0; idx_s_map < s_map->size; idx_s_map++) {
        int idx_neighbor_r = s_map->elements[idx_s_map];
        if (is_processed[idx_neighbor_r]) {
            return true;
        }
    }

    return false;
}

MappedCliques* _map_r_s_cliques(CliqueSet* s_cliques, CliqueSet* r_cliques) {
    assert(s_cliques != NULL);
    assert(r_cliques != NULL);

    int len_s_clique = s_cliques->k;
    int len_r_clique = r_cliques->k;

    int num_s_cliques = s_cliques->size;
    int num_r_cliques = r_cliques->size;

    MappedCliques* mapped_cliques = (MappedCliques*)malloc(sizeof(MappedCliques));
    mapped_cliques->s_map = (OrderedSet**)calloc(num_s_cliques, sizeof(OrderedSet*));
    mapped_cliques->r_map = (OrderedSet**)calloc(num_r_cliques, sizeof(OrderedSet*));

    for (int idx_r_clique = 0; idx_r_clique < num_r_cliques; idx_r_clique++) {
        clique r_clique = r_cliques->cliques[idx_r_clique];
        OrderedSet* r_map = mapped_cliques->r_map[idx_r_clique];

        if (r_map == NULL) {
            r_map = ordered_set_new(1);
            mapped_cliques->r_map[idx_r_clique] = r_map;
        }

        for (int idx_s_clique = 0; idx_s_clique < num_s_cliques; idx_s_clique++) {
            clique s_clique = s_cliques->cliques[idx_s_clique];
            OrderedSet* s_map = mapped_cliques->s_map[idx_s_clique];

            if (s_map == NULL) {
                s_map = ordered_set_new(1);
                mapped_cliques->s_map[idx_s_clique] = s_map;
            }

            if (array_count_symmetric_difference(s_clique, r_clique, len_s_clique, len_r_clique) == 1) {
                ordered_set_insert(r_map, idx_s_clique);
                ordered_set_insert(s_map, idx_r_clique);
            }
        }
    }

    return mapped_cliques;
}

void _print_k_value_count(int* k_values, int num_k_values) {
    int num_printed = 0;
    int k_value = 0;

    while (num_printed < num_k_values) {
        int num_at_k = 0;

        for (int idx_k = 0; idx_k < num_k_values; idx_k++) {
            if (k_values[idx_k] == k_value) {
                num_at_k++;
            }
        }

        printf("%d: %d\n", k_value, num_at_k);
        num_printed += num_at_k;
        k_value++;
    }
}

int* set_k(CliqueSet* s_cliques, CliqueSet* r_cliques) {
    assert(s_cliques != NULL);
    assert(r_cliques != NULL);

    int* k_values = (int*)calloc(r_cliques->size, sizeof(int));
    int* s_degrees = _compute_all_s_degrees(s_cliques, r_cliques);

    int num_processed = 0;
    bool* is_processed = (bool*)calloc(r_cliques->size, sizeof(bool));

    MappedCliques* mapped_cliques = _map_r_s_cliques(s_cliques, r_cliques);
    OrderedSet** s_map = mapped_cliques->s_map;
    OrderedSet** r_map = mapped_cliques->r_map;

    while (num_processed < r_cliques->size) {
        int idx_min_s_degree = array_filtered_argmin(s_degrees, r_cliques->size, is_processed);
        k_values[idx_min_s_degree] = s_degrees[idx_min_s_degree];

        OrderedSet* curr_r_map = r_map[idx_min_s_degree];
        for (int idx_r_map = 0; idx_r_map < curr_r_map->size; idx_r_map++) {
            int idx_s_clique = curr_r_map->elements[idx_r_map];
            OrderedSet* curr_s_map = s_map[idx_s_clique];

            if (_is_s_clique_processed(curr_s_map, is_processed)) {
                continue;
            }

            for (int idx_s_map = 0; idx_s_map < curr_s_map->size; idx_s_map++) {
                int idx_neighbor_r_clique = curr_s_map->elements[idx_s_map];
                if (s_degrees[idx_neighbor_r_clique] > s_degrees[idx_min_s_degree]) {
                    s_degrees[idx_neighbor_r_clique]--;
                }
            }
        }

        is_processed[idx_min_s_degree] = true;
        num_processed++;
    }

    for (int i = 0; i < r_cliques->size; i++) {
        ordered_set_delete(&r_map[i]);
    }

    for (int i = 0; i < s_cliques->size; i++) {
        ordered_set_delete(&s_map[i]);
    }

    free(r_map);
    free(s_map);
    free(mapped_cliques);
    free(s_degrees);
    free(is_processed);

    return k_values;
}

int* set_k_updateless(CliqueSet* s_cliques, CliqueSet* r_cliques) {
    assert(s_cliques != NULL);
    assert(r_cliques != NULL);

    int num_r_cliques = r_cliques->size;

    int* k_values = (int*)calloc(num_r_cliques, sizeof(int));
    int* s_degrees = _compute_all_s_degrees(s_cliques, r_cliques);

    MappedCliques* mapped_cliques = _map_r_s_cliques(s_cliques, r_cliques);
    OrderedSet** s_map = mapped_cliques->s_map;
    OrderedSet** r_map = mapped_cliques->r_map;

    int* processing_order = array_generate_sequence(0, 1, num_r_cliques);
    array_parallel_sort_2(processing_order, s_degrees, num_r_cliques, num_r_cliques, false);

    for (int i = 0; i < num_r_cliques; i++) {
        int idx_next_r_clique = processing_order[i];
        k_values[idx_next_r_clique] = s_degrees[idx_next_r_clique];

        OrderedSet* curr_r_map = r_map[idx_next_r_clique];
        for (int idx_r_map = 0; idx_r_map < curr_r_map->size; idx_r_map++) {
            int idx_s_clique = curr_r_map->elements[idx_r_map];
            OrderedSet* curr_s_map = s_map[idx_s_clique];

            bool is_processed = false;

            for (int idx_s_map = 0; idx_s_map < curr_s_map->size; idx_s_map++) {
                int idx_neighbor_r_clique = curr_s_map->elements[idx_s_map];
                if (idx_neighbor_r_clique > idx_next_r_clique) {
                    is_processed = true;
                    break;
                }
            }

            if (is_processed) {
                continue;
            }

            for (int idx_s_map = 0; idx_s_map < curr_s_map->size; idx_s_map++) {
                int idx_neighbor_r_clique = curr_s_map->elements[idx_s_map];
                if (s_degrees[idx_neighbor_r_clique] > s_degrees[idx_next_r_clique]) {
                    s_degrees[idx_neighbor_r_clique]--;
                }
            }
        }
    }

    for (int i = 0; i < r_cliques->size; i++) {
        ordered_set_delete(&r_map[i]);
    }

    for (int i = 0; i < s_cliques->size; i++) {
        ordered_set_delete(&s_map[i]);
    }

    free(r_map);
    free(s_map);
    free(mapped_cliques);
    free(s_degrees);

    return k_values;
}

int* set_k_limited(CliqueSet* s_cliques, CliqueSet* r_cliques, int s_limit) {
    assert(s_cliques != NULL);
    assert(r_cliques != NULL);

    int* k_values = (int*)calloc(r_cliques->size, sizeof(int));
    int* s_degrees = _compute_all_s_degrees(s_cliques, r_cliques);

    int num_processed = 0;
    bool* is_processed = (bool*)calloc(r_cliques->size, sizeof(bool));

    MappedCliques* mapped_cliques = _map_r_s_cliques(s_cliques, r_cliques);
    OrderedSet** s_map = mapped_cliques->s_map;
    OrderedSet** r_map = mapped_cliques->r_map;

    while (num_processed < r_cliques->size) {
        int idx_min_s_degree = array_filtered_argmin(s_degrees, r_cliques->size, is_processed);
        k_values[idx_min_s_degree] = s_degrees[idx_min_s_degree];

        OrderedSet* curr_r_map = r_map[idx_min_s_degree];
        for (int idx_r_map = 0; idx_r_map < min(s_limit, curr_r_map->size); idx_r_map++) {
            int idx_s_clique = curr_r_map->elements[idx_r_map];
            OrderedSet* curr_s_map = s_map[idx_s_clique];

            if (_is_s_clique_processed(curr_s_map, is_processed)) {
                continue;
            }

            for (int idx_s_map = 0; idx_s_map < curr_s_map->size; idx_s_map++) {
                int idx_neighbor_r_clique = curr_s_map->elements[idx_s_map];
                if (s_degrees[idx_neighbor_r_clique] > s_degrees[idx_min_s_degree]) {
                    s_degrees[idx_neighbor_r_clique]--;
                }
            }
        }

        is_processed[idx_min_s_degree] = true;
        num_processed++;
    }

    for (int i = 0; i < r_cliques->size; i++) {
        ordered_set_delete(&r_map[i]);
    }

    for (int i = 0; i < s_cliques->size; i++) {
        ordered_set_delete(&s_map[i]);
    }

    free(r_map);
    free(s_map);
    free(mapped_cliques);
    free(s_degrees);
    free(is_processed);

    return k_values;
}

int* set_k_monte_carlo(CliqueSet* s_cliques, CliqueSet* r_cliques, int num_samples) {
    assert(s_cliques != NULL);
    assert(r_cliques != NULL);

    int* k_values = (int*)calloc(r_cliques->size, sizeof(int));
    int* s_degrees = _compute_all_s_degrees(s_cliques, r_cliques);

    for (int i = 0; i < num_samples; i++) {
        int idx_random_r_clique = rand() % r_cliques->size;
        k_values[idx_random_r_clique] += s_degrees[idx_random_r_clique];
    }

    for (int idx_r_clique = 0; idx_r_clique < r_cliques->size; idx_r_clique++) {
        k_values[idx_r_clique] /= num_samples;
    }

    free(s_degrees);

    return k_values;
}

int* set_k_updateless_limited(CliqueSet* s_cliques, CliqueSet* r_cliques, int limit) {
    assert(s_cliques != NULL);
    assert(r_cliques != NULL);

    int num_r_cliques = r_cliques->size;

    int* k_values = (int*)calloc(num_r_cliques, sizeof(int));
    int* s_degrees = _compute_all_s_degrees(s_cliques, r_cliques);

    MappedCliques* mapped_cliques = _map_r_s_cliques(s_cliques, r_cliques);
    OrderedSet** s_map = mapped_cliques->s_map;
    OrderedSet** r_map = mapped_cliques->r_map;

    int* processing_order = array_generate_sequence(0, 1, num_r_cliques);
    array_parallel_sort_2(processing_order, s_degrees, num_r_cliques, num_r_cliques, true);

    for (int i = 0; i < num_r_cliques; i++) {
        int idx_next_r_clique = processing_order[i];
        k_values[idx_next_r_clique] = s_degrees[idx_next_r_clique];

        OrderedSet* curr_r_map = r_map[idx_next_r_clique];
        for (int idx_r_map = 0; idx_r_map < min(limit, curr_r_map->size); idx_r_map++) {
            int idx_s_clique = curr_r_map->elements[idx_r_map];
            OrderedSet* curr_s_map = s_map[idx_s_clique];

            bool is_processed = false;

            for (int idx_s_map = 0; idx_s_map < curr_s_map->size; idx_s_map++) {
                int idx_neighbor_r_clique = curr_s_map->elements[idx_s_map];
                if (idx_neighbor_r_clique > idx_next_r_clique) {
                    is_processed = true;
                    break;
                }
            }

            if (is_processed) {
                continue;
            }

            for (int idx_s_map = 0; idx_s_map < curr_s_map->size; idx_s_map++) {
                int idx_neighbor_r_clique = curr_s_map->elements[idx_s_map];
                if (s_degrees[idx_neighbor_r_clique] > s_degrees[idx_next_r_clique]) {
                    s_degrees[idx_neighbor_r_clique]--;
                }
            }
        }
    }

    for (int i = 0; i < r_cliques->size; i++) {
        ordered_set_delete(&r_map[i]);
    }

    for (int i = 0; i < s_cliques->size; i++) {
        ordered_set_delete(&s_map[i]);
    }

    free(r_map);
    free(s_map);
    free(mapped_cliques);
    free(s_degrees);

    return k_values;
}

void run_nucleus_decomposition_3_4(Graph* graph) {
    assert(graph != NULL);

    Stopwatch* stopwatch = stopwatch_new();

    // CliqueSet* triangles = find_k_cliques(graph, 3);
    // printf("Computed %d triangles in %.2f seconds.\n", triangles->size, stopwatch_lap(stopwatch));

    CliqueSet* squares = find_k_cliques(graph, 4);
    printf("Computed %d squares in %.2f seconds.\n", squares->size, stopwatch_lap(stopwatch));

    // clique_set_print(triangles, true);
    // clique_set_print(squares, true);

    // int* k_values = set_k(squares, triangles);
    // // printf("\nComputed k values in %.2f seconds.\n", stopwatch_lap(stopwatch));
    // // _print_k_value_count(k_values, triangles->size);
    // // array_print(k_values, triangles->size, true);

    // int* k_values_updateless = set_k_updateless(squares, triangles);
    // // printf("\nComputed updateless k values in %.2f seconds.\n", stopwatch_lap(stopwatch));
    // // _print_k_value_count(k_values_updateless, triangles->size);
    // // array_print(k_values_updateless, triangles->size, true);

    // int num_r_cliques = triangles->size;

    // // for (int num_samples = num_r_cliques / 10; num_samples <= num_r_cliques * 10; num_samples *= 2) {
    // //     int* k_values_monte_carlo = set_k_monte_carlo(squares, triangles, num_samples);
    // //     _print_k_value_count(k_values_monte_carlo, triangles->size);
    // //     printf("\nComputed monte carlo %d draws for k values in %.2f seconds.\n", num_samples, stopwatch_lap(stopwatch));
    // // }

    // // for (int s_limit = 1; s_limit < 5; s_limit++) {
    // //     int* k_values_limited = set_k_limited(squares, triangles, s_limit);
    // //     printf("\nComputed limited k values with s_limit %d in %.2f seconds.\n", s_limit, stopwatch_lap(stopwatch));
    // //     _print_k_value_count(k_values_limited, triangles->size);
    // // }

    // int max_k_value = 0;
    // for (int i = 0; i < num_r_cliques; i++) {
    //     max_k_value = max(max_k_value, k_values[i]);
    // }

    // max_k_value++;

    // double* densities = (double*)calloc(max_k_value, sizeof(double));

    // for (int i = 0; i < max_k_value; i++) {
    //     int actual_count = 0;
    //     int estimated_count = 0;

    //     for (int j = 0; j < num_r_cliques; j++) {
    //         if (k_values[j] == i) {
    //             actual_count++;
    //         }

    //         if (k_values_updateless[j] == i) {
    //             estimated_count++;
    //         }
    //     }

    //     densities[i] = (double)estimated_count / (double)actual_count;
    // }

    // for (int i = 0; i < max_k_value; i++) {
    //     printf("%d: %.2f\n", i, densities[i]);
    // }

    // printf("\n");

    graph_delete(&graph);
    // clique_set_delete(&triangles);
    clique_set_delete(&squares);
    stopwatch_delete(&stopwatch);
}