#include "clique_expansion.h"

GenericLinkedList* expand_cliques(GenericLinkedList* cliques) {
    GenericLinkedList* expanded_cliques = generic_linked_list_new(ordered_set_generic_copy, ordered_set_generic_delete, ordered_set_generic_compare, ordered_set_generic_print);

    return expanded_cliques;
}