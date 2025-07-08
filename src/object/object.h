#pragma once

#include "types/uid.h"

enum ObjectType {
    TypeObject,
    TypeNode,
    TypeNode3D,
};

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
