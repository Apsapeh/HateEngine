#pragma once

#include "../node.h"
#include "math/mat4.h"

/**
 * @brief
 *
 * @api
 */
typedef struct {
    Node node;
    Mat4 transform;
} Node3D;

void node3d_init(Node3D* this, enum ObjectType object_type, const char* name);

/**
 * @brief
 *
 * @api
 */
Node3D* node3d_new(const char* name);

void node3d_free(Node3D* node3d);
