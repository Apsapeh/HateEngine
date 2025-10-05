#pragma once

#include "../node.h"


/**
 * @brief
 *
 * @api
 */
typedef struct {
    Node node;
    // Add your custom fields here
} CanvasItem;

void canvas_item_init(CanvasItem* this, enum ObjectType object_type, const char* name);

void canvas_item_free(CanvasItem* this);

/**
 * @api
 */
CanvasItem* canvas_item_new(const char* name);
