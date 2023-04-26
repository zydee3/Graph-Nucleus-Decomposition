#ifndef CLIQUE_CHIBA_NISHIZEKI_H_INCLUDED
#define CLIQUE_CHIBA_NISHIZEKI_INCLUDED

#include "../algorithms/core.h"
#include "../collections/csr_graph.h"
#include "../collections/generic_linked_list.h"
#include "../collections/ordered_set.h"
#include "../collections/queue.h"
#include "../utilities/array_util.h"

OrderedSet** find_k_cliques(CSRGraph* graph, int k);

#endif