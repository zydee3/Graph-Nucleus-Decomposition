#ifndef CLIQUE_SET_H_INCLUDED
#define CLIQUE_SET_H_INCLUDED

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../utilities/array_util.h"
#include "graph.h"

typedef struct CliqueSet {
    int k;
    int size;
    int capacity;
    int resize_value;
    clique* cliques;
} CliqueSet;

CliqueSet* clique_set_new(int k, int resize_value);
CliqueSet* clique_set_copy(CliqueSet* clique_set);
void clique_set_delete(CliqueSet** ptr_clique_set);

bool clique_set_insert(CliqueSet* clique_set, clique clique);
void clique_set_print(CliqueSet* clique_set, bool should_print_newline);
void clique_set_print_n(CliqueSet* clique_set);

bool clique_set_is_equal(CliqueSet* clique_set_a, CliqueSet* clique_set_b);
#endif