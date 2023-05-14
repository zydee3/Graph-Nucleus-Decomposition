#ifndef NUCLEUS_DECOMPOSITION_H_INCLUDED
#define NUCLEUS_DECOMPOSITION_H_INCLUDED

#include <assert.h>
#include <time.h>

#include "../collections/clique_set.h"
#include "../collections/generic_node.h"
#include "../collections/graph.h"
#include "../collections/ordered_set.h"
#include "../utilities/array_util.h"
#include "../utilities/stopwatch.h"
#include "clique.h"

typedef struct {
    OrderedSet** s_map;
    OrderedSet** r_map;
} MappedCliques;

void run_nucleus_decomposition_3_4(Graph* graph);

#endif