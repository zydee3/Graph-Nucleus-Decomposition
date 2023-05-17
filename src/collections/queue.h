#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../utilities/array_util.h"

typedef struct Queue {
    int capacity;
    int size;

    int* elements;
    int num_resize_amount;
} Queue;

// Create and Delete Functions
Queue* queue_new(int num_resize_amount);
void queue_delete(Queue** ptr_queue);

// Manipulation Functions
void queue_enqueue(Queue* queue, int element);
int queue_dequeue(Queue* queue);
int queue_peek(Queue* queue);

// Membership functions
bool queue_contains(Queue* queue, int element);

// Utility Functions
bool queue_is_empty(Queue* queue);
void queue_print(Queue* queue, bool should_print_newline);

#endif