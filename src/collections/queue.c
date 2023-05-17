#include "queue.h"

/**
 * @brief Create a new Queue object.
 *
 * @param num_resize_amount The resize amount is the value by which
 * the capacity of the queue is increased or decreased when the queue
 * is full or empty, respectively.
 * @return Queue* A pointer to the new Queue object.
 */
Queue* queue_new(int num_resize_amount) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->elements = (int*)malloc(sizeof(int) * num_resize_amount);
    queue->capacity = num_resize_amount;
    queue->size = 0;
    queue->num_resize_amount = num_resize_amount;

    return queue;
}

/**
 * @brief Delete a Queue object.
 *
 * The Queue object is deleted by freeing the memory allocated to the
 * elements array and the Queue object itself. The pointer to the
 * Queue object is set to NULL.
 *
 * @param ptr_queue A pointer to a pointer to a Queue object.
 */
void queue_delete(Queue** ptr_queue) {
    assert(ptr_queue != NULL);
    assert(*ptr_queue != NULL);

    free((*ptr_queue)->elements);
    free(*ptr_queue);
    *ptr_queue = NULL;
}

// Manipulation Functions

/**
 * @brief Add an element to the end of the queue.
 *
 * If the queue is full, the capacity of the queue is increased by
 * the resize amount.
 *
 * @param queue The queue to which the element is added.
 * @param element The element to add to the end of the queue.
 */
void queue_enqueue(Queue* queue, int element) {
    assert(queue != NULL);

    if (queue->size == queue->capacity) {
        int new_capacity = queue->capacity + queue->num_resize_amount;

        queue->elements = realloc(queue->elements, sizeof(int) * new_capacity);
        assert(queue->elements != NULL);

        queue->capacity = new_capacity;
    }

    queue->elements[queue->size] = element;
    queue->size++;
}

/**
 * @brief Remove an element from the front of the queue.
 *
 * If the difference between the size and the capacity of the queue
 * is greater than or equal to the resize amount, the capacity of the
 * queue is decreased by the resize amount.
 *
 * @param queue The queue to which the element is removed.
 * @param element The element to remove from the front of the queue.
 * @return int The element removed from the front of the queue.
 */
int queue_dequeue(Queue* queue) {
    assert(queue->size > 0);

    int element = queue->elements[0];
    memmove(queue->elements, &queue->elements[1], sizeof(int) * (queue->capacity - 1));
    queue->elements[queue->capacity - 1] = 0;

    queue->size--;

    if (queue->size < queue->capacity - queue->num_resize_amount) {
        int new_capacity = queue->capacity - queue->num_resize_amount;

        queue->elements = realloc(queue->elements, sizeof(int) * new_capacity);
        assert(queue->elements != NULL);

        queue->capacity = new_capacity;
    }

    return element;
}

/**
 * @brief Get the element at the front of the queue without removing
 * it from the queue.
 *
 * This function errors if the queue is empty.
 *
 * @param queue The queue from which the element is peeked.
 * @return int The first element in the queue.
 */
int queue_peek(Queue* queue) {
    assert(queue->size > 0);

    return queue->elements[0];
}

/**
 * @brief Checks to see if the list contains a specified element.
 *
 * The function uses the a linear search to determine membership.
 *
 * @param queue The queue to search.
 * @param element The element to search for.
 * @return bool True if the element exists, otherwise false.
 */
bool queue_contains(Queue* queue, int element) {
    return array_linear_search(queue->elements, queue->size, element) > -1;
}

// Utility Functions

/**
 * @brief Checks to see if the queue is empty.
 *
 * @param queue The queue to check.
 * @return bool True if the queue is empty, otherwise false.
 */
bool queue_is_empty(Queue* queue) {
    return queue->size == 0;
}

/**
 * @brief Prints the queue to stdout.
 *
 * @param queue The queue to print.
 * @param should_print_newline True if a newline should be printed
 * at the end of all print statements, false otherwise.
 */
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