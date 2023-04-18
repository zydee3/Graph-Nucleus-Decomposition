#ifndef CLIQUE_H_INCLUDED
#define CLIQUE_H_INCLUDED

#include "../algorithms/core.h"
#include "../collections/csr_graph.h"
#include "../collections/ordered_set.h"
#include "../collections/queue.h"
#include "../utilities/array_util.h"

OrderedSet** compute_k_cliques(CSRGraph* graph, int k);

#endif