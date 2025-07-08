#pragma once

#include "../node.h"
#include "math/mat4.h"

// [[API Generator]]
typedef struct {
    Node node;
    Mat4 transform;
} Node3D;

void node3d_init(Node3D* node3d, const char* name);

// [[API Generator]]
Node3D* node3d_new(void);

void node3d_free(Node3D* node3d);
