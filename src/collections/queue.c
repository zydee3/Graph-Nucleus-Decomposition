#include "queue.h"

Queue* queue_new(int num_resize_amount) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->elements = (int*)malloc(sizeof(int) * num_resize_amount);
    queue->capacity = num_resize_amount;
    queue->size = 0;
    queue->num_resize_amount = num_resize_amount;

    return queue;
}

void queue_delete(Queue** ptr_queue) {
    assert(ptr_queue != NULL);
    assert(*ptr_queue != NULL);

    free((*ptr_queue)->elements);
    free(*ptr_queue);
    *ptr_queue = NULL;
}

// Manipulation Functions
void queue_enqueue(Queue* queue, int element) {
    assert(queue != NULL);

    if (queue->size == queue->capacity) {
        int new_capacity = queue->capacity + queue->num_resize_amount;
        array_resize(&queue->elements, queue->capacity, new_capacity);
        queue->capacity = new_capacity;
    }

    queue->elements[queue->size] = element;
    queue->size++;
}

int queue_dequeue(Queue* queue) {
    assert(queue->size > 0);

    int element = queue->elements[0];
    memmove(queue->elements, &queue->elements[1], sizeof(int) * (queue->capacity - 1));
    queue->elements[queue->capacity - 1] = 0;

    queue->size--;

    if (queue->size < queue->capacity - queue->num_resize_amount) {
        int new_capacity = queue->capacity - queue->num_resize_amount;
        array_resize(&queue->elements, queue->capacity, new_capacity);
        queue->capacity = new_capacity;
    }

    return element;
}

int queue_peek(Queue* queue) {
    assert(queue->size > 0);

    return queue->elements[0];
}

bool queue_contains(Queue* queue, int element) {
    return array_binary_search(queue->elements, queue->size, element) > -1;
}

// Utility Functions
int queue_is_empty(Queue* queue) {
    return queue->size == 0;
}

void queue_print(Queue* queue, bool should_print_newline) {
    assert(queue != NULL);

    printf("Queue: [");
    for (int i = 0; i < queue->size; i++) {
        printf("%d", queue->elements[i]);
        if (i != queue->size - 1) {
            printf(", ");
        }
    }
    printf("]");

    if (should_print_newline) {
        printf("\n");
    }
}