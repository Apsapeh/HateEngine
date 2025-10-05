#pragma once

// #include <error.h>

// typedef struct SurfaceBridge SurfaceBridge;

// // [[API Generator]]
// typedef struct {
//     int n;
// } RenderContextBackend;


// /*
//  * @brief Initialize the static variables and default backends
//  */
// void render_context_init(void);

// /*
//  * @brief Load a backend. First you should register them via window_server_register_backend
//  * @warning It's private function, engine automatically loads the backends by already loaded render
//  and
//  * window servers
//  * @return "NotFound" if a backend with the given name is not registered
//  * @return "InvalidState" if the backend is already loaded
//  */
// Error render_context_load_backend(const char* render_backend_name, const char* window_backend_name);

// // [[API Generator]]
// /*
//  * @brief Register a backend
//  * @return "InvalidArgument" if name is NULL or backend is NULL
//  * @return "AlreadyExists" if a backend with the same name is already registered
//  */
// Error render_context_register_backend(
//         const char* render_backend_name, const char* window_backend_name, RenderContextBackend*
//         backend
// );


// /* ====================> WindowServerBackend functions <==================== */

// // [[API Generator]]
// /*
//  * @brief Create a new SurfaceBridgeBackend instance
//  * @return NULL if memory allocation fails
//  */
// RenderContextBackend* render_context_backend_new(void);

// // [[API Generator]]
// /*
//  * @brief Set a function pointer for a backend
//  * @return "InvalidArgument" if name is NULL or func is NULL
//  * @return "AlreadyExists" if a function with the same name is already registered
//  */
// Error render_context_backend_set_function(
//         RenderContextBackend* backend, const char* name, void (*function)(void)
// );
