#pragma once

#include <error.h>

typedef struct SurfaceBridge SurfaceBridge;

// [[API Generator]]
typedef struct {
    int n;
} SurfaceBridgeBackend;

// Static global WindowServer
extern SurfaceBridgeBackend Surface;

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
 * @brief Create a new SurfaceBridgeBackend instance
 * @return NULL if memory allocation fails
 */
SurfaceBridgeBackend* surface_bridge_backend_new(void);

// [[API Generator]]
/*
 * @brief Set a function pointer for a backend
 * @return "InvalidArgument" if name is NULL or func is NULL
 * @return "AlreadyExists" if a function with the same name is already registered
 */
Error surface_bridge_backend_set_function(
        SurfaceBridgeBackend* backend, const char* name, void (*function)(void)
);
