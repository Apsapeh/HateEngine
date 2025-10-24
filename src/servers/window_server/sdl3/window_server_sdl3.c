#include "window_server_sdl3.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_video.h"
#include "error.h"
#include "log.h"
#include "math/ivec2.h"
#include "servers/window_server/window_server.h"
#include "types/types.h"
#include "helpers.h"

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


static WindowServerWindow* create_window(const char* title, IVec2 size, WindowServerWindow* parent) {
    ERROR_ARGS_CHECK_1(title, { return NULL; });
    MAIN_THREAD_CHECK(create_window, { return NULL; });

    SDL_Window* window = SDL_CreateWindow(title, size.x, size.y, SDL_WINDOW_OPENGL);
    if (!window) {
        LOG_ERROR("Failed to create windowv SDL Error: %s", SDL_GetError());
        set_error(ANY_ERROR);
        return NULL;
    }
    return (WindowServerWindow*) window;
}

static boolean destroy_window(WindowServerWindow* this) {
    ERROR_ARGS_CHECK_1(this, { return false; });
    MAIN_THREAD_CHECK(destroy_window, { return false; });

    SDL_DestroyWindow((SDL_Window*) this);
    return true;
}


static boolean window_set_title(WindowServerWindow* this, const char* title) {
    ERROR_ARGS_CHECK_2(this, title, { return false; });
    MAIN_THREAD_CHECK(window_set_title, { return false; });

    if (!SDL_SetWindowTitle((SDL_Window*) this, title)) {
        LOG_ERROR("Failed to set window title. SDL Error: %s", SDL_GetError());
        set_error(ANY_ERROR);
        return false;
    }
    return true;
}

static c_str window_get_title(WindowServerWindow* this) {
    ERROR_ARG_CHECK(this, { return NULL; });
    MAIN_THREAD_CHECK(window_get_title, { return NULL; });

    return SDL_GetWindowTitle((SDL_Window*) this);
}


static boolean window_set_mode(WindowServerWindow* this, WindowServerWindowMode mode) {
    ERROR_ARGS_CHECK_1(this, { return false; });
    MAIN_THREAD_CHECK(window_set_mode, { return false; });

    // TODO: Implement this function
    set_error(ERROR_NOT_IMPLEMENTED);
    return false;

    return true;
}

static WindowServerWindowMode window_get_mode(WindowServerWindow* this) {
    ERROR_ARGS_CHECK_1(this, { return WINDOW_SERVER_WINDOW_MODE_UNKNOWN; });
    MAIN_THREAD_CHECK(window_get_mode, { return WINDOW_SERVER_WINDOW_MODE_UNKNOWN; });

    // TODO: Implement this function
    set_error(ERROR_NOT_IMPLEMENTED);
    return WINDOW_SERVER_WINDOW_MODE_UNKNOWN;

    return WINDOW_SERVER_WINDOW_MODE_UNKNOWN;
}


static boolean window_set_size(WindowServerWindow* this, IVec2 dimensions) {
    ERROR_ARGS_CHECK_1(this, { return false; });
    MAIN_THREAD_CHECK(window_set_size, { return false; });

    if (!SDL_SetWindowSize((SDL_Window*) this, dimensions.x, dimensions.y)) {
        LOG_ERROR("Failed to set window size. SDL Error: %s", SDL_GetError());
        set_error(ANY_ERROR);
        return false;
    }
    return true;
}

static IVec2 window_get_size(WindowServerWindow* this, boolean* success) {
    ERROR_ARGS_CHECK_1(this, {
        H_SET_SUCCESS_FALSE;
        return ivec2_new(-1, -1);
    });
    MAIN_THREAD_CHECK(window_get_size, {
        H_SET_SUCCESS_FALSE;
        return ivec2_new(-1, -1);
    });

    int out_w, out_h;
    if (!SDL_GetWindowSize((SDL_Window*) this, &out_w, &out_h)) {
        LOG_ERROR("Failed to get window size. SDL Error: %s", SDL_GetError());
        set_error(ANY_ERROR);
        H_SET_SUCCESS_FALSE;
        return ivec2_new(-1, -1);
    }

    H_SET_SUCCESS_TRUE;
    return ivec2_new((i32) out_w, (i32) out_h);
}


static boolean window_set_position(WindowServerWindow* this, IVec2 dimensions) {
    ERROR_ARGS_CHECK_1(this, { return false; });
    MAIN_THREAD_CHECK(window_set_position, { return false; });

    if (!SDL_SetWindowPosition((SDL_Window*) this, dimensions.x, dimensions.y)) {
        LOG_ERROR("Failed to set window position. SDL Error: %s", SDL_GetError());
        set_error(ANY_ERROR);
        return false;
    }
    return true;
}

static IVec2 window_get_position(WindowServerWindow* this, boolean* success) {
    ERROR_ARGS_CHECK_1(this, {
        H_SET_SUCCESS_FALSE;
        return ivec2_new(I32_MIN, I32_MIN);
    });
    MAIN_THREAD_CHECK(window_get_position, {
        H_SET_SUCCESS_FALSE;
        return ivec2_new(I32_MIN, I32_MIN);
    });

    int out_x, out_y;
    if (!SDL_GetWindowPosition((SDL_Window*) this, &out_x, &out_y)) {
        LOG_ERROR("Failed to get window position. SDL Error: %s", SDL_GetError());
        set_error(ANY_ERROR);
        H_SET_SUCCESS_FALSE;
        return ivec2_new(I32_MIN, I32_MIN);
    }

    H_SET_SUCCESS_TRUE;
    return ivec2_new((i32) out_x, (i32) out_y);
}


#define REGISTER(fn) window_server_backend_set_function(ws, #fn, (void (*)(void)) fn)

void window_server_sdl3_backend_register(void) {
    WindowServerBackend* ws = window_server_backend_new();

    REGISTER(_init);
    REGISTER(_quit);

    REGISTER(create_window);
    REGISTER(destroy_window);

    REGISTER(window_set_title);
    REGISTER(window_get_title);

    REGISTER(window_set_mode);
    REGISTER(window_get_mode);

    REGISTER(window_set_size);
    REGISTER(window_get_size);

    REGISTER(window_set_position);
    REGISTER(window_get_position);

    // REGISTER(window_set_fullscreen_display);

    window_server_register_backend("SDL3", ws);
}
