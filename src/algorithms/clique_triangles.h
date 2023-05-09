#ifndef CLIQUE_TRIANGLES_H_INCLUDED
#define CLIQUE_TRIANGLES_H_INCLUDED

#include <assert.h>
#include <stdbool.h>
#include <time.h>

#include "../algorithms/clique.h"
#include "../collections/generic_linked_list.h"
#include "../collections/graph.h"
#include "../collections/ordered_set.h"
#include "../utilities/array_util.h"
#include "../utilities/math.h"
#include "../utilities/stopwatch.h"

GenericLinkedList* compute_triangles(Graph* graph);

#endif
