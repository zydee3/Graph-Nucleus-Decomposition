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

/**
 * @brief Create a new CliqueSet object.
 *
 * @param k The size of each clique in the set.
 * @param resize_value The resize amount is the value by which
 * the capacity of the queue is increased or decreased when the queue
 * is full or empty, respectively.
 * @return CliqueSet* The new clique set.
 */
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

/**
 * @brief Create a deep copy of a CliqueSet object.
 *
 * The function creates a new clique size with the same k and resize
 * value as the original clique set. The function then copies each
 * clique in the original clique set into the new clique set by
 * iterating through the set and copying each clique into the new
 * array and inserting the new clique set into the new clique set.
 *
 * @param clique_set The clique set to copy.
 * @return CliqueSet* The deep copy of the clique set.
 */
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

/**
 * @brief Delete a CliqueSet object and all associated memory.
 *
 * This function deletes all cliques in the clique set and then
 * deletes the array of cliques and the clique set itself. The pointer
 * to the clique set is set to NULL.
 *
 * @param ptr_clique_set A pointer to the clique set to delete.
 */
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

/**
 * @brief Insert a clique into a clique set.
 *
 * Since each clique is ordered, binary sort is used to find the
 * correct position to insert the clique. If the clique is already
 * in the clique set, the function returns false. Otherwise, the
 * clique is inserted into the clique set and the function returns
 * true. Note, although the look up is O(log(n)), the insert is
 * O(n) since the array must be shifted to make room for the new
 * clique.
 *
 * @param clique_set The clique set to insert the clique into.
 * @param clique_to_insert The clique to insert into the clique set.
 * @return bool True if the clique was inserted, false if the clique
 * was already in the clique set.
 */
bool clique_set_insert(CliqueSet* clique_set, clique clique_to_insert) {
    assert(clique_set != NULL);
    assert(clique_to_insert != NULL);

    // clique[0] < clique[1] < ... < clique[k]
    // Necessary to ensure the clique set is sorted for binary search
    // Binary search used for insert and membership operations
    qsort(clique_to_insert, clique_set->k, sizeof(int), cmp_ints_asc);

    // If the clique set is full, resize the clique set to make room
    // for the new clique.
    if (clique_set->size == clique_set->capacity) {
        clique_set->capacity += clique_set->resize_value;
        clique_set->cliques = realloc(clique_set->cliques, sizeof(clique_to_insert) * clique_set->capacity);
        assert(clique_set->cliques != NULL);
    }

    // If the clique set is empty, insert the clique at the first
    // position.
    if (clique_set->size == 0) {
        clique_set->cliques[0] = clique_to_insert;
        clique_set->size++;
        return true;
    }

    // Binary search to find the correct position to insert the clique
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

    // If the clique is to be inserted in the middle of the clique
    // set, shift the array to make room for the new clique.
    if (idx_lower_bound < clique_set->size) {
        int num_cliques_to_move = clique_set->size - idx_lower_bound;
        int num_bytes_to_move = sizeof(clique) * num_cliques_to_move;
        clique* src = &clique_set->cliques[idx_lower_bound];
        clique* dst = &clique_set->cliques[idx_lower_bound + 1];
        memmove(dst, src, num_bytes_to_move);
    }

    // Insert the clique into the clique set and increment the size.
    clique_set->cliques[idx_lower_bound] = clique_to_insert;
    clique_set->size++;
    return true;
}

/**
 * @brief Frees all unused memory in a clique set.
 *
 * Since the resize amount can be larger than 1, it is possible
 * that the clique set has unused memory. This function frees
 * the unused memory and sets the capacity to the current size of the
 * clique set.
 *
 * @param clique_set The clique set to free unused memory from.
 */
void clique_set_fit(CliqueSet* clique_set) {
    assert(clique_set != NULL);

    // If the clique set is already at capacity, then there is no
    // unused memory to free.
    if (clique_set->size == clique_set->capacity) {
        return;
    }

    clique_set->capacity = clique_set->size;
    clique_set->cliques = realloc(clique_set->cliques, sizeof(clique) * clique_set->capacity);
    assert(clique_set->cliques != NULL);
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

/**
 * @brief Prints a clique set to stdout.
 *
 * @param clique_set The clique set to print.
 * @param should_print_newline True if a newline should be printed
 * at the end of all print statements, false otherwise.
 */
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

/**
 * @brief Prints a clique set to stdout with a newline.
 *
 * This is primarily used for generic collections to match the
 * expected function signiture (void (*)(void*)).
 *
 * @param clique_set The clique set to print.
 */
void clique_set_print_n(CliqueSet* clique_set) {
    clique_set_print(clique_set, true);
}