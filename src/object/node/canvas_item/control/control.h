#pragma once

#include "../canvas_item.h"

// [[API Generator]]
typedef struct {
    CanvasItem canvas_item;
    // Add your custom fields here
} Control;

void control_init(Control* this, enum ObjectType object_type, const char* name);

void control_free(Control* this);

// [[API Generator]]
Control* control_new(const char* name);
