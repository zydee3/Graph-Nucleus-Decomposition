#include "math.h"

int min(int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int min_3(int a, int b, int c) {
    if (a < b && a < c) {
        return a;
    } else if (b < a && b < c) {
        return b;
    } else {
        return c;
    }
}

int max_3(int a, int b, int c) {
    if (a > b && a > c) {
        return a;
    } else if (b > a && b > c) {
        return b;
    } else {
        return c;
    }
}

bool cmp_min(int a, int b) {
    return a < b;
}

bool cmp_max(int a, int b) {
    return a > b;
}

int cmp_qsort_min(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

int cmp_qsort_max(const void* a, const void* b) {
    return *(int*)b - *(int*)a;
}
