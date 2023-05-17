#ifndef THREE_FOUR_CLIQUES_H_INCLUDED
#define THREE_FOUR_CLIQUES_H_INCLUDED

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../collections/graph.h"
#include "../utilities/array_util.h"
#include "../utilities/math.h"
#include "../utilities/stopwatch.h"

typedef struct ThreeFourCliques {
    bool is_storing_cliques;
    int num_three_cliques;
    int num_four_cliques;
    clique* three_cliques;
    clique* four_cliques;
} ThreeFourCliques;

ThreeFourCliques* three_four_cliques_new(bool is_storing_cliques);
void three_four_cliques_delete(ThreeFourCliques** ptr_three_four_cliques);
void three_four_cliques_record(void* ptr_three_four_cliques, vertex u, vertex v, vertex w, vertex x);
void three_four_cliques_print(ThreeFourCliques* collector, bool print_cliques);

#endif