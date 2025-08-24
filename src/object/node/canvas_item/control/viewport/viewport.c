#include "viewport.h"
#include "platform/memory.h"

void viewport_init(Viewport* this, enum ObjectType object_type, const char* name) {
    // control_init(&this->control, object_type, <your_args>);
    control_init(&this->control, object_type, name);
    // Add your custom initialization here
}

void viewport_free(Viewport* this) {
    control_free(&this->control);
    // Add your custom cleanup here
}

Viewport* viewport_new(const char* name) {
    Viewport* viewport = tmalloc(sizeof(Viewport));
    viewport_init(viewport, ObjectTypeViewport, name);
    return viewport;
}
