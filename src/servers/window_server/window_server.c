#include "window_server.h"
#include "error.h"
#include "log.h"
#include <types/vector.h>

/* ==> Backends <== */
#include "sdl3_backend/mod.h"
/* ================ */

WindowServerBackend WindowServer;

struct BackendPair {
    const char* name;
    WindowServerBackend* backend;
};


vector_template_def(BackendPair, struct BackendPair);
vector_template_impl(BackendPair, struct BackendPair);

static vec_BackendPair registred_backends;

void window_server_init(void) {
    registred_backends = vec_BackendPair_init();

    // Register default backend
    window_server_sdl3_backend_register();
}

Error window_server_register_backend(const char* name, WindowServerBackend* backend) {
    ERROR_ARGS_CHECK_2(name, backend)

    for (size_t i = 0; i < registred_backends.size; i++) {
        if (strcmp(registred_backends.data[i].name, name) == 0) {
            LOG_ERROR("Backend with name '%s' already registered", name);
            return ERROR_ALREADY_EXISTS;
        }
    }

    vec_BackendPair_push_back(&registred_backends, (struct BackendPair) {name, backend});

    return ERROR_SUCCESS;
}

Error window_server_load_backend(const char* name) {
    return ERROR_SUCCESS;
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

Error window_server_backend_set_function(
        WindowServerBackend* backend, const char* name, void (*function)(void)
) {
    if (!backend) {
        LOG_ERROR("Backend is NULL");
        return ERROR_INVALID_ARGUMENT;
    }
    if (!name) {
        LOG_ERROR("Function name is NULL");
        return ERROR_INVALID_ARGUMENT;
    }
    if (!function) {
        LOG_ERROR("Function pointer is NULL");
        return ERROR_INVALID_ARGUMENT;
    }

    return backend_set_get(backend, name, (void (**)(void)) function, 1);
}

Error window_server_backend_get_function(
        WindowServerBackend* backend, const char* name, void (**function)(void)
) {
    if (!backend) {
        LOG_ERROR("Backend is NULL");
        return ERROR_INVALID_ARGUMENT;
    }
    if (!name) {
        LOG_ERROR("Function name is NULL");
        return ERROR_INVALID_ARGUMENT;
    }
    if (!function) {
        LOG_ERROR("Function pointer is NULL");
        return ERROR_INVALID_ARGUMENT;
    }

    return backend_set_get(backend, name, function, 0);
}


static Error backend_set_get(
        WindowServerBackend* backend, const char* name, void (**fn)(void), unsigned char is_set
) {
    typedef void (**fn_t)(void);
    struct fn_pair {
        const char* name;
        fn_t function;
    };

    const struct fn_pair pairs[] = {
            {"create_window", (fn_t) &backend->create_window},
            {"destroy_window", (fn_t) &backend->destroy_window},
            {"window_set_title", (fn_t) &backend->window_set_title},
            {"window_get_title", (fn_t) &backend->window_get_title},
            {"window_set_size", (fn_t) &backend->window_set_size},
            {"window_get_size", (fn_t) &backend->window_get_size},
    };

    for (int i = 0; i < sizeof(pairs) / sizeof(pairs[0]); i++) {
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
