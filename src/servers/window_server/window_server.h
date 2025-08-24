#pragma once

#include <error.h>

enum WindowServerWindowVSync {
    WindowServerVsyncDisabled = 0,
    WindowServerVsyncEnabled = 1,
    WindowServerVsyncEnabledAsync = 2,
    WindowServerWindowUnknown = 100
};

enum WindowServerWindowMode {
    WindowServerWindowModeWindowed = 0,
    WindowServerWindowModeFullscreen = 1,
    WindowServerWindowModeBorderlessFullscreen = 2,
    WindowServerWindowModeUnknown = 100
};

typedef struct WindowServerWindow WindowServerWindow;

typedef struct WindowServerDisplay WindowServerDisplay;

// #define FN(fn, ...) (*fn)(__VA_ARGS__)
// #define METHOD(fn) FN(fn, WindowServerWindow* this)
// #define METHOD_ARGS(fn, ...) FN(fn, WindowServerWindow* this, __VA_ARGS__)

/// [[API Generator]]
// DECLARE_RESULT(WindowServerWindowPtr, WindowServerWindow*);
/// [[API Generator]]
// DECLARE_RESULT(WindowServerWindowVSyncEnum, enum WindowServerWindowVSync);
/// [[API Generator]]
// DECLARE_RESULT(WindowServerWindowModeEnum, enum WindowServerWindowMode);


typedef Error (*WindowServerBackend_create_window)(
        const char* title, int w, int h, WindowServerWindow* parent, WindowServerWindow** out
);

typedef Error (*WindowServerBackend_destroy_window)(WindowServerWindow* this);

typedef Error (*WindowServerBackend_window_set_title)(WindowServerWindow* this, const char* title);
typedef Error (*WindowServerBackend_window_get_title)(WindowServerWindow* this, const char** out);

typedef Error (*WindowServerBackend_window_set_vsync)(
        WindowServerWindow* this, enum WindowServerWindowVSync vsync
);
typedef Error (*WindowServerBackend_window_get_vsync)(
        WindowServerWindow* this, enum WindowServerWindowVSync* out
);

typedef Error (*WindowServerBackend_window_set_mode)(
        WindowServerWindow* this, enum WindowServerWindowMode mode
);
typedef Error (*WindowServerBackend_window_get_mode)(
        WindowServerWindow* this, enum WindowServerWindowMode* out
);

typedef Error (*WindowServerBackend_window_set_size)(WindowServerWindow* this, int w, int h);
typedef Error (*WindowServerBackend_window_get_size)(WindowServerWindow* this, int* w, int* h);

typedef Error (*WindowServerBackend_window_set_position)(WindowServerWindow* this, int x, int y);
typedef Error (*WindowServerBackend_window_get_position)(WindowServerWindow* this, int* x, int* y);

typedef Error (*WindowServerBackend_window_set_fullscreen_display)(
        WindowServerWindow* this, WindowServerDisplay* fullscreen
);

// [[API Generator]]
typedef struct {
    WindowServerBackend_create_window create_window;
    WindowServerBackend_destroy_window destroy_window;

    WindowServerBackend_window_set_title window_set_title;
    WindowServerBackend_window_get_title window_get_title;

    WindowServerBackend_window_set_vsync window_set_vsync;
    WindowServerBackend_window_get_vsync window_get_vsync;

    WindowServerBackend_window_set_mode window_set_mode;
    WindowServerBackend_window_get_mode window_get_mode;

    WindowServerBackend_window_set_size window_set_size;
    WindowServerBackend_window_get_size window_get_size;

    WindowServerBackend_window_set_position window_set_position;
    WindowServerBackend_window_get_position window_get_position;

    WindowServerBackend_window_set_fullscreen_display window_set_fullscreen_display;
} WindowServerBackend;

// Static global WindowServer
extern WindowServerBackend WindowServer;

//
// extern WindowServerWindow


/*
 * @brief Initialize the static variables and default backends
 */
void window_server_init(void);

// [[API Generator]]
/*
 * @brief Register a backend
 * @return "InvalidArgument" if name is NULL or backend is NULL
 * @return "AlreadyExists" if a backend with the same name is already registered
 */
Error window_server_register_backend(const char* name, WindowServerBackend* backend);

// [[API Generator]]
/*
 * @brief Load a backend. First you should register them via window_server_register_backend
 * @warning If the backend is already loaded, this function does nothing.
 * @return "InvalidArgument" if name is NULL
 * @return "NotFound" if a backend with the given name is not registered
 * @return "InvalidState" if the backend is already loaded
 */
Error window_server_load_backend(const char* name);


/* ====================> WindowServerBackend functions <==================== */

// [[API Generator]]
/*
 * @brief Create a new WindowServerBackend instance
 * @return NULL if memory allocation fails
 */
WindowServerBackend* window_server_backend_new(void);

// [[API Generator]]
/*
 * @brief Set a function pointer for a backend
 * @return "InvalidArgument" if name is NULL or func is NULL
 * @return "AlreadyExists" if a function with the same name is already registered
 */
Error window_server_backend_set_function(
        WindowServerBackend* backend, const char* name, void (*function)(void)
);
