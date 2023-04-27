#ifndef CLIQUE_H_INCLUDED
#define CLIQUE_H_INCLUDED

#include "../collections/csr_graph.h"
#include "../collections/generic_linked_list.h"
#include "../collections/ordered_set.h"

GenericLinkedList* compute_one_clique(CSRGraph* graph);
GenericLinkedList* compute_two_clique(CSRGraph* graph);

#endif