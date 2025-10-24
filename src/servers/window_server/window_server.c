#include "window_server.h"
#include "error.h"
#include "log.h"
#include <types/vector.h>
#include "types/types.h"

/* ==> Backends <== */
#include "sdl3/window_server_sdl3.h"
/* ================ */

WindowServerBackend WindowServer;

struct WindowServerBackendPair {
    const char* name;
    WindowServerBackend* backend;
};


// clang-format off
vector_template_def_static(WindowServerBackendPair, struct WindowServerBackendPair)
vector_template_impl_static(WindowServerBackendPair, struct WindowServerBackendPair)

// FIXME: Add mutex
static vec_WindowServerBackendPair g_registredBackends;
static boolean g_isLoaded = false;
// clang-format on

void window_server_init(void) {
    g_registredBackends = vec_WindowServerBackendPair_init();

    // Register default backend
    window_server_sdl3_backend_register();
}

void window_server_exit(void) {
    for (usize i = 0; i < g_registredBackends.size; i++)
        window_server_backend_free(g_registredBackends.data[i].backend);

    vec_WindowServerBackendPair_free(&g_registredBackends);
}

Error window_server_register_backend(const char* name, WindowServerBackend* backend) {
    ERROR_ARGS_CHECK_2(name, backend);

    for (usize i = 0; i < g_registredBackends.size; i++) {
        if (strcmp(g_registredBackends.data[i].name, name) == 0) {
            LOG_ERROR("Backend with name '%s' already registered", name);
            return ERROR_ALREADY_EXISTS;
        }
    }

    vec_WindowServerBackendPair_push_back(
            &g_registredBackends, (struct WindowServerBackendPair) {name, backend}
    );

    return ERROR_SUCCESS;
}

Error window_server_load_backend(const char* name) {
    ERROR_ARGS_CHECK_1(name);

    if (g_isLoaded) {
        LOG_ERROR("(window_server_load_backend) Window server already loaded");
        return ERROR_INVALID_STATE;
    }

    for (usize i = 0; i < g_registredBackends.size; i++) {
        if (strcmp(g_registredBackends.data[i].name, name) == 0) {
            WindowServer = *g_registredBackends.data[i].backend;
            g_isLoaded = true;
            return ERROR_SUCCESS;
        }
    }

    return ERROR_NOT_FOUND;
}

boolean window_server_is_loaded(void) {
    return g_isLoaded;
}


/* ====================> WindowServerBackend functions <==================== */
static Error backend_set_get(
        WindowServerBackend* backend, const char* name, void (**fn)(void), unsigned char is_set
);

WindowServerBackend* window_server_backend_new(void) {
    WindowServerBackend* backend = tmalloc(sizeof(WindowServerBackend));

    // TODO: Add default functions

    return backend;
}

Error window_server_backend_free(WindowServerBackend* backend) {
    ERROR_ARG_CHECK(backend);
    tfree(backend);
    return ERROR_SUCCESS;
}

Error window_server_backend_set_function(
        WindowServerBackend* backend, const char* name, void (*function)(void)
) {
    ERROR_ARGS_CHECK_3(backend, name, function);
    return backend_set_get(backend, name, (void (**)(void)) function, 1);
}

Error window_server_backend_get_function(
        WindowServerBackend* backend, const char* name, void (**function)(void)
) {
    ERROR_ARGS_CHECK_3(backend, name, function);
    return backend_set_get(backend, name, function, 0);
}


static Error backend_set_get(
        WindowServerBackend* backend, const char* name, void (**fn)(void), unsigned char is_set
) {
    typedef void (**FnT)(void);
    struct FnPair {
        const char* name;
        FnT function;
    };

    // TODO: generate with API Generator
    const struct FnPair pairs[] = {
            {"_init", (FnT) &backend->_init},
            {"_quit", (FnT) &backend->_quit},
            {"create_window", (FnT) &backend->create_window},
            {"destroy_window", (FnT) &backend->destroy_window},
            {"window_set_title", (FnT) &backend->window_set_title},
            {"window_get_title", (FnT) &backend->window_get_title},
            {"window_set_mode", (FnT) &backend->window_set_mode},
            {"window_get_mode", (FnT) &backend->window_get_mode},
            {"window_set_size", (FnT) &backend->window_set_size},
            {"window_get_size", (FnT) &backend->window_get_size},
            {"window_set_position", (FnT) &backend->window_set_position},
            {"window_get_position", (FnT) &backend->window_get_position},
            {"window_set_fullscreen_display", (FnT) &backend->window_set_fullscreen_display},
    };

    for (usize i = 0; i < sizeof(pairs) / sizeof(pairs[0]); i++) {
        if (!strcmp(name, pairs[i].name)) {
            if (is_set)
                *pairs[i].function = (void (*)(void)) fn;
            else
                *fn = *pairs[i].function;
            return ERROR_SUCCESS;
        }
    }

    LOG_ERROR("Unknown function name: %s", name);
    return ERROR_NOT_FOUND;
}
