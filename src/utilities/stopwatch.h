#ifndef STOPWATCH_H_INCLUDED
#define STOPWATCH_H_INCLUDED

#include <assert.h>
#include <stdlib.h>
#include <time.h>

typedef struct Stopwatch {
    clock_t start;
} Stopwatch;

Stopwatch* stopwatch_new();
void stopwatch_delete(Stopwatch** ptr_stopwatch);
double stopwatch_lap(Stopwatch* stopwatch);

#endif
