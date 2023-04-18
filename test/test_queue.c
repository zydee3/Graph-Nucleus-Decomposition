#include "test_queue.h"

void test_queue_enqueue() {
    int* answer = array_generate_sequence_shuffled(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);

    Queue* queue = queue_new(5);
    for (int i = 0; i < NUM_ELEMS; i++) {
        queue_enqueue(queue, answer[i]);
    }

    bool is_passing = memcmp(queue->elements, answer, sizeof(int) * NUM_ELEMS) == 0;
    is_passing = is_passing && (queue->size == NUM_ELEMS);

    queue_delete(&queue);
    free(answer);

    print_test_result(__FILE__, __func__, is_passing);
}

void test_queue_dequeue() {
    bool is_passing = true;
    int* answer = array_generate_sequence_shuffled(RANGE_ELEMS_S1_LOW, INC_VALUE, NUM_ELEMS);

    Queue* queue = queue_new(5);
    for (int i = 0; i < NUM_ELEMS; i++) {
        queue_enqueue(queue, answer[i]);
    }

    for (int i = 0; i < NUM_ELEMS; i++) {
        is_passing = is_passing && (queue_dequeue(queue) == answer[i]);
    }

    is_passing = is_passing && (queue->size == 0);

    queue_delete(&queue);
    free(answer);

    print_test_result(__FILE__, __func__, is_passing);
}

void test_queue() {
    test_queue_enqueue();
    test_queue_dequeue();
}