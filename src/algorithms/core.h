#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED

#include "../collections/graph.h"
#include "../collections/ordered_set.h"
#include "../collections/queue.h"
#include "../utilities/array_util.h"

bool* get_vertices_not_in_k_core(Graph* graph, int k);

#endif