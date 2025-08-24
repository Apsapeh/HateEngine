#pragma once

#include "../node.h"

// [[API Generator]]
typedef struct {
    Node node;
} Window;

void window_init(Window* this, enum ObjectType object_type, const char* name);

void window_free(Window* this);

// [[API Generator]]
/**
 * @param name Node name
 * @param title Window title
 * @param w Width
 * @param h Height
 *
 */
Window* window_new(const char* name, const char* title, int w, int h);


// [[API Generator]]
void window_set_title(Window* this, const char* title);

// [[API Generator]]
const char* window_get_title(const Window* this);
