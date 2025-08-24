#pragma once

#include "../node.h"


// [[API Generator]]
typedef struct {
    Node node;
    // Add your custom fields here
} CanvasItem;

void canvas_item_init(CanvasItem* this, enum ObjectType object_type, const char* name);

void canvas_item_free(CanvasItem* this);

// [[API Generator]]
CanvasItem* canvas_item_new(const char* name);
