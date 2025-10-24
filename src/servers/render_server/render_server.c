#include "render_server.h"
#include "error.h"
#include "log.h"
#include <types/vector.h>

/* ==> Backends <== */
#include "opengl_13/render_server_opengl_13.h"
/* ================ */

RenderServerBackend RenderServer;

struct backendPair {
    const char* name;
    RenderServerBackend* backend;
};


// clang-format off
vector_template_def_static(backendPair, struct backendPair)
vector_template_impl_static(backendPair, struct backendPair)

// FIXME: Add mutex
static vec_backendPair g_registredBackends;
static boolean g_isLoaded = false;
// clang-format on

void render_server_init(void) {
    g_registredBackends = vec_backendPair_init();

    // Register default backend
    render_server_opengl_13_backend_register();
}

void render_server_exit(void) {
    for (usize i = 0; i < g_registredBackends.size; i++)
        render_server_backend_free(g_registredBackends.data[i].backend);

    vec_backendPair_free(&g_registredBackends);
}

Error render_server_register_backend(const char* name, RenderServerBackend* backend) {
    ERROR_ARGS_CHECK_2(name, backend);

    for (usize i = 0; i < g_registredBackends.size; i++) {
        if (strcmp(g_registredBackends.data[i].name, name) == 0) {
            LOG_ERROR("Backend with name '%s' already registered", name);
            return ERROR_ALREADY_EXISTS;
        }
    }

    vec_backendPair_push_back(&g_registredBackends, (struct backendPair) {name, backend});

    return ERROR_SUCCESS;
}

Error render_server_load_backend(const char* name) {
    ERROR_ARGS_CHECK_1(name);

    if (g_isLoaded) {
        LOG_ERROR("(render_server_load_backend) Render server already loaded");
        return ERROR_INVALID_STATE;
    }

    for (usize i = 0; i < g_registredBackends.size; i++) {
        if (strcmp(g_registredBackends.data[i].name, name) == 0) {
            RenderServer = *g_registredBackends.data[i].backend;
            g_isLoaded = true;
            return ERROR_SUCCESS;
        }
    }

    return ERROR_NOT_FOUND;
}

boolean render_server_is_loaded(void) {
    return g_isLoaded;
}

/* ====================> RenderServerBackend functions <==================== */
static Error backend_set_get(
        RenderServerBackend* backend, const char* name, void (**fn)(void), unsigned char is_set
);

RenderServerBackend* render_server_backend_new(void) {
    RenderServerBackend* backend = tmalloc(sizeof(RenderServerBackend));

    // TODO: Add default functions

    return backend;
}

Error render_server_backend_free(RenderServerBackend* backend) {
    ERROR_ARG_CHECK(backend);
    tfree(backend);
    return ERROR_SUCCESS;
}

Error render_server_backend_set_function(
        RenderServerBackend* backend, const char* name, void (*function)(void)
) {
    ERROR_ARGS_CHECK_3(backend, name, function);
    return backend_set_get(backend, name, (void (**)(void)) function, 1);
}

Error render_server_backend_get_function(
        RenderServerBackend* backend, const char* name, void (**function)(void)
) {
    ERROR_ARGS_CHECK_3(backend, name, function);
    return backend_set_get(backend, name, function, 0);
}


static Error backend_set_get(
        RenderServerBackend* backend, const char* name, void (**fn)(void), unsigned char is_set
) {
    typedef void (**FnT)(void);
    struct fn_pair {
        const char* name;
        FnT function;
    };

    // TODO: generate with API Generator
    const struct fn_pair pairs[] = {
            {"_init", (FnT) &backend->_init},
            {"_quit", (FnT) &backend->_quit},
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
