#include "clique_expansion.h"

void _empty_delete_function(void** data) {
    assert(data != NULL);
    return;
}

/**
 * @brief Inserts param clique_candidate into param clique_group if
 * clique_candidate is adjacent to all cliques in clique_group.
 *
 * @param clique_group A Linked List of Ordered Sets
 * @param clique_candidate A clique that needs to be grouped.
 * @return bool True if clique_candidate was inserted into a clique
 * group or if the clique_candidate was already in a clique group,
 * false otherwise.
 */
bool _insert_clique_into_group(GenericLinkedList* clique_group, OrderedSet* clique_candidate) {
    assert(clique_group != NULL);
    assert(clique_candidate != NULL);
    assert(clique_group->size > 0);

    GenericNode* prev_clique_node = NULL;
    GenericNode* curr_clique_node = clique_group->head;

    while (curr_clique_node != NULL) {
        OrderedSet* curr_clique = (OrderedSet*)curr_clique_node->data;
        assert(curr_clique != NULL);

        int size_symm_diff = array_count_symmetric_difference(curr_clique->elements, clique_candidate->elements, curr_clique->size, clique_candidate->size);

        // OrderedSet* symm_diff = ordered_set_symmetric_difference(curr_clique, clique_candidate);
        // assert(symm_diff != NULL);

        // int symm_diff_size = symm_diff->size;
        // ordered_set_delete(&symm_diff);

        // if the size is 0, then the cliques are equal
        if (size_symm_diff == 0) {
            return true;
        }

        // if the size is 1, then we're taking the symmetric
        // difference of two cliques with different sizes, which is
        // impossible as all cliques should be of size k.
        assert(size_symm_diff != 1);

        // if the size is not 2, then the cliques are not adjacent.
        // actually, this is a redundant check as this would have
        // failed in any iteration (can be shown inductively)
        if (size_symm_diff > 2) {
            return false;
        }

        prev_clique_node = curr_clique_node;
        curr_clique_node = curr_clique_node->next;
    }

    assert(prev_clique_node->next == NULL);
    prev_clique_node->next = generic_node_new(clique_candidate);
    clique_group->size++;

    return true;
}

/**
 * @brief Creates a new linked list and inserts param clique_candidate
 * into the new linked list.
 *
 * @param clique_candidate The parent clique of the new group.
 * @return GenericLinkedList* The new group containing the single
 * parent clique (param clique_candidate).
 */
GenericLinkedList* _create_new_clique_group(OrderedSet* clique_candidate) {
    assert(clique_candidate != NULL);

    GenericLinkedList* clique_group = generic_linked_list_new(
        ordered_set_generic_copy,
        _empty_delete_function,
        ordered_set_generic_is_equal,
        ordered_set_generic_print);

    generic_linked_list_insert(clique_group, clique_candidate);

    return clique_group;
}

/**
 * @brief Given a list of all clique groups and a clique candidate,
 * inserts the clique candidate into the first clique group that it
 * is adjacent to, or creates a new clique group if the clique
 * candidate is not adjacent to any clique groups.
 *
 * This function iterates through all clique groups and checks if the
 * clique candidate is adjacent to any of the clique groups.
 * Adjacency is checked by taking the symmetric difference of the
 * clique candidate and each clique in the clique group. Let D be the
 * length of the symmetric difference of any two cliques.
 *
 * If D=0, then the clique candidate is equal to at least one clique
 * in the clique group.
 *
 * If D=1, then the clique candidate has a different k value than the
 * clique group. This is impossible as all cliques should be of size k.
 * An assertion is thrown.
 *
 * If D=2, then the clique candidate is adjacent to the clique group.
 *
 * If D>2, then the clique candidate is not adjacent to the clique
 * group.
 *
 * @param list_clique_group The list of all k cliques.
 * @param clique_candidate The clique that needs to be grouped.
 */
void _insert_clique_into_group_list(GenericLinkedList* list_clique_group, OrderedSet* clique_candidate) {
    GenericNode* prev_group_node = NULL;
    GenericNode* curr_group_node = list_clique_group->head;

    while (curr_group_node != NULL) {
        GenericLinkedList* curr_group = (GenericLinkedList*)curr_group_node->data;
        assert(curr_group != NULL);

        bool was_clique_inserted = _insert_clique_into_group(curr_group, clique_candidate);

        if (was_clique_inserted) {
            return;
        }

        prev_group_node = curr_group_node;
        curr_group_node = curr_group_node->next;
    }

    // If the clique was not inserted into any groups, then create a
    // new group containing only the candidate clique.
    GenericLinkedList* new_clique_group = _create_new_clique_group(clique_candidate);
    GenericNode* new_clique_group_node = generic_node_new(new_clique_group);

    if (prev_group_node == NULL) {
        list_clique_group->head = new_clique_group_node;
    } else {
        prev_group_node->next = new_clique_group_node;
    }

    list_clique_group->size++;
}

/**
 * @brief Groups all cliques in param k_cliques into clique groups
 * of adjacent cliques.
 *
 * This function iterates through all k-cliques and attempts to insert
 * each clique into a clique group. If the clique is adjacent to all
 * cliques in a clique group, then the clique is inserted into the
 * clique group. If the clique is not adjacent to any clique in any
 * clique group, then a new clique group is created containing only
 * the clique and appended to the resulting list of clique groups.
 *
 * @param k_cliques All cliques of size k to be grouped.
 * @return GenericLinkedList* A list of clique groups where each
 * clique group is a list of cliques (i.e., a 2D list of cliques).
 */
GenericLinkedList* group_k_cliques(GenericLinkedList* k_cliques) {
    GenericLinkedList* list_grouped_cliques = generic_linked_list_new(
        generic_generic_linked_list_copy,
        generic_generic_linked_list_delete,
        generic_generic_linked_list_is_equal,
        generic_generic_linked_list_print);

    GenericNode* curr_clique_node = k_cliques->head;

    // For each clique
    while (curr_clique_node != NULL) {
        OrderedSet* curr_clique = (OrderedSet*)curr_clique_node->data;
        assert(curr_clique != NULL);

        // Insert the clique into a clique group or create a new
        // clique group containing only the clique.
        _insert_clique_into_group_list(list_grouped_cliques, curr_clique);
        curr_clique_node = curr_clique_node->next;
    }

    return list_grouped_cliques;
}

/**
 * @brief Reduces all k-clique groups in param grouped_k_cliques to
 * a list of (k+1)-cliques.
 *
 * A single group of k-cliques is reducible if it contains at least
 * (k+1 choose k) k-cliques. If a group is reducible then a
 * (k+1)-clique is created by taking the union of any two k-cliques in
 * the group.
 *
 * This proceedure is repeated for all k-clique groups in the list.
 *
 * @param grouped_k_cliques A list of all grouped k-cliques.
 * @param k The size of the cliques in param grouped_k_cliques.
 * @return GenericLinkedList* A list of all (k+1)-cliques.
 */
GenericLinkedList* reduce_grouped_k_cliques(GenericLinkedList* grouped_k_cliques, int k) {
    GenericLinkedList* k_plus_one_cliques = generic_linked_list_new(
        ordered_set_generic_copy,
        ordered_set_generic_delete,
        ordered_set_generic_is_equal,
        ordered_set_generic_print);

    GenericNode* curr_k_plus_one_clique_node = grouped_k_cliques->head;
    while (curr_k_plus_one_clique_node != NULL) {
        GenericLinkedList* curr_k_plus_one_clique = (GenericLinkedList*)curr_k_plus_one_clique_node->data;

        // (k+1) choose k = (k+1)
        if (curr_k_plus_one_clique->size >= k + 1) {
            GenericNode* node_first = curr_k_plus_one_clique->head;
            GenericNode* node_second = node_first->next;

            OrderedSet* k_plus_one_clique = ordered_set_union((OrderedSet*)node_first->data, (OrderedSet*)node_second->data);
            generic_linked_list_insert(k_plus_one_cliques, k_plus_one_clique);
        }

        curr_k_plus_one_clique_node = curr_k_plus_one_clique_node->next;
    }

    return k_plus_one_cliques;
}

/**
 * @brief Computes all (k+1)-cliques from all k-cliques.
 *
 * This function first groups all k-cliques into groups of adjacent
 * cliques. Then, for each complete group of adjacent cliques, forms
 * (k+1)-cliques by taking the union of any two adjacent cliques in
 * the clique group. A clique group is complete if it contains at
 * least (k+1 choose k) k-cliques. The resulting (k+1)-cliques are
 * then returned.
 *
 * Trivial cases (i.e., k=0,1) are handled by
 * clique::compute_one_clique and clique::compute_two_clique.
 *
 * @param graph
 * @param k_cliques
 * @return GenericLinkedList*
 */
GenericLinkedList* expand_cliques(CSRGraph* graph, GenericLinkedList* k_cliques) {
    assert(graph != NULL);
    assert(k_cliques != NULL);

    int num_k_cliques = k_cliques->size;
    if (num_k_cliques == 0) {
        return generic_linked_list_new(
            ordered_set_generic_copy,
            ordered_set_generic_delete,
            ordered_set_generic_is_equal,
            ordered_set_generic_print);
    }

    OrderedSet* clique_sample = (OrderedSet*)k_cliques->head->data;
    int k = clique_sample->size;

    if (k == 0) {
        return compute_one_clique(graph);
    }

    if (k == 1) {
        return compute_two_clique(graph);
    }

    // Group all the k-cliques
    GenericLinkedList* grouped_k_cliques = group_k_cliques(k_cliques);

    // For each k-clique group, form a (k+1)-clique if the group
    // forms a valid (k+1)-clique.
    GenericLinkedList* k_plus_one_cliques = reduce_grouped_k_cliques(grouped_k_cliques, k);

    // Cleanup
    generic_linked_list_delete(&grouped_k_cliques);

    return k_plus_one_cliques;
}