#include "node.h"

/**
 * @brief Creates a new node with the param data and meta.
 *
 * The param data and meta are copied into the new node. The new nodes
 * children and next are initialized to NULL. The new node is
 * returned.
 *
 * @param data The data to copy into the new node.
 * @param meta The meta data to copy into the new node.
 * @return Node* The new node.
 */
Node* node_new(int data, int meta) {
    Node* node = malloc(sizeof(Node));
    node->data = data;
    node->meta = meta;
    node->next = NULL;
    node->children = NULL;
    return node;
}

/**
 * @brief Copies the param node and all of its children.
 *
 * The param node is copied by first copying all of its children,
 * then copying the param node.
 *
 * @note This function must be in the form (void* (*)(void*)) to be
 * used for copying data of this type in generic collections.
 *
 * @param ref_node
 * @return Node*
 */
Node* node_copy(void* ref_node) {
    assert(ref_node != NULL);

    Node* node = (Node*)ref_node;
    Node* copy = malloc(sizeof(Node));

    copy->data = node->data;
    copy->meta = node->meta;
    copy->next = NULL;
    copy->children = linked_list_copy(node->children);

    return copy;
}

/**
 * @brief Deletes the param node and all of its children.
 *
 * The param node is deleted by first deleting all of its children,
 * then freeing the param node.
 *
 * @note This function must be in the form (void (*)(void**)) to be
 * used for copying data of this type in generic collections.
 *
 * @param ptr_node The node to delete.
 */
void node_delete(void** ptr_node) {
    assert(ptr_node != NULL && *ptr_node != NULL);

    Node* node = (Node*)*ptr_node;
    linked_list_delete(&node->children);

    free(*ptr_node);
    *ptr_node = NULL;
}

/**
 * @brief Compares the data and meta of the param nodes.
 *
 * Each param node is compared to the other param node by checking if
 * their data, meta, and children are equal. If all three are equal,
 * true is returned, otherwise, false is returned.
 *
 * @note This function must be in the form (bool* (*)(void*, void*))
 * to be used for copying data of this type in generic collections.
 *
 * @param node_a The first node to compare to the second node.
 * @param node_b The second node to compare to the first node.
 * @return true If the data, meta, and children are equal.
 * @return false If the data, meta, or children are not equal.
 */
bool node_compare(void* node_a, void* node_b) {
    assert(node_a != NULL && node_b != NULL);

    Node* a = (Node*)node_a;
    Node* b = (Node*)node_b;
    return a->data == b->data && a->meta == b->meta && linked_list_compare(a->children, b->children);
}

/**
 * @brief Prints the data and meta of the param node to stdout.
 *
 * @note This function must be in the form (void (*)(void*)) to be
 * used for copying data of this type in generic collections.
 *
 * @param ref_node The node to print.
 */
void node_print(void* ref_node) {
    assert(ref_node != NULL);

    Node* node = (Node*)ref_node;
    printf("(%d, %d)", node->data, node->meta);
}