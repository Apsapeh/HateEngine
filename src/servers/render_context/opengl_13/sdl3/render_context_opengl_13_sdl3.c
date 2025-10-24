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
#include "types/types.h"

/*struct RenderContextSurface {
    int a;
};*/

/* ====> Errors <==== */
#define NOT_MAIN_THREAD_ERROR "SDL3NotMainThread"
#define ANY_ERROR "SDL3AnyError"
/* ================== */


#define MAIN_THREAD_CHECK(function, end_block)                                                          \
    do {                                                                                                \
        if (!SDL_IsMainThread()) {                                                                      \
            LOG_ERROR(                                                                                  \
                    "WindowServer(SDL3)::" #function " must be called only from main thread or "        \
                    "via call_deferred/call_deferred_async"                                             \
            )                                                                                           \
            set_error(NOT_MAIN_THREAD_ERROR);                                                           \
            end_block                                                                                   \
        }                                                                                               \
    } while (0)


const static u32 INIT_FLAGS = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
static u8 g_isInit = 0;

static boolean _init(void) {
    if (!SDL_WasInit(INIT_FLAGS)) {
        if (!SDL_Init(INIT_FLAGS)) {
            set_error(ANY_ERROR);
            return false;
        }
        g_isInit = 1;
    }

    return true;
}

static boolean _quit(void) {
    if (g_isInit) {
        SDL_QuitSubSystem(INIT_FLAGS);
    }
    return true;
}


static SDL_GLContext g_openglSharedContext = NULL;
static RenderContextSurface* create_surface(WindowServerWindow* window) {
    ERROR_ARGS_CHECK_1(window, { return NULL; });
    MAIN_THREAD_CHECK(create_surface, { return NULL; });

    if (!g_openglSharedContext) {
        g_openglSharedContext = SDL_GL_CreateContext((SDL_Window*) window);

        if (!g_openglSharedContext) {
            LOG_FATAL("Failed to create shared OpenGL context. SDL Error: %s", SDL_GetError());
            set_error(ANY_ERROR);
            return NULL;
        }
    }

    return (RenderContextSurface*) window;
}


static boolean destroy_surface(RenderContextSurface* surface) {
    ERROR_ARG_CHECK(surface, { return false; });
    return true;
}


static RenderContextSurface* g_currentSurface = NULL;
static boolean surface_make_current(RenderContextSurface* surface) {
    ERROR_ARG_CHECK(surface, { return false; });
    MAIN_THREAD_CHECK(create_surface, { return false; });

    if (surface == g_currentSurface) {
        return true;
    }

    if (!SDL_GL_MakeCurrent((SDL_Window*) surface, g_openglSharedContext)) {
        LOG_FATAL("Failed to change OpenGL context. SDL Error: %s", SDL_GetError());
        set_error(ANY_ERROR);
        return false;
    }
    g_currentSurface = surface;

    return true;
}


static boolean surface_present(RenderContextSurface* surface) {
    ERROR_ARG_CHECK(surface, { return false; });
    MAIN_THREAD_CHECK(create_surface, { return false; });

    if (surface != g_currentSurface) {
        if (!surface_make_current(surface))
            return false;
    }
    if (!SDL_GL_SwapWindow((SDL_Window*) surface)) {
        LOG_FATAL("Failed to swap window's buffers. SDL Error: %s", SDL_GetError());
        set_error(ANY_ERROR);
        return false;
    }
    return true;
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
