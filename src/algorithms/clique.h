#ifndef CLIQUE_H_INCLUDED
#define CLIQUE_H_INCLUDED

#include "../collections/clique_set.h"
#include "../collections/generic_linked_list.h"
#include "../collections/graph.h"
#include "../collections/ordered_set.h"
#include "../utilities/array_util.h"
#include "../utilities/math.h"
#include "../utilities/stopwatch.h"
#include "core.h"

CliqueSet* enumerate_k_cliques(Graph* graph, int k);
void enumerate_three_cliques(Graph* graph, void* collection, void (*record)(void*, vertex, vertex, vertex));
void enumerate_four_cliques(Graph* graph, void* collection, void (*record)(void*, vertex, vertex, vertex, vertex));

#endif