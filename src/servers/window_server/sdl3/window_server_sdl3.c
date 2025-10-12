#include "window_server_sdl3.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_video.h"
#include "error.h"
#include "log.h"
#include "servers/window_server/window_server.h"

/* ====> Errors <==== */
#define NOT_MAIN_THREAD_ERROR "SDL3NotMainThread"
#define ANY_ERROR "SDL3AnyError"
/* ================== */


#define MAIN_THREAD_CHECK(function)                                                                     \
    do {                                                                                                \
        if (!SDL_IsMainThread()) {                                                                      \
            LOG_ERROR(                                                                                  \
                    "WindowServer(SDL3)::" #function " must be called only from main thread or "        \
                    "via call_deferred/call_deferred_async"                                             \
            )                                                                                           \
            return NOT_MAIN_THREAD_ERROR;                                                               \
        }                                                                                               \
    } while (0)


const unsigned int INIT_FLAGS = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
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


static Error create_window(
        const char* title, i32 w, i32 h, WindowServerWindow* parent, WindowServerWindow** out
) {
    MAIN_THREAD_CHECK(create_window);

    SDL_Window* window = SDL_CreateWindow(title, w, h, SDL_WINDOW_OPENGL);
    if (!window) {
        LOG_ERROR("Failed to create windowv SDL Error: %s", SDL_GetError());
        return ANY_ERROR;
    }
    *out = (WindowServerWindow*) window;
    return ERROR_SUCCESS;
}

static Error destroy_window(WindowServerWindow* this) {
    ERROR_ARGS_CHECK_1(this);
    MAIN_THREAD_CHECK(destroy_window);

    SDL_DestroyWindow((SDL_Window*) this);
    return ERROR_SUCCESS;
}


static Error window_set_title(WindowServerWindow* this, const char* title) {
    ERROR_ARGS_CHECK_2(this, title);
    MAIN_THREAD_CHECK(window_set_title);

    if (SDL_SetWindowTitle((SDL_Window*) this, title) != 0) {
        LOG_ERROR("Failed to set window title. SDL Error: %s", SDL_GetError());
        return "";
    }
    return ERROR_SUCCESS;
}

static Error window_get_title(WindowServerWindow* this, const char** out) {
    ERROR_ARGS_CHECK_2(this, out);
    MAIN_THREAD_CHECK(window_get_title);

    *out = SDL_GetWindowTitle((SDL_Window*) this);
    return ERROR_SUCCESS;
}


static Error window_set_mode(WindowServerWindow* this, WindowServerWindowMode mode) {
    ERROR_ARGS_CHECK_1(this);
    MAIN_THREAD_CHECK();

    // TODO: Implement this function
    return ERROR_NOT_IMPLEMENTED;

    return ERROR_SUCCESS;
}

static Error window_get_mode(WindowServerWindow* this, WindowServerWindowMode* out) {
    ERROR_ARGS_CHECK_2(this, out);
    MAIN_THREAD_CHECK();

    // TODO: Implement this function
    return ERROR_NOT_IMPLEMENTED;

    return ERROR_SUCCESS;
}


static Error window_set_size(WindowServerWindow* this, i32 w, i32 h) {
    ERROR_ARGS_CHECK_1(this);
    MAIN_THREAD_CHECK();

    if (SDL_SetWindowSize((SDL_Window*) this, w, h) != 0) {
        LOG_ERROR("Failed to set window size. SDL Error: %s", SDL_GetError());
        return ANY_ERROR;
    }
    return ERROR_SUCCESS;
}

static Error window_get_size(WindowServerWindow* this, i32* out_w, i32* out_h) {
    ERROR_ARGS_CHECK_3(this, out_w, out_h);
    MAIN_THREAD_CHECK();

    if (SDL_GetWindowSize((SDL_Window*) this, out_w, out_h) != 0) {
        LOG_ERROR("Failed to get window size. SDL Error: %s", SDL_GetError());
        return ANY_ERROR;
    }
    return ERROR_SUCCESS;
}


static Error window_set_position(WindowServerWindow* this, i32 x, i32 y) {
    ERROR_ARGS_CHECK_3(this, x, y);
    MAIN_THREAD_CHECK();

    if (SDL_SetWindowPosition((SDL_Window*) this, x, y) != 0) {
        LOG_ERROR("Failed to set window position. SDL Error: %s", SDL_GetError());
        return ANY_ERROR;
    }
    return ERROR_SUCCESS;
}

static Error window_get_position(WindowServerWindow* this, i32* out_x, i32* out_y) {
    ERROR_ARGS_CHECK_3(this, out_x, out_y);
    MAIN_THREAD_CHECK();

    if (SDL_GetWindowPosition((SDL_Window*) this, out_x, out_y) != 0) {
        LOG_ERROR("Failed to get window position. SDL Error: %s", SDL_GetError());
        return ANY_ERROR;
    }
    return ERROR_SUCCESS;
}


static Error window_set_fullscreen_display(WindowServerWindow* this, WindowServerDisplay* display) {
    ERROR_ARGS_CHECK_2(this, display);
    MAIN_THREAD_CHECK();

    // TODO: Implement
    return ERROR_NOT_IMPLEMENTED;

    return ERROR_SUCCESS;
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

    REGISTER(window_set_fullscreen_display);

    window_server_register_backend("SDL3", ws);
}
