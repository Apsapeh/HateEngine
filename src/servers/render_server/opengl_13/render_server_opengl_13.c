#include "render_server_opengl_13.h"
#include "servers/render_server/render_server.h"
#include "error.h"
#include "log.h"
#include "types/types.h"

/* ====> Errors <==== */

/* ================== */

static boolean _init(void) {
    return true;
}

static boolean _quit(void) {
    return true;
}


#define REGISTER(fn) render_server_backend_set_function(backend, #fn, (void (*)(void)) fn)

void render_server_opengl_13_backend_register(void) {
    RenderServerBackend* backend = render_server_backend_new();

    REGISTER(_init);
    REGISTER(_quit);

    render_server_register_backend("OpenGL 1.3", backend);
}
