#include "SDL3/SDL_error.h"
#include "platform/memory.h"
#include "render_context_opengl_13_sdl3.h"
#include "log.h"
#include "error.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_video.h"

#include "servers/render_context/render_context.h"
#include "servers/window_server/sdl3/window_server_sdl3.h"
#include "servers/window_server/window_server.h"

/*struct RenderContextSurface {
    int a;
};*/

/* ====> Errors <==== */
#define NOT_MAIN_THREAD_ERROR "SDL3NotMainThread"
#define ANY_ERROR "SDL3AnyError"
/* ================== */


#define MAIN_THREAD_CHECK(function)                                                                     \
    do {                                                                                                \
        if (!SDL_IsMainThread()) {                                                                      \
            LOG_ERROR(                                                                                  \
                    "RenderContext(OpenGL 1.3, SDL3)::" #function                                       \
                    " must be called only from main thread or "                                         \
                    "via call_deferred/call_deferred_async"                                             \
            )                                                                                           \
            return NOT_MAIN_THREAD_ERROR;                                                               \
        }                                                                                               \
    } while (0)


const static u32 INIT_FLAGS = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
static u8 is_init = 0;

static Error _init(void) {
    if (!SDL_WasInit(INIT_FLAGS)) {
        if (!SDL_Init(INIT_FLAGS)) {
            return ANY_ERROR;
        }
        is_init = 1;
    }

    return ERROR_SUCCESS;
}

static Error _quit(void) {
    if (is_init) {
        SDL_QuitSubSystem(INIT_FLAGS);
    }
    return ERROR_SUCCESS;
}


static SDL_GLContext OPENGL_SHARED_CONTEXT = NULL;
static Error create_surface(WindowServerWindow* window, RenderContextSurface** out) {
    ERROR_ARGS_CHECK_2(window, out);
    MAIN_THREAD_CHECK(create_surface);

    if (!OPENGL_SHARED_CONTEXT) {
        OPENGL_SHARED_CONTEXT = SDL_GL_CreateContext((SDL_Window*) window);

        if (!OPENGL_SHARED_CONTEXT) {
            LOG_FATAL("Failed to create shared OpenGL context. SDL Error: %s", SDL_GetError());
            return ANY_ERROR;
        }
    }

    *out = (RenderContextSurface*) window;
    return ERROR_SUCCESS;
}


static Error destroy_surface(RenderContextSurface* surface) {
    ERROR_ARG_CHECK(surface);
    return ERROR_SUCCESS;
}


static RenderContextSurface* CURRENT_SURFACE = NULL;
static Error surface_make_current(RenderContextSurface* surface) {
    ERROR_ARG_CHECK(surface);
    MAIN_THREAD_CHECK(create_surface);

    if (surface == CURRENT_SURFACE) {
        return ERROR_SUCCESS;
    }

    if (!SDL_GL_MakeCurrent((SDL_Window*) surface, OPENGL_SHARED_CONTEXT)) {
        LOG_FATAL("Failed to change OpenGL context. SDL Error: %s", SDL_GetError());
        return ANY_ERROR;
    }
    CURRENT_SURFACE = surface;

    return ERROR_SUCCESS;
}


static Error surface_present(RenderContextSurface* surface) {
    ERROR_ARG_CHECK(surface);
    MAIN_THREAD_CHECK(create_surface);

    if (surface != CURRENT_SURFACE) {
        Error err = surface_make_current(surface);
        if (err)
            return err;
    }
    if (!SDL_GL_SwapWindow((SDL_Window*) surface)) {
        LOG_FATAL("Failed to swap window's buffers. SDL Error: %s", SDL_GetError());
        return ANY_ERROR;
    }
    return ERROR_SUCCESS;
}


#define REGISTER(fn) render_context_backend_set_function(backend, #fn, (fptr) fn)

void render_context_opengl_13_sdl3_backend_register(void) {
    RenderContextBackend* backend = render_context_backend_new();

    REGISTER(_init);
    REGISTER(_quit);
    REGISTER(create_surface);
    REGISTER(destroy_surface);
    REGISTER(surface_make_current);
    REGISTER(surface_present);

    render_context_backend_set_function(backend, "get_proc_addr", (fptr) SDL_GL_GetProcAddress);

    render_context_register_backend("OpenGL 1.3", "SDL3", backend);
}
