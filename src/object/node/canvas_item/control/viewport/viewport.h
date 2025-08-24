#pragma once

#include "../control.h"

// [[API Generator]]
typedef struct {
    Control control;
    // Add your custom fields here
} Viewport;

void viewport_init(Viewport* this, enum ObjectType object_type, const char* name);

void viewport_free(Viewport* this);

// [[API Generator]]
Viewport* viewport_new(const char* name);
