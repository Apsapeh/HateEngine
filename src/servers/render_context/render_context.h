#pragma once

#include <error.h>
#include <types/types.h>

#include <servers/window_server/window_server.h>

/**
 * @brief
 *
 * @api
 */
typedef struct RenderContextSurface RenderContextSurface;

/**
 * @api server
 * @api_config {
 *     "fn_prefix": "render_context_",
 *     "init_method": "___hate_engine_runtime_init_render_context"
 * }
 */
typedef struct {
    Error (*_init)(void);
    Error (*_quit)(void);

    Error (*create_surface)(WindowServerWindow* window, RenderContextSurface** out);
    Error (*destroy_surface)(RenderContextSurface* surface);

    Error (*surface_make_current)(RenderContextSurface* surface);
    Error (*surface_present)(RenderContextSurface* surface);

    fptr (*get_proc_addr)(const char* proc);
} RenderContextBackend;


// Static global WindowServer
extern RenderContextBackend RenderContext;

/**
 * @brief Initialize the static variables and default backends
 */
void render_context_init(void);

/**
 * @brief Register a backend
 * @return "InvalidArgument" if render_server_name is NULL or window_server_name is NULL or backend is
 * NULL
 * @return "AlreadyExists" if a backend with the same render_server_name and window_server_name is
 * already registered
 *
 * @api
 */
Error render_context_register_backend(
        const char* render_server_name, const char* window_server_name, RenderContextBackend* backend
);

/**
 * @brief Load a backend. First you should register them via render_context_register_backend
 * @warning If the backend is already loaded, this function does nothing.
 * @return "InvalidArgument" if render_server_name is NULL or window_server_name
 * @return "NotFound" if a backend with the given names is not registered
 * @return "InvalidState" if the backend is already loaded
 *
 * @api
 */
Error render_context_load_backend(const char* render_server_name, const char* window_server_name);


/* ====================> WindowServerBackend functions <==================== */

/**
 * @brief Create a new RenderContextBackend instance
 * @return NULL if memory allocation fails
 *
 * @api
 */
RenderContextBackend* render_context_backend_new(void);

/**
 * @brief Set a function pointer for a backend
 * @return "InvalidArgument" if name is NULL or func is NULL
 * @return "NotFound" if a function with the given name does not exist in the backend
 *
 * @api
 */
Error render_context_backend_set_function(
        RenderContextBackend* backend, const char* name, void (*function)(void)
);


/**
 * @brief Get a function pointer for a backend
 * @return "InvalidArgument" if backend is NULL or name is NULL or function is NULL
 * @return "NotFound" if a function with the given name is not registered
 *
 * @api
 */
Error render_context_backend_get_function(
        RenderContextBackend* backend, const char* name, void (**function)(void)
);
