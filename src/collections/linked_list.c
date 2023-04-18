#include "linked_list.h"

// Helper functions

// LinkedList functions

/**
 * @brief Creates a new linked list with a NULL head and size 0.
 *
 * @return LinkedList* The new linked list.
 */
LinkedList* linked_list_new() {
    LinkedList* list = malloc(sizeof(LinkedList));
    list->head = NULL;
    list->size = 0;

    return list;
}

/**
 * @brief Copies the param linked list and all of its nodes.
 *
 * The param linked list is copied by first creating a new linked
 * list, then deep copying all of the nodes in the param linked list
 * into the new linked list.
 *
 * @note This function must be in the form (void* (*)(void*)) to be
 * used for copying data of this type in generic collections.
 *
 * @param list The linked list to copy.
 * @return LinkedList* The copy of the param list.
 */
LinkedList* linked_list_copy(LinkedList* list) {
    if (list == NULL) {
        return NULL;
    }

    LinkedList* copy = linked_list_new();

    Node* curr_node = list->head;
    while (curr_node != NULL) {
        linked_list_insert(copy, curr_node->data, curr_node->meta);

        Node* curr_copy = linked_list_get(copy, curr_node->data);
        curr_copy->children = linked_list_copy(curr_node->children);

        curr_node = curr_node->next;
    }

    return copy;
}

/**
 * @brief Deletes the linked list and all of its nodes.
 *
 * The linked list is deleted by first deleting all of its nodes,
 * then freeing the linked list itself.
 *
 * @note This function must be in the form (void (*)(void**)) to be
 * used for copying data of this type in generic collections.
 *
 * @param list The linked list to delete.
 */
void linked_list_delete(LinkedList** list) {
    if (list == NULL || *list == NULL) {
        return;
    }

    Node* node = (*list)->head;
    while (node != NULL) {
        Node* next = node->next;
        node_delete((void**)&node);
        node = next;
    }

    free(*list);
    *list = NULL;
}

/**
 * @brief Inserts the param data into the param list.
 *
 * The linked list is first checks if either of the params are NULL.
 * If both params are NULL, true is returned. If only one param is
 * NULL, false is returned. If both params are not NULL, this function
 * checks if the two lists have the same size and if their elements
 * are the same and ordered. If both conditions are true, true is
 * returned, otherwise, false is returned.
 *
 * @note This function must be in the form (bool* (*)(void*, void*))
 * to be used for copying data of this type in generic collections.
 *
 * @param list_a The first list to compare.
 * @param list_b The second list to compare.
 * @return true if the two lists are NULL or if the two lists are not
 * NULL and contain the same elements in the same order, otherwise,
 * false is returned.
 */
bool linked_list_compare(LinkedList* list_a, LinkedList* list_b) {
    if (list_a == NULL && list_b == NULL) {
        return true;
    }

    if (list_a == NULL || list_b == NULL) {
        return false;
    }

    if (list_a->size != list_b->size) {
        return false;
    }

    Node* node_a = list_a->head;
    Node* node_b = list_b->head;

    while (node_a != NULL && node_b != NULL) {
        if (node_a->data != node_b->data) {
            return false;
        }
        node_a = node_a->next;
        node_b = node_b->next;
    }

    if (node_a != NULL || node_b != NULL) {
        return false;
    }

    return true;
}

/**
 * @brief Prints the param list.
 *
 * The param list is printed to stdout by first printing the data of
 * each node in the list, which also prints out their respective
 * children.
 *
 * @note This function must be in the form (void (*)(void*)) to be
 * used for copying data of this type in generic collections.
 *
 * @param list The list to print.
 */
void linked_list_print(LinkedList* list) {
    assert(list != NULL);

    Node* node = list->head;
    while (node != NULL) {
        printf("%d", node->data);

        if (node->children != NULL) {
            printf(" (");
            linked_list_print(node->children);
            printf(")");
        }

        node = node->next;

        if (node != NULL) {
            printf(", ");
        }
    }
}

/**
 * @brief Inserts the param data into the param list.
 *
 * The linked list is first checked for the existence of the param
 * data. If the data exists, the meta data is updated with the param
 * meta and false is returned. If the data does not exist, the param
 * data is inserted into the linked list in ascending order and true
 * is returned. The size of the linked list is incremented by 1 if
 * the node is inserted.
 *
 * @param list The linked list to insert the param data into.
 * @param data The data to insert into the linked list.
 * @param meta The meta data to insert into the linked list.
 * @return true If the node was inserted, otherwise false.
 */
bool linked_list_insert(LinkedList* list, int data, int meta) {
    assert(list != NULL);

    Node* prev_node = NULL;
    Node* curr_node = list->head;

    while (curr_node != NULL) {
        if (curr_node->data > data) {
            break;
        }

        if (curr_node->data == data) {
            curr_node->meta = meta;
            return false;
        }

        prev_node = curr_node;
        curr_node = curr_node->next;
    }

    Node* to_insert = node_new(data, meta);

    if (prev_node == NULL) {
        to_insert->next = list->head;
        list->head = to_insert;
    } else {
        to_insert->next = curr_node;
        prev_node->next = to_insert;
    }

    list->size++;
    return true;
}

/**
 * @brief Remove the node with param data from the linked list.
 *
 * The element is found by iteratively checking each element in the
 * linked list for equality with param data. If the element is found,
 * the node is removed and true is returned, otherwise false is
 * returned. The size of the linked list is decremented by 1 if the
 * node is removed.
 *
 * @param list The linked list to remove the node from.
 * @param data The data of the node to remove.
 * @return true If the node was removed, otherwise false.
 */
bool linked_list_remove(LinkedList* list, int data) {
    assert(list != NULL);

    Node* prev_node = NULL;
    Node* curr_node = list->head;

    while (curr_node != NULL) {
        if (curr_node->data == data) {
            if (prev_node == NULL) {
                list->head = curr_node->next;
            } else {
                prev_node->next = curr_node->next;
            }

            node_delete((void**)&curr_node);
            list->size--;
            return true;
        }

        prev_node = curr_node;
        curr_node = curr_node->next;
    }

    return false;
}

/**
 * @brief Check if the linked list contains a node with param data.
 *
 * The element is found by iteratively checking each element in the
 * linked list for equality with param data. If the element is found,
 * true is returned, otherwise false.
 *
 * @note This linkedlist does not allow duplicates. Only the data of
 * the child is used to determine existence.
 *
 * @param list The linked list to check.
 * @param data The data of the node to check for.
 * @return true If the linked list contains a node with param data,
 * otherwise false.
 */
bool linked_list_contains(LinkedList* list, int data) {
    assert(list != NULL);

    Node* node = list->head;
    while (node != NULL) {
        if (node->data == data) {
            return true;
        }

        node = node->next;
    }

    return false;
}

/**
 * @brief Get the node with param data from the linked list.
 *
 * The element is found by iteratively checking each element in the
 * linked list for equality with param data. If the node exists, the
 * node is returned, otherwise NULL is returned.
 *
 * @note This linkedlist does not allow duplicates. Only the data of
 * the child is used to determine existence.
 *
 * @param list The linked list to get the node from.
 * @param data The data of the node to get.
 * @return Node* The node with param data or NULL if the node does not
 * exist.
 */
Node* linked_list_get(LinkedList* list, int data) {
    assert(list != NULL);

    Node* node = list->head;
    while (node != NULL) {
        if (node->data == data) {
            return node;
        }

        node = node->next;
    }

    return NULL;
}

/**
 * @brief Check if the linked list is empty.
 *
 * Empty is defined as the head of the linked list being NULL and the
 * size of the linked list being 0.
 *
 * @param list The list to check.
 * @return true If the number of elements in the linkedlist is 0,
 * otherwise false.
 */
bool linked_list_is_empty(LinkedList* list) {
    assert(list != NULL);

    return list->head == NULL && list->size == 0;
}

/**
 * @brief Insert a child into the children of the parent node with
 * param parent_data.
 *
 * The parent is fetched from the linked list using param parent_data.
 * The child is initialized with param child_data and param
 * child_meta.
 *
 * @note This linkedlist inserts elements in ascending order and does
 * not allow duplicates. If a child with param child_data already
 * exists, the existing childs meta data will be updated with param
 * child_meta.
 *
 * @param list The linked list where the new child will be inserted.
 * @param parent_data The data of the parent node.
 * @param child_data The data of the child node.
 * @param child_meta The meta data of the child node.
 * @return true If the child was inserted, otherwise false.
 */
bool linked_list_insert_child(LinkedList* list, int parent_data, int child_data, int child_meta) {
    assert(list != NULL);

    Node* parent = linked_list_get(list, parent_data);
    if (parent == NULL) {
        return false;
    }

    if (parent->children == NULL) {
        parent->children = linked_list_new();
    }

    return linked_list_insert(parent->children, child_data, child_meta);
}

/**
 * @brief Remove a child node from a parent node.
 *
 * The parent is fetched from the linked list using param parent_data.
 * If the parent exists, the child is removed from the parent's
 * children if the child exists.
 *
 * @param list The linked list to search.
 * @param parent_data The data of the parent node.
 * @param child_data The data of the child node.
 * @return true If the child node was removed, otherwise false.
 */
bool linked_list_remove_child(LinkedList* list, int parent_data, int child_data) {
    assert(list != NULL);

    Node* parent = linked_list_get(list, parent_data);
    if (parent == NULL) {
        return false;
    }

    return linked_list_remove(parent->children, child_data);
}

/**
 * @brief Check if a linked list contains a child node of a parent
 * node.
 *
 * The parent is fetched from the linked list using param parent_data.
 * If the parent exists, the child is checked for existence in the
 * parent's children. If the child exists, true is returned, otherwise
 * false.
 *
 * @note This linkedlist does not allow duplicates. Only the data of
 * the child is used to determine existence.
 *
 * @param list The linked list to search.
 * @param parent_data The data of the parent node.
 * @param child_data The data of the child node.
 * @return true If the a child node with data child_data exists for a
 * specified parent with data parent_data.
 */
bool linked_list_contains_child(LinkedList* list, int parent_data, int child_data) {
    assert(list != NULL);

    Node* parent = linked_list_get(list, parent_data);
    if (parent == NULL) {
        return false;
    }

    return linked_list_contains(parent->children, child_data);
}

/**
 * @brief Get the child node of a parent node in a linked list.
 *
 * The parent is fetched from the linked list using param parent_data.
 * If the parent exists, the child is fetched from the parent's
 * children if the child exists. If the child exists, the child is
 * returned, otherwise NULL is returned.
 *
 * @note This linkedlist does not allow duplicates. Only the data of
 * the child is used to determine existence.
 *
 * @param list The linked list to search.
 * @param parent_data The data of the parent node.
 * @param child_data The data of the child node.
 * @return Node* The child node if it exists, otherwise NULL.
 */
Node* linked_list_get_child(LinkedList* list, int parent_data, int child_data) {
    assert(list != NULL);

    Node* parent = linked_list_get(list, parent_data);
    if (parent == NULL) {
        return NULL;
    }

    return linked_list_get(parent->children, child_data);
}