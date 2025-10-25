#pragma once

#include <error.h>
#include <types/types.h>
#include "math/ivec2.h"

/*
API ENUM {
        "name": "WindowServerWindowVSync",
        "type": "char",
        "values": [
                ["Unknown", "'\\0'"],
                ["Disabled", "'d'"],
                ["Enabled", "'e'"],
                ["EnabledAsync", "'a'"]
        ]
}
*/

#define WINDOW_SERVER_WINDOW_VSYNC_UNKNOWN '\0'
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
                ["Unknown", "'\\0'"],
                ["Windowed", "'w'"],
                ["Fullscreen", "'f'"],
                ["BorderlessFullscreen", "'b'"]
        ]
}
*/

#define WINDOW_SERVER_WINDOW_MODE_UNKNOWN '\0'
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
    boolean (*_init)(void);
    boolean (*_quit)(void);

    WindowServerWindow* (*create_window)(const char* title, IVec2 size, WindowServerWindow* parent);
    boolean (*destroy_window)(WindowServerWindow* this);

    boolean (*window_set_title)(WindowServerWindow* this, const char* title);
    c_str (*window_get_title)(WindowServerWindow* this);

    boolean (*window_set_mode)(WindowServerWindow* this, WindowServerWindowMode mode);
    WindowServerWindowMode (*window_get_mode)(WindowServerWindow* this);

    boolean (*window_set_size)(WindowServerWindow* this, IVec2 dimensions);
    boolean (*window_get_size)(WindowServerWindow* this, IVec2* out);

    boolean (*window_set_position)(WindowServerWindow* this, IVec2 dimensions);
    boolean (*window_get_position)(WindowServerWindow* this, IVec2* out);

    // boolean (*window_set_fullscreen_display)(WindowServerWindow* this, WindowServerDisplay*
    // fullscreen);
} WindowServerBackend;


// Static global WindowServer
extern WindowServerBackend WindowServer;

/**
 * @brief Initialize the static variables and default backends
 */
void window_server_init(void);

void window_server_exit(void);

/**
 * @brief Register a backend
 * @return "InvalidArgument" if name is NULL or backend is NULL
 * @return "AlreadyExists" if a backend with the same name is already registered
 *
 * @api
 */
boolean window_server_register_backend(const char* name, WindowServerBackend* backend);

/**
 * @brief Load a backend. First you should register them via window_server_register_backend
 * @warning If the backend is already loaded, this function does nothing.
 * @return "InvalidArgument" if name is NULL
 * @return "NotFound" if a backend with the given name is not registered
 * @return "InvalidState" if the backend is already loaded
 *
 * @api
 */
boolean window_server_load_backend(const char* name);

/**
 * @brief If backend was loaded
 */
boolean window_server_is_loaded(void);


/* ====================> WindowServerBackend functions <==================== */

/**
 * @brief Create a new WindowServerBackend instance
 * @return NULL if memory allocation fails
 *
 * @api
 */
WindowServerBackend* window_server_backend_new(void);

/**
 * @brieif Free a WindowServerBackend instance
 * @return "InvalidArgument" if backned is NULL
 *
 * @api
 */
boolean window_server_backend_free(WindowServerBackend* backend);

/**
 * @brief Set a function pointer for a backend
 * @return "InvalidArgument" if name is NULL or func is NULL
 * @return "NotFound" if a function with the given name does not exist in the backend
 *
 * @api
 */
boolean window_server_backend_set_function(
        WindowServerBackend* backend, const char* name, fptr function
);


/**
 * @brief Get a function pointer for a backend
 * @return "InvalidArgument" if backend is NULL or name is NULL or function is NULL
 * @return "NotFound" if a function with the given name is not registered
 *
 * @api
 */
fptr window_server_backend_get_function(WindowServerBackend* backend, const char* name);
