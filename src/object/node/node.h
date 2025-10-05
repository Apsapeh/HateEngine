#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../object.h"
#include "types/vector.h"

/**
 * @brief
 *
 * @api
 */
typedef struct {
    Object object;
    char* name;
    vec_ptr children;
} Node;

void node_init(Node* this, enum ObjectType object_type, const char* name);

void node_free(Node* node);


/**
 * @api
 */
Node* node_new(const char* name);

/**
 * @api
 */
Node* from_node(Node* node);


/**
 * @api
 */
void node_set_name(Node* node, const char* name);

/**
 * @api
 */
const char* node_get_name(Node* node);

/**
 * @brief Add a child node to a parent node
 * @warning Take ownership of child. Do not free it!
 * @param node
 * @param child
 *
 * @api
 */
void node_add_child(Node* node, Node* child);

/**
 * @brief Remove and free a child node by pointer from a parent node
 *
 * @param node
 * @param child
 * @return true if child was removed
 * @return false if child was not found
 *
 * @api
 */
bool node_remove_child(Node* node, Node* child);

/**
 * @brief Remove and free a child node by name from a parent node
 *
 * @param node
 * @param name
 * @return true if child was removed
 * @return false if child was not found
 *
 * @api
 */
bool node_remove_child_by_name(Node* node, const char* name);

/**
 * @brief Remove and free a child node by uid from a parent node
 *
 * @param node
 * @param uid
 * @return true if child was removed
 * @return false if child was not found
 *
 * @api
 */
bool node_remove_child_by_uid(Node* node, UID uid);

/**
 * @api
 */
bool node_remove_all_children(Node* node);

/**
 * @brief Get a child node by name
 *
 * @param node
 * @param name
 * @return Pointer to found child, NULL if not found
 *
 * @api
 */
Node* node_get_child_by_name(Node* node, const char* name);

/**
 * @api
 */
Node* node_get_child_by_uid(Node* node, UID uid);
