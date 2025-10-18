#include "render_server_opengl_13.h"
#include "servers/render_server/render_server.h"
#include "error.h"
#include "log.h"

/* ====> Errors <==== */

/* ================== */

static Error _init(void) {
    return ERROR_SUCCESS;
}

static Error _quit(void) {
    return ERROR_SUCCESS;
}


#define REGISTER(fn) render_server_backend_set_function(backend, #fn, (void (*)(void)) fn)

void render_server_opengl_13_backend_register(void) {
    RenderServerBackend* backend = render_server_backend_new();

    REGISTER(_init);
    REGISTER(_quit);

    render_server_register_backend("OpenGL 1.3", backend);
}
