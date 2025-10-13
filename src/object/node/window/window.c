#include "window.h"
#include "platform/memory.h"

void window_init(Window* this, enum ObjectType object_type, const char* name) {
    node_init(&this->node, object_type, name);
}

void window_free(Window* this) {
    node_free(&this->node);
}

Window* window_new(const char* name, const char* title, i32 w, i32 h) {
    Window* window = tmalloc(sizeof(Window));
    window_init(window, ObjectTypeWindow, name);
    return window;
}


void window_set_title(Window* this, const char* title) {
}

const char* window_get_title(const Window* this) {
    return "";
}
