#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

#include <stdbool.h>

int min(int a, int b);
int max(int a, int b);

int min_3(int a, int b, int c);
int max_3(int a, int b, int c);

bool cmp_min(int a, int b);
bool cmp_max(int a, int b);

int cmp_qsort_min(const void* a, const void* b);
int cmp_qsort_max(const void* a, const void* b);

#endif