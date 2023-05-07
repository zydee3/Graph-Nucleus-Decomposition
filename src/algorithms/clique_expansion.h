#ifndef CLIQUE_EXPANSION_H_INCLUDED
#define CLIQUE_EXPANSION_H_INCLUDED

#include <assert.h>
#include <stdbool.h>

#include "../algorithms/clique.h"
#include "../collections/csr_graph.h"
#include "../collections/generic_linked_list.h"
#include "../collections/ordered_set.h"

GenericLinkedList* group_k_cliques(GenericLinkedList* k_cliques);
GenericLinkedList* reduce_grouped_k_cliques(GenericLinkedList* grouped_k_cliques, int k);
GenericLinkedList* expand_cliques(CSRGraph* graph, GenericLinkedList* cliques);

#endif