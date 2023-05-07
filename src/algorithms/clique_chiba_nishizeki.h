#ifndef CLIQUE_CHIBA_NISHIZEKI_H_INCLUDED
#define CLIQUE_CHIBA_NISHIZEKI_INCLUDED

#include "../algorithms/core.h"
#include "../collections/generic_linked_list.h"
#include "../collections/graph.h"
#include "../collections/ordered_set.h"
#include "../collections/queue.h"
#include "../utilities/array_util.h"

GenericLinkedList* find_k_cliques(Graph* graph, int k);

#endif