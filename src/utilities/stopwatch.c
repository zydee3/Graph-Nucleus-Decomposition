#include "stopwatch.h"

Stopwatch* stopwatch_new() {
    Stopwatch* stopwatch = malloc(sizeof(Stopwatch));
    stopwatch->start = clock();
    return stopwatch;
}

void stopwatch_delete(Stopwatch** ptr_stopwatch) {
    assert(ptr_stopwatch != NULL);
    assert(*ptr_stopwatch != NULL);

    free(*ptr_stopwatch);
    *ptr_stopwatch = NULL;
}

double stopwatch_lap(Stopwatch* stopwatch) {
    assert(stopwatch != NULL);

    clock_t end = clock();
    double elapsed = (double)(end - stopwatch->start) / CLOCKS_PER_SEC;
    stopwatch->start = end;
    return elapsed;
}