#ifndef NUCLEUS_DECOMPOSITION_H_INCLUDED
#define NUCLEUS_DECOMPOSITION_H_INCLUDED

#include <assert.h>

#include "../collections/csr_graph.h"
#include "../collections/generic_node.h"
#include "../collections/ordered_set.h"
#include "../utilities/array_util.h"
#include "clique_chiba_nishizeki.h"
#include "clique_expansion.h"

void run_nucleus_decomposition_3_4(CSRGraph* graph);

#endif