#include "object.h"
#include "node/node.h"
#include "node/canvas_item/canvas_item.h"
#include "node/canvas_item/control/control.h"
#include "node/canvas_item/control/viewport/viewport.h"
#include "node/node3d/node3d.h"
#include "node/window/window.h"

#include "platform/memory.h"

typedef void (*free_func)(void* object);

#define CAST_FREE(func) (free_func) func

// clang-format off
static free_func free_table[] = {
        [ObjectTypeNode]       = CAST_FREE(node_free),
        [ObjectTypeCanvasItem] = CAST_FREE(canvas_item_free),
        [ObjectTypeControl]    = CAST_FREE(control_free),
        [ObjectTypeViewport]   = CAST_FREE(viewport_free),
        [ObjectTypeNode3D]     = CAST_FREE(node3d_free),
        [ObjectTypeWindow]     = CAST_FREE(window_free)
};
// clang-format on

void auto_free(Object* object) {
    free_table[object->type](object);
    tfree(object);
}
