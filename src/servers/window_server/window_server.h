#pragma once

#include <error.h>
#include <types/types.h>

/*
API ENUM {
        "name": "WindowServerWindowVSync",
        "type": "char",
        "values": [
                ["Disabled", "'d'"],
                ["Enabled", "'e'"],
                ["EnabledAsync", "'a'"]
        ]
}
*/

#define WINDOW_SERVER_WINDOW_VSYNC_DISABLED 'd'
#define WINDOW_SERVER_WINDOW_VSYNC_ENABLED 'e'
#define WINDOW_SERVER_WINDOW_VSYNC_ENABLED_ASYNC 'a'

/**
 * 'd' - disabled
 *
 * 'e' - enabled
 *
 * 'a' - enabled async
 *
 * @api
 */
typedef u8 WindowServerWindowVSync;


/*
API ENUM {
        "name": "WindowServerWindowMode",
        "type": "char",
        "values": [
                ["Windowed", "'w'"],
                ["Fullscreen", "'f'"],
                ["BorderlessFullscreen", "'b'"]
        ]
}
*/

#define WINDOW_SERVER_WINDOW_MODE_WINDOWED 'w'
#define WINDOW_SERVER_WINDOW_MODE_FULLSCREEN 'f'
#define WINDOW_SERVER_WINDOW_MODE_BORDERLESS_FULLSCREEN 'b'

/**
 * 'w' - windowed
 *
 * 'f' - exclusive fullscreen
 *
 * 'b' - borderless fullscreen
 *
 * @api
 */
typedef u8 WindowServerWindowMode;

/**
 * @brief
 *
 * @api
 */
typedef struct WindowServerWindow WindowServerWindow;

/**
 * @brief
 *
 * @api
 */
typedef struct WindowServerDisplay WindowServerDisplay;


/**
 * @api server
 * @api_config {
 *     "fn_prefix": "window_server_",
 *     "init_method": "___hate_engine_runtime_init_window_server"
 * }
 */
typedef struct {
    Error (*_init)(void);
    Error (*_quit)(void);

    Error (*create_window)(
            const char* title, i32 w, i32 h, WindowServerWindow* parent, WindowServerWindow** out
    );
    Error (*destroy_window)(WindowServerWindow* this);

    Error (*window_set_title)(WindowServerWindow* this, const char* title);
    Error (*window_get_title)(WindowServerWindow* this, const char** out);

    /**
     * @brief HOLA BOLA
     *
     */
    Error (*window_set_mode)(WindowServerWindow* this, WindowServerWindowMode mode);
    Error (*window_get_mode)(WindowServerWindow* this, WindowServerWindowMode* out);

    Error (*window_set_size)(WindowServerWindow* this, i32 w, i32 h);
    Error (*window_get_size)(WindowServerWindow* this, i32* w, i32* h);

    Error (*window_set_position)(WindowServerWindow* this, i32 x, i32 y);
    Error (*window_get_position)(WindowServerWindow* this, i32* x, i32* y);

    Error (*window_set_fullscreen_display)(WindowServerWindow* this, WindowServerDisplay* fullscreen);
} WindowServerBackend;


// Static global WindowServer
extern WindowServerBackend WindowServer;

/**
 * @brief Initialize the static variables and default backends
 */
void window_server_init(void);

/**
 * @brief Register a backend
 * @return "InvalidArgument" if name is NULL or backend is NULL
 * @return "AlreadyExists" if a backend with the same name is already registered
 *
 * @api
 */
Error window_server_register_backend(const char* name, WindowServerBackend* backend);

/**
 * @brief Load a backend. First you should register them via window_server_register_backend
 * @warning If the backend is already loaded, this function does nothing.
 * @return "InvalidArgument" if name is NULL
 * @return "NotFound" if a backend with the given name is not registered
 * @return "InvalidState" if the backend is already loaded
 *
 * @api
 */
Error window_server_load_backend(const char* name);


/* ====================> WindowServerBackend functions <==================== */

/**
 * @brief Create a new WindowServerBackend instance
 * @return NULL if memory allocation fails
 *
 * @api
 */
WindowServerBackend* window_server_backend_new(void);

/**
 * @brief Set a function pointer for a backend
 * @return "InvalidArgument" if name is NULL or func is NULL
 * @return "NotFound" if a function with the given name does not exist in the backend
 *
 * @api
 */
Error window_server_backend_set_function(
        WindowServerBackend* backend, const char* name, void (*function)(void)
);


/**
 * @brief Get a function pointer for a backend
 * @return "InvalidArgument" if backend is NULL or name is NULL or function is NULL
 * @return "NotFound" if a function with the given name is not registered
 *
 * @api
 */
Error window_server_backend_get_function(
        WindowServerBackend* backend, const char* name, void (**function)(void)
);
