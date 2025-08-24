#include "canvas_item.h"
#include "platform/memory.h"

void canvas_item_init(CanvasItem* this, enum ObjectType object_type, const char* name) {
    node_init(&this->node, object_type, name);
    // Add your custom initialization here
}

void canvas_item_free(CanvasItem* this) {
    node_free(&this->node);
    // Add your custom cleanup here
}

CanvasItem* canvas_item_new(const char* name) {
    CanvasItem* canvas_item = tmalloc(sizeof(CanvasItem));
    canvas_item_init(canvas_item, ObjectTypeCanvasItem, name);
    return canvas_item;
}
