#include "three_four_cliques.h"

/**
 * @brief Creates a new collection of three-cliques and
 * four-cliques.
 *
 * @param is_storing_cliques True if the collection should store
 * the three-cliques and four-cliques, false otherwise.
 * @return ThreeFourCliques* The new collection of three-cliques
 * and four-cliques.
 */
ThreeFourCliques* three_four_cliques_new(bool is_storing_cliques) {
    ThreeFourCliques* three_four_cliques = malloc(sizeof(ThreeFourCliques));

    three_four_cliques->is_storing_cliques = is_storing_cliques;

    three_four_cliques->num_three_cliques = 0;
    three_four_cliques->num_four_cliques = 0;

    three_four_cliques->three_cliques = NULL;
    three_four_cliques->four_cliques = NULL;

    return three_four_cliques;
}

/**
 * @brief Deletes the current collection of three-cliques and
 * four-cliques.
 *
 * @param ptr_three_four_cliques The current collection of
 * three-cliques and four-cliques.
 */
void three_four_cliques_delete(ThreeFourCliques** ptr_three_four_cliques) {
    ThreeFourCliques* three_four_cliques = *ptr_three_four_cliques;

    if (three_four_cliques->is_storing_cliques) {
        for (int i = 0; i < three_four_cliques->num_three_cliques; i++) {
            free(three_four_cliques->three_cliques[i]);
        }

        for (int i = 0; i < three_four_cliques->num_four_cliques; i++) {
            free(three_four_cliques->four_cliques[i]);
        }

        free(three_four_cliques->three_cliques);
        free(three_four_cliques->four_cliques);
    }

    free(three_four_cliques);
    *ptr_three_four_cliques = NULL;
}

/**
 * @brief Records a three-clique or four-clique in the current
 * collection (three_four_cliques).
 *
 * Counts the number of three-cliques and four-cliques in the graph.
 * If is_storing_cliques is true, then the three-cliques and
 * four-cliques are stored in the current collection. If
 * is_storing_cliques is false, then the vertices are not stored and
 * ignored.
 *
 * @param ptr_three_four_cliques
 * @param u
 * @param v
 * @param w
 * @param x A sentinel value. If x == -1, then the vertices u, v, and
 * w form a three-clique. Otherwise, when x > 0, the vertices u, v, w,
 * and x form a four-clique. Sort of a hacky way to avoid having to
 * write two functions.
 */
void three_four_cliques_record(void* ptr_three_four_cliques, vertex u, vertex v, vertex w, vertex x) {
    assert(ptr_three_four_cliques != NULL);

    ThreeFourCliques* three_four_cliques = ptr_three_four_cliques;

    bool is_three_clique = (x == -1);

    if (is_three_clique) {
        three_four_cliques->num_three_cliques++;
    } else {
        three_four_cliques->num_four_cliques++;
    }

    if (three_four_cliques->is_storing_cliques == false) {
        return;
    }

    if (is_three_clique) {
        clique three_clique = calloc(3, sizeof(vertex));
        three_clique[0] = min_3(u, v, w);
        three_clique[2] = max_3(u, v, w);
        three_clique[1] = u + v + w - three_clique[0] - three_clique[2];

        three_four_cliques->three_cliques = realloc(three_four_cliques->three_cliques, sizeof(clique) * three_four_cliques->num_three_cliques);
        three_four_cliques->three_cliques[three_four_cliques->num_three_cliques - 1] = three_clique;
    } else {
        clique four_clique = calloc(4, sizeof(vertex));
        four_clique[0] = u;
        four_clique[1] = v;
        four_clique[2] = w;
        four_clique[3] = x;

        qsort(four_clique, 4, sizeof(vertex), cmp_ints_asc);

        three_four_cliques->four_cliques = realloc(three_four_cliques->four_cliques, sizeof(clique) * three_four_cliques->num_four_cliques);
        three_four_cliques->four_cliques[three_four_cliques->num_four_cliques - 1] = four_clique;
    }
}

/**
 * @brief Prints the number of 3,4-cliques and optionally the cliques
 * themselves.
 *
 * @param collector The 3,4-clique collection.
 * @param print_cliques True if the cliques should be printed,
 * false otherwise.
 */
void three_four_cliques_print(ThreeFourCliques* collector, bool print_cliques) {
    assert(collector != NULL);

    if (collector->is_storing_cliques == false || print_cliques == false) {
        printf("3,4-Cliques { 3-Cliques: %d, 4-Cliques: %d }\n", collector->num_three_cliques, collector->num_four_cliques);
        return;
    }

    printf("3,4-Cliques { \n\n");
    printf("3-Cliques(%d): ", collector->num_three_cliques);
    for (int i = 0; i < collector->num_three_cliques; i++) {
        printf("(%d, %d, %d)", collector->three_cliques[i][0], collector->three_cliques[i][1], collector->three_cliques[i][2]);
        if (i < collector->num_three_cliques - 1) {
            printf(", ");
        }
    }

    printf("\n\n4-Cliques(%d): ", collector->num_four_cliques);
    for (int i = 0; i < collector->num_four_cliques; i++) {
        printf("(%d, %d, %d, %d)", collector->four_cliques[i][0], collector->four_cliques[i][1], collector->four_cliques[i][2], collector->four_cliques[i][3]);
        if (i < collector->num_four_cliques - 1) {
            printf(", ");
        }
    }

    printf("\n\n}\n\n");
}