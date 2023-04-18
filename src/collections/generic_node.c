#include "generic_node.h"

GenericNode* generic_node_new(void* data) {
    assert(data != NULL);

    GenericNode* node = malloc(sizeof(GenericNode));
    node->data = data;
    node->next = NULL;
    return node;
}

GenericNode* generic_node_copy(void* ref_node, void* (*_copy_data)(void*), bool (*_compare_data)(void*, void*)) {
    assert(ref_node != NULL && _copy_data != NULL && _compare_data != NULL);

    GenericNode* node = (GenericNode*)ref_node;
    GenericNode* copy = malloc(sizeof(GenericNode));
    copy->data = _copy_data(node->data);
    copy->next = NULL;
    return copy;
}

void generic_node_delete(void** ptr_node, void (*_delete_data)(void**)) {
    assert(ptr_node != NULL && *ptr_node != NULL && _delete_data != NULL);

    GenericNode* node = (GenericNode*)*ptr_node;
    _delete_data(&node->data);

    free(*ptr_node);
    *ptr_node = NULL;
}

bool generic_node_compare(void* node_a, void* node_b, bool (*_compare_data)(void*, void*)) {
    assert(node_a != NULL && node_b != NULL && _compare_data != NULL);

    GenericNode* a = (GenericNode*)node_a;
    GenericNode* b = (GenericNode*)node_b;
    return _compare_data(a->data, b->data);
}

void generic_node_print(void* node, void (*_print_data)(void*)) {
    assert(node != NULL && _print_data != NULL);

    GenericNode* n = (GenericNode*)node;
    _print_data(n->data);
}