#include "object.h"
#include "node/node.h"
#include "node/node3d/node3d.h"

#include "platform/memory.h"
#include "types/uid.h"

typedef void (*free_func)(void* object);

#define CAST_FREE(func) (free_func) func

static free_func free_table[] = {
        [TypeObject] = CAST_FREE(object_free),
        [TypeNode] = CAST_FREE(node_free),
        [TypeNode3D] = CAST_FREE(node3d_free),
};

void auto_free(Object* object) {
    free_table[object->type](object);
}

void object_init(Object* object, enum ObjectType type) {
    object->type = type;
    object->uid = uid_new();
}

void object_free(Object* object) {
    tfree(object);
}
