#include "node3d.h"
#include "math/mat4.h"
#include "object/node/node.h"
#include "platform/memory.h"

void node3d_init(Node3D* this, const char* name) {
    node_init(&this->node, name);
    this->transform = mat4_new_one();
}

Node3D* node3d_new(void) {
    Node3D* node3d = tmalloc(sizeof(Node3D));
    return node3d;
}

void node3d_free(Node3D* node3d) {
    node_free(&node3d->node);
}
