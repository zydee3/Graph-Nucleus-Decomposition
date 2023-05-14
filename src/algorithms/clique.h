#ifndef CLIQUE_H_INCLUDED
#define CLIQUE_H_INCLUDED

#include "../collections/clique_set.h"
#include "../collections/generic_linked_list.h"
#include "../collections/graph.h"
#include "../collections/ordered_set.h"
#include "../utilities/array_util.h"
#include "../utilities/math.h"
#include "core.h"

CliqueSet* find_k_cliques(Graph* graph, int k);
CliqueSet* expand_cliques(Graph* graph, CliqueSet* k_cliques);

#endif