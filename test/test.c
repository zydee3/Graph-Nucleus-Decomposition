#include <stdio.h>

#include "test_array_util.h"
#include "test_clique.h"
#include "test_core.h"
#include "test_csr.h"
#include "test_csr_graph.h"
#include "test_generic_linked_list.h"
#include "test_graph.h"
#include "test_linked_list.h"
#include "test_ordered_set.h"
#include "test_queue.h"

int main() {
    // Index of the first test to run. Allows us to skip previous
    // tests so we're not running old tests every time despite them
    // not changing.
    int idx_begin_tests = 0;

    void (*test_functions[10])() = {
        test_linked_list,
        test_generic_linked_list,
        test_graph,
        test_array_util,
        test_ordered_set,
        test_queue,
        test_csr,
        test_csr_graph,
        test_core,
        test_clique,
    };

    int num_tests = sizeof(test_functions) / sizeof(test_functions[0]);

    for (int i = idx_begin_tests; i < num_tests; i++) {
        printf("\nRunning test (%d/%d)...\n", i + 1, num_tests);
        test_functions[i]();
    }

    printf("\n");

    return EXIT_SUCCESS;
}