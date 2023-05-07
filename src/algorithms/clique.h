#ifndef CLIQUE_H_INCLUDED
#define CLIQUE_H_INCLUDED

#include "../collections/generic_linked_list.h"
#include "../collections/graph.h"
#include "../collections/ordered_set.h"

GenericLinkedList* compute_one_clique(Graph* graph);
GenericLinkedList* compute_two_clique(Graph* graph);

#endif