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

boolean window_server_register_backend(const char* name, WindowServerBackend* backend) {
    ERROR_ARGS_CHECK_2(name, backend, { return false; });

    for (usize i = 0; i < g_registredBackends.size; i++) {
        if (strcmp(g_registredBackends.data[i].name, name) == 0) {
            LOG_ERROR("Backend with name '%s' already registered", name);
            set_error(ERROR_ALREADY_EXISTS);
            return false;
        }
    }

    vec_WindowServerBackendPair_push_back(
            &g_registredBackends, (struct WindowServerBackendPair) {name, backend}
    );

    return true;
}

boolean window_server_load_backend(const char* name) {
    ERROR_ARGS_CHECK_1(name, { return false; });

    if (g_isLoaded) {
        LOG_ERROR("(window_server_load_backend) Window server already loaded");
        set_error(ERROR_INVALID_STATE);
        return false;
    }

    for (usize i = 0; i < g_registredBackends.size; i++) {
        if (strcmp(g_registredBackends.data[i].name, name) == 0) {
            WindowServer = *g_registredBackends.data[i].backend;
            g_isLoaded = true;
            return true;
        }
    }

    set_error(ERROR_NOT_FOUND);
    return false;
}

boolean window_server_is_loaded(void) {
    return g_isLoaded;
}


/* ====================> WindowServerBackend functions <==================== */
static boolean backend_set_get(
        WindowServerBackend* backend, const char* name, void (**fn)(void), unsigned char is_set
);

WindowServerBackend* window_server_backend_new(void) {
    // FIXME: Add tmalloc check
    WindowServerBackend* backend = tmalloc(sizeof(WindowServerBackend));

    // TODO: Add default functions

    return backend;
}

boolean window_server_backend_free(WindowServerBackend* backend) {
    ERROR_ARG_CHECK(backend, { return false; });
    tfree(backend);
    return true;
}

boolean window_server_backend_set_function(
        WindowServerBackend* backend, const char* name, fptr function
) {
    ERROR_ARGS_CHECK_3(backend, name, function, { return false; });
    return backend_set_get(backend, name, (void (**)(void)) function, 1);
}

fptr window_server_backend_get_function(WindowServerBackend* backend, const char* name) {
    ERROR_ARGS_CHECK_2(backend, name, { return false; });
    fptr function = NULL;
    backend_set_get(backend, name, &function, 0);
    return function;
}


static boolean backend_set_get(
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
            // {"window_set_fullscreen_display", (FnT) &backend->window_set_fullscreen_display},
    };

    for (usize i = 0; i < sizeof(pairs) / sizeof(pairs[0]); i++) {
        if (!strcmp(name, pairs[i].name)) {
            if (is_set)
                *pairs[i].function = (void (*)(void)) fn;
            else
                *fn = *pairs[i].function;
            return true;
        }
    }

    LOG_ERROR("Unknown function name: %s", name);
    set_error(ERROR_NOT_FOUND);
    return false;
}
