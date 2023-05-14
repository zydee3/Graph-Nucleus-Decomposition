#include "clique_set.h"

/**
 * @brief Element-wise compares clique_a and clique_b, returning -1
 * if clique_a < clique_b, 0 if clique_a == clique_b, and 1 if
 * clique_a > clique_b.
 *
 * @param clique_a
 * @param clique_b
 * @param k
 * @return int
 */
int _compare_cliques(clique clique_a, clique clique_b, int k) {
    assert(clique_a != NULL);
    assert(clique_b != NULL);
    assert(k > 0);

    for (int i = 0; i < k; i++) {
        if (clique_a[i] < clique_b[i]) {
            return -1;
        }

        if (clique_a[i] > clique_b[i]) {
            return 1;
        }
    }

    return 0;
}

CliqueSet* clique_set_new(int k, int resize_value) {
    assert(k > 0);
    assert(resize_value > 0);

    CliqueSet* clique_set = malloc(sizeof(CliqueSet));
    clique_set->k = k;
    clique_set->size = 0;
    clique_set->capacity = resize_value;
    clique_set->resize_value = resize_value;
    clique_set->cliques = malloc(sizeof(clique) * clique_set->resize_value);

    return clique_set;
}

CliqueSet* clique_set_copy(CliqueSet* clique_set) {
    assert(clique_set != NULL);

    CliqueSet* copy = clique_set_new(clique_set->k, clique_set->resize_value);
    copy->cliques = realloc(copy->cliques, sizeof(clique) * clique_set->capacity);
    assert(copy->cliques != NULL);

    for (int i = 0; i < clique_set->size; i++) {
        clique curr_clique = clique_set->cliques[i];
        clique copy_clique = malloc(sizeof(int) * clique_set->k);
        memcpy(copy_clique, curr_clique, sizeof(vertex) * clique_set->k);
        copy->cliques[i] = copy_clique;
    }

    return copy;
}

void clique_set_delete(CliqueSet** ptr_clique_set) {
    assert(ptr_clique_set != NULL);
    assert(*ptr_clique_set != NULL);

    CliqueSet* clique_set = *ptr_clique_set;

    for (int i = 0; i < clique_set->size; i++) {
        free(clique_set->cliques[i]);
    }

    free(clique_set->cliques);
    free(*ptr_clique_set);

    *ptr_clique_set = NULL;
}

bool clique_set_insert(CliqueSet* clique_set, clique clique_to_insert) {
    assert(clique_set != NULL);
    assert(clique_to_insert != NULL);

    // clique[0] < clique[1] < ... < clique[k]
    // Necessary to ensure the clique set is sorted for binary search
    // Binary search used for insert and membership operations
    qsort(clique_to_insert, clique_set->k, sizeof(int), cmp_ints_asc);

    if (clique_set->size == clique_set->capacity) {
        clique_set->capacity += clique_set->resize_value;
        clique_set->cliques = realloc(clique_set->cliques, sizeof(clique_to_insert) * clique_set->capacity);
        assert(clique_set->cliques != NULL);
    }

    if (clique_set->size == 0) {
        clique_set->cliques[0] = clique_to_insert;
        clique_set->size++;
        return true;
    }

    int idx_lower_bound = 0;
    int idx_upper_bound = clique_set->size - 1;

    while (idx_lower_bound <= idx_upper_bound) {
        int idx_middle = (idx_lower_bound + idx_upper_bound) / 2;
        clique curr_clique = clique_set->cliques[idx_middle];

        int comparison = _compare_cliques(clique_to_insert, curr_clique, clique_set->k);

        if (comparison == 0) {
            return false;
        }

        if (comparison == -1) {
            idx_upper_bound = idx_middle - 1;
        } else {
            idx_lower_bound = idx_middle + 1;
        }
    }

    if (idx_lower_bound < clique_set->size) {
        int num_cliques_to_move = clique_set->size - idx_lower_bound;
        int num_bytes_to_move = sizeof(clique) * num_cliques_to_move;
        clique* src = &clique_set->cliques[idx_lower_bound];
        clique* dst = &clique_set->cliques[idx_lower_bound + 1];
        memmove(dst, src, num_bytes_to_move);
    }

    clique_set->cliques[idx_lower_bound] = clique_to_insert;
    clique_set->size++;
    return true;
}

void clique_set_fit(CliqueSet* clique_set) {
    assert(clique_set != NULL);

    if (clique_set->size == clique_set->capacity) {
        return;
    }

    clique_set->capacity = clique_set->size;
    clique_set->cliques = realloc(clique_set->cliques, sizeof(clique) * clique_set->capacity);
    assert(clique_set->cliques != NULL);
}

void clique_set_print(CliqueSet* clique_set, bool should_print_newline) {
    assert(clique_set != NULL);
    printf("Cliques: { Size: %d, Elements: [", clique_set->size);
    for (int i = 0; i < clique_set->size; i++) {
        printf("(");
        array_print(clique_set->cliques[i], clique_set->k, false);

        if (i == clique_set->size - 1) {
            printf(")");
        } else {
            printf("),");
        }
    }

    printf("] }");

    if (should_print_newline) {
        printf("\n");
    }
}

void clique_set_print_n(CliqueSet* clique_set) {
    clique_set_print(clique_set, true);
}

bool clique_set_is_equal(CliqueSet* clique_set_a, CliqueSet* clique_set_b) {
    if (clique_set_a == NULL && clique_set_b == NULL) {
        return true;
    }

    if (clique_set_a == NULL || clique_set_b == NULL) {
        return false;
    }

    if (clique_set_a->size != clique_set_b->size) {
        return false;
    }

    for (int i = 0; i < clique_set_a->size; i++) {
        clique clique_a = clique_set_a->cliques[i];
        clique clique_b = clique_set_b->cliques[i];

        assert(clique_a != NULL);
        assert(clique_b != NULL);

        if (memcmp(clique_a, clique_b, sizeof(int) * clique_set_a->k) != 0) {
            return false;
        }
    }

    return true;
}