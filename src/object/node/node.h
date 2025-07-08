#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../object.h"
#include "types/vec_def.h"

// [[API Generator]]
typedef struct {
    Object object;
    const char* name;
    vec_ptr children;
} Node;

void node_init(Node* node, const char* name);

void node_free(Node* node);


// [[API Generator]]
Node* node_new(const char* name);

// [[API Generator]]
Node* from_node(Node* node);


// [[API Generator]]
void node_set_name(Node* node, const char* name);

// [[API Generator]]
const char* node_get_name(Node* node);

// [[API Generator]]
/**
 * @brief Add a child node to a parent node
 * @warning Take ownership of child. Do not free it!
 * @param node
 * @param child
 */
void node_add_child(Node* node, Node* child);

// [[API Generator]]
/**
 * @brief Remove and free a child node by pointer from a parent node
 *
 * @param node
 * @param child
 * @return true if child was removed
 * @return false if child was not found
 */
bool node_remove_child(Node* node, Node* child);

// [[API Generator]]
/**
 * @brief Remove and free a child node by name from a parent node
 *
 * @param node
 * @param name
 * @return true if child was removed
 * @return false if child was not found
 */
bool node_remove_child_by_name(Node* node, const char* name);

// [[API Generator]]
/**
 * @brief Remove and free a child node by uid from a parent node
 *
 * @param node
 * @param uid
 * @return true if child was removed
 * @return false if child was not found
 */
bool node_remove_child_by_uid(Node* node, UID uid);

// [[API Generator]]
bool node_remove_all_children(Node* node);

// [[API Generator]]
/**
 * @brief Get a child node by name
 *
 * @param node
 * @param name
 * @return Pointer to found child, NULL if not found
 */
Node* node_get_child_by_name(Node* node, const char* name);

// [[API Generator]]
Node* node_get_child_by_uid(Node* node, UID uid);
