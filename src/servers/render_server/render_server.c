#include "render_server.h"
#include "error.h"
#include "log.h"
#include <types/vector.h>

/* ==> Backends <== */
#include "opengl_13/render_server_opengl_13.h"
/* ================ */

RenderServerBackend RenderServer;

struct BackendPair {
    const char* name;
    RenderServerBackend* backend;
};


// clang-format off
vector_template_def_static(backendPair, struct BackendPair)
vector_template_impl_static(backendPair, struct BackendPair)

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

boolean render_server_register_backend(const char* name, RenderServerBackend* backend) {
    ERROR_ARGS_CHECK_2(name, backend, { return false; });

    for (usize i = 0; i < g_registredBackends.size; i++) {
        if (strcmp(g_registredBackends.data[i].name, name) == 0) {
            LOG_ERROR("Backend with name '%s' already registered", name);
            set_error(ERROR_ALREADY_EXISTS);
            return false;
        }
    }

    vec_backendPair_push_back(&g_registredBackends, (struct BackendPair) {name, backend});

    return true;
}

boolean render_server_load_backend(const char* name) {
    ERROR_ARGS_CHECK_1(name, { return false; });

    if (g_isLoaded) {
        LOG_ERROR("(render_server_load_backend) Window server already loaded");
        set_error(ERROR_INVALID_STATE);
        return false;
    }

    for (usize i = 0; i < g_registredBackends.size; i++) {
        if (strcmp(g_registredBackends.data[i].name, name) == 0) {
            RenderServer = *g_registredBackends.data[i].backend;
            g_isLoaded = true;
            return true;
        }
    }

    set_error(ERROR_NOT_FOUND);
    return false;
}

boolean render_server_is_loaded(void) {
    return g_isLoaded;
}


/* ====================> RenderServerBackend functions <==================== */
static boolean backend_set_get(
        RenderServerBackend* backend, const char* name, void (**fn)(void), unsigned char is_set
);

RenderServerBackend* render_server_backend_new(void) {
    // FIXME: Add tmalloc check
    RenderServerBackend* backend = tmalloc(sizeof(RenderServerBackend));

    // TODO: Add default functions

    return backend;
}

boolean render_server_backend_free(RenderServerBackend* backend) {
    ERROR_ARG_CHECK(backend, { return false; });
    tfree(backend);
    return true;
}

boolean render_server_backend_set_function(
        RenderServerBackend* backend, const char* name, fptr function
) {
    ERROR_ARGS_CHECK_3(backend, name, function, { return false; });
    return backend_set_get(backend, name, (void (**)(void)) function, 1);
}

fptr render_server_backend_get_function(RenderServerBackend* backend, const char* name) {
    ERROR_ARGS_CHECK_2(backend, name, { return false; });
    fptr function = NULL;
    backend_set_get(backend, name, &function, 0);
    return function;
}


static boolean backend_set_get(
        RenderServerBackend* backend, const char* name, void (**fn)(void), unsigned char is_set
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
