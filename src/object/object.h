#pragma once

#include "types/uid.h"

// clang-format off
enum ObjectType {
    // Root
    ObjectTypeObject,
    // Object->Node
    ObjectTypeNode,
        // Object->Node->Node3D
        ObjectTypeNode3D,
        // Object->Node->Window
        ObjectTypeWindow,
};
// clang-format on

// [[API Generator]]
typedef struct {
    enum ObjectType type;
    UID uid;
} Object;

// [[API Generator]]
/**
 * @brief Free object by type
 *
 * @param object
 */
void auto_free(Object* object);

void object_init(Object* object, enum ObjectType type);

void object_free(Object* object);
