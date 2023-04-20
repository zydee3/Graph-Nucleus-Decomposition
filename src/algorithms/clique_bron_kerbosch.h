#ifndef CLIQUE_BRON_KERBOSCH_H_INCLUDED
#define CLIQUE_BRON_KERBOSCH_H_INCLUDED

#include "../algorithms/core.h"
#include "../collections/csr_graph.h"
#include "../collections/ordered_set.h"
#include "../collections/queue.h"
#include "../utilities/array_util.h"

OrderedSet** find_k_cliques_bron_kerbosch(CSRGraph* graph, int min_k);

#endif