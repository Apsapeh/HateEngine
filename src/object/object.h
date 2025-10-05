#pragma once

#include "types/uid.h"
#include "object_types.h"

/**
 * @api
 */
typedef struct {
    enum ObjectType type;
    UID uid;
} Object;

/**
 * @brief Free object by type
 *
 * @param object
 * @api
 */
void auto_free(Object* object);

void object_init(Object* this, enum ObjectType type);

void object_free(Object* object);
