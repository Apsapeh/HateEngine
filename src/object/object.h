#pragma once

#include "types/uid.h"
#include "object_types.h"

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

void object_init(Object* this, enum ObjectType type);

void object_free(Object* object);
