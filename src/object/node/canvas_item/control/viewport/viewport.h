#pragma once

#include "../control.h"

/**
 * @brief
 *
 * @api
 */
typedef struct {
    Control control;
    // Add your custom fields here
} Viewport;

void viewport_init(Viewport* this, enum ObjectType object_type, const char* name);

void viewport_free(Viewport* this);

/**
 * @brief
 *
 * @api
 */
Viewport* viewport_new(const char* name);
