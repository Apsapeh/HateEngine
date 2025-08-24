#include "mod.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_video.h"
#include "error.h"
#include "log.h"
#include "servers/window_server/window_server.h"

/* ====> Errors <==== */
#define NOT_MAIN_THREAD_ERROR "SDL3NotMainThread"
#define NOT_INITIALIZED_ERROR "SDL3NotInitialized"
#define ANY_ERROR "SDL3AnyError"
/* ================== */


#define MAIN_THREAD_CHECK(function)                                                                     \
    if (!SDL_IsMainThread()) {                                                                          \
        LOG_ERROR(                                                                                      \
                "WindowServer(SDL3)::" #function " must be called only from main thread or "            \
                "via call_deferred/call_deferred_async"                                                 \
        )                                                                                               \
        return NOT_MAIN_THREAD_ERROR;                                                                   \
    }

static Error create_window(
        const char* title, int w, int h, WindowServerWindow* parent, WindowServerWindow** out
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
    ERROR_ARGS_CHECK_1(this)
    MAIN_THREAD_CHECK(destroy_window)

    SDL_DestroyWindow((SDL_Window*) this);
    return ERROR_SUCCESS;
}


static Error window_set_title(WindowServerWindow* this, const char* title) {
    ERROR_ARGS_CHECK_2(this, title)
    MAIN_THREAD_CHECK(window_set_title);

    if (SDL_SetWindowTitle((SDL_Window*) this, title) != 0) {
        LOG_ERROR("Failed to set window title. SDL Error: %s", SDL_GetError());
        return "";
    }
    return ERROR_SUCCESS;
}

static Error window_get_title(WindowServerWindow* this, const char** out) {
    ERROR_ARGS_CHECK_2(this, out)
    MAIN_THREAD_CHECK(window_get_title);

    *out = SDL_GetWindowTitle((SDL_Window*) this);
    return ERROR_SUCCESS;
}


static Error window_set_vsync(WindowServerWindow* this, enum WindowServerWindowVSync vsync) {
    ERROR_ARGS_CHECK_1(this)
    MAIN_THREAD_CHECK();

    // TODO: Implement this function
    return ERROR_NOT_IMPLEMENTED;

    return ERROR_SUCCESS;
}

static Error window_get_vsync(WindowServerWindow* this, enum WindowServerWindowVSync* out) {
    // TODO: Implement this function
    return ERROR_NOT_IMPLEMENTED;

    return ERROR_SUCCESS;
}


static Error window_set_mode(WindowServerWindow* this, enum WindowServerWindowMode mode) {
    ERROR_ARGS_CHECK_1(this)
    MAIN_THREAD_CHECK();

    // TODO: Implement this function
    return ERROR_NOT_IMPLEMENTED;

    return ERROR_SUCCESS;
}

static Error window_get_mode(WindowServerWindow* this, enum WindowServerWindowMode* out) {
    ERROR_ARGS_CHECK_2(this, out)
    MAIN_THREAD_CHECK();

    // TODO: Implement this function
    return ERROR_NOT_IMPLEMENTED;

    return ERROR_SUCCESS;
}


static Error window_set_size(WindowServerWindow* this, int w, int h) {
    ERROR_ARGS_CHECK_1(this)
    MAIN_THREAD_CHECK();

    if (SDL_SetWindowSize((SDL_Window*) this, w, h) != 0) {
        LOG_ERROR("Failed to set window size. SDL Error: %s", SDL_GetError());
        return ANY_ERROR;
    }
    return ERROR_SUCCESS;
}

static Error window_get_size(WindowServerWindow* this, int* out_w, int* out_h) {
    ERROR_ARGS_CHECK_3(this, out_w, out_h)
    MAIN_THREAD_CHECK();

    if (SDL_GetWindowSize((SDL_Window*) this, out_w, out_h) != 0) {
        LOG_ERROR("Failed to get window size. SDL Error: %s", SDL_GetError());
        return ANY_ERROR;
    }
    return ERROR_SUCCESS;
}


static Error window_set_position(WindowServerWindow* this, int x, int y) {
    ERROR_ARGS_CHECK_3(this, x, y)
    MAIN_THREAD_CHECK();

    if (SDL_SetWindowPosition((SDL_Window*) this, x, y) != 0) {
        LOG_ERROR("Failed to set window position. SDL Error: %s", SDL_GetError());
        return ANY_ERROR;
    }
    return ERROR_SUCCESS;
}

static Error window_get_position(WindowServerWindow* this, int* out_x, int* out_y) {
    ERROR_ARGS_CHECK_3(this, out_x, out_y)
    MAIN_THREAD_CHECK();

    if (SDL_GetWindowPosition((SDL_Window*) this, out_x, out_y) != 0) {
        LOG_ERROR("Failed to get window position. SDL Error: %s", SDL_GetError());
        return ANY_ERROR;
    }
    return ERROR_SUCCESS;
}


static Error window_set_fullscreen_display(WindowServerWindow* this, WindowServerDisplay* display) {
    ERROR_ARGS_CHECK_2(this, display)
    MAIN_THREAD_CHECK();

    // TODO: Implement
    return ERROR_NOT_IMPLEMENTED;

    return ERROR_SUCCESS;
}


WindowServerBackend window_server_sdl3_backend_init(void) {
    WindowServerBackend ws = {
            .create_window = create_window,
            .destroy_window = destroy_window,

            .window_set_title = window_set_title,
            .window_get_title = window_get_title,

            .window_set_vsync = window_set_vsync,
            .window_get_vsync = window_get_vsync,

            .window_set_mode = window_set_mode,
            .window_get_mode = window_get_mode,

            .window_set_size = window_set_size,
            .window_get_size = window_get_size,

            .window_set_position = window_set_position,
            .window_get_position = window_get_position,

            .window_set_fullscreen_display = window_set_fullscreen_display,
    };
    return ws;
}
