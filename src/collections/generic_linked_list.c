#include "generic_linked_list.h"

void* generic_generic_linked_list_copy(void* list) {
    return generic_linked_list_copy((GenericLinkedList*)list);
}

void generic_generic_linked_list_delete(void** list) {
    generic_linked_list_delete((GenericLinkedList**)list);
}

bool generic_generic_linked_list_is_equal(void* list_a, void* list_b) {
    return generic_linked_list_is_equal((GenericLinkedList*)list_a, (GenericLinkedList*)list_b);
}

void generic_generic_linked_list_print(void* list) {
    generic_linked_list_print((GenericLinkedList*)list);
}

GenericLinkedList* generic_linked_list_new(void* (*_copy_data)(void*), void (*_delete_data)(void**), bool (*_compare_data)(void*, void*), void (*_print_data)(void*)) {
    assert(_copy_data != NULL && _delete_data != NULL && _compare_data != NULL && _print_data != NULL);

    GenericLinkedList* list = malloc(sizeof(GenericLinkedList));
    list->head = NULL;
    list->size = 0;

    list->_copy_data = _copy_data;
    list->_delete_data = _delete_data;
    list->_compare_data = _compare_data;
    list->_print_data = _print_data;

    return list;
}

GenericLinkedList* generic_linked_list_copy(GenericLinkedList* list) {
    assert(list != NULL);

    GenericLinkedList* copy = generic_linked_list_new(list->_copy_data, list->_delete_data, list->_compare_data, list->_print_data);

    GenericNode* node = list->head;
    while (node != NULL) {
        generic_linked_list_insert(copy, list->_copy_data(node->data));
        node = node->next;
    }

    return copy;
}

void generic_linked_list_delete(GenericLinkedList** list) {
    assert(list != NULL && *list != NULL);

    GenericNode* node = (*list)->head;
    while (node != NULL) {
        GenericNode* next = node->next;
        generic_node_delete((void**)&node, (*list)->_delete_data);
        node = next;
    }

    free(*list);
    *list = NULL;
}

bool generic_linked_list_is_equal(GenericLinkedList* list_a, GenericLinkedList* list_b) {
    assert(list_a != NULL && list_b != NULL);

    if (list_a->size != list_b->size) {
        return false;
    }

    GenericNode* node_a = list_a->head;
    GenericNode* node_b = list_b->head;

    while (node_a != NULL && node_b != NULL) {
        if (!list_a->_compare_data(node_a->data, node_b->data)) {
            return false;
        }

        node_a = node_a->next;
        node_b = node_b->next;
    }

    return true;
}

void generic_linked_list_print(GenericLinkedList* list) {
    assert(list != NULL);

    GenericNode* node = list->head;
    while (node != NULL) {
        list->_print_data(node->data);
        node = node->next;

        if (node != NULL) {
            printf(",");
        }
    }
}

bool generic_linked_list_insert(GenericLinkedList* list, void* data) {
    assert(list != NULL && data != NULL);

    GenericNode* node = list->head;

    if (node == NULL) {
        list->head = generic_node_new(data);
        list->size++;
        return true;
    }

    while (node->next != NULL) {
        if (list->_compare_data(node->data, data)) {
            return false;
        }

        node = node->next;
    }

    node->next = generic_node_new(data);
    list->size++;
    return true;
}

bool generic_linked_list_prepend(GenericLinkedList* list, void* data) {
    assert(list != NULL);
    assert(data != NULL);

    GenericNode* head = list->head;
    GenericNode* node = generic_node_new(data);

    node->next = head;
    list->head = node;

    list->size++;
    return true;
}

bool generic_linked_list_remove(GenericLinkedList* list, void* data) {
    assert(list != NULL && data != NULL);

    GenericNode* prev = NULL;
    GenericNode* curr = list->head;

    while (curr != NULL) {
        if (list->_compare_data(curr->data, data)) {
            if (prev == NULL) {
                list->head = curr->next;
            } else {
                prev->next = curr->next;
            }

            generic_node_delete((void**)&curr, list->_delete_data);
            list->size--;
            return true;
        }

        prev = curr;
        curr = curr->next;
    }

    return false;
}

bool generic_linked_list_contains(GenericLinkedList* list, void* data) {
    assert(list != NULL && data != NULL);

    GenericNode* node = list->head;
    while (node != NULL) {
        if (list->_compare_data(node->data, data)) {
            return true;
        }

        node = node->next;
    }

    return false;
}

bool generic_linked_list_is_empty(GenericLinkedList* list) {
    assert(list != NULL);
    return list->head == NULL;
}