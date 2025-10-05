#pragma once

#include "../node.h"

/**
 * @brief
 *
 * @api
 */
typedef struct {
    Node node;
} Window;

void window_init(Window* this, enum ObjectType object_type, const char* name);

void window_free(Window* this);

/**
 * @param name Node name
 * @param title Window title
 * @param w Width
 * @param h Height
 *
 * @api
 */
Window* window_new(const char* name, const char* title, int w, int h);


/**
 * @brief
 *
 * @api
 */
void window_set_title(Window* this, const char* title);

/**
 * @brief
 *
 * @api
 */
const char* window_get_title(const Window* this);
