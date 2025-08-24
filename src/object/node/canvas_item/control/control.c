#include "control.h"
#include "platform/memory.h"

void control_init(Control* this, enum ObjectType object_type, const char* name) {
    canvas_item_init(&this->canvas_item, object_type, name);
    // Add your custom initialization here
}

void control_free(Control* this) {
    canvas_item_free(&this->canvas_item);
    // Add your custom cleanup here
}

Control* control_new(const char* name) {
    Control* control = tmalloc(sizeof(Control));
    control_init(control, ObjectTypeControl, name);
    return control;
}
