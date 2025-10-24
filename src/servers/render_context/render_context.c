#include "render_context.h"
#include "error.h"
#include "log.h"
#include <types/vector.h>

/* ==> Backends <== */
#include "opengl_13/sdl3/render_context_opengl_13_sdl3.h"
#include "types/types.h"
/* ================ */

RenderContextBackend RenderContext;

struct RenderContextBackendPair {
    const char* render_server_name;
    const char* window_server_name;
    RenderContextBackend* backend;
};

// clang-format off

vector_template_def_static(RenderContextBackendPair, struct RenderContextBackendPair)
vector_template_impl_static(RenderContextBackendPair, struct RenderContextBackendPair)

// FIXME: Add mutex
static vec_RenderContextBackendPair g_registredBackends;
static boolean g_isLoaded = false;
// clang-format on


void render_context_init(void) {
    g_registredBackends = vec_RenderContextBackendPair_init();

    // Register default backend
    render_context_opengl_13_sdl3_backend_register();
}

void render_context_exit(void) {
    for (usize i = 0; i < g_registredBackends.size; i++)
        render_context_backend_free(g_registredBackends.data[i].backend);

    vec_RenderContextBackendPair_free(&g_registredBackends);
}

Error render_context_register_backend(
        const char* render_server_name, const char* window_server_name, RenderContextBackend* backend
) {
    ERROR_ARGS_CHECK_3(render_server_name, window_server_name, backend);

    for (usize i = 0; i < g_registredBackends.size; i++) {
        if (strcmp(g_registredBackends.data[i].render_server_name, render_server_name) == 0 &&
            strcmp(g_registredBackends.data[i].window_server_name, window_server_name) == 0) {
            LOG_ERROR(
                    "Backend with RenderServer = '%s' and WindowServer = '%s' already registered",
                    render_server_name, window_server_name
            );
            return ERROR_ALREADY_EXISTS;
        }
    }

    vec_RenderContextBackendPair_push_back(
            &g_registredBackends,
            (struct RenderContextBackendPair) {render_server_name, window_server_name, backend}
    );

    return ERROR_SUCCESS;
}

Error render_context_load_backend(const char* render_server_name, const char* window_server_name) {
    ERROR_ARGS_CHECK_2(render_server_name, window_server_name);

    if (g_isLoaded) {
        LOG_ERROR("(render_context_load_backend) RenderContext already loaded");
        return ERROR_INVALID_STATE;
    }

    for (usize i = 0; i < g_registredBackends.size; i++) {
        if (strcmp(g_registredBackends.data[i].render_server_name, render_server_name) == 0 &&
            strcmp(g_registredBackends.data[i].window_server_name, window_server_name) == 0) {

            RenderContext = *g_registredBackends.data[i].backend;
            g_isLoaded = true;
            return ERROR_SUCCESS;
        }
    }

    return ERROR_NOT_FOUND;
}

boolean render_context_is_loaded(void) {
    return g_isLoaded;
}


/* ====================> RenderContextBackend functions <==================== */
static Error backend_set_get(
        RenderContextBackend* backend, const char* name, void (**fn)(void), u8 is_set
);

RenderContextBackend* render_context_backend_new(void) {
    RenderContextBackend* backend = tmalloc(sizeof(RenderContextBackend));

    // TODO: Add default functions

    return backend;
}

Error render_context_backend_free(RenderContextBackend* backend) {
    ERROR_ARG_CHECK(backend);
    tfree(backend);
    return ERROR_SUCCESS;
}

Error render_context_backend_set_function(
        RenderContextBackend* backend, const char* name, void (*function)(void)
) {
    ERROR_ARGS_CHECK_3(backend, name, function);
    return backend_set_get(backend, name, (void (**)(void)) function, 1);
}

Error render_context_backend_get_function(
        RenderContextBackend* backend, const char* name, void (**function)(void)
) {
    ERROR_ARGS_CHECK_3(backend, name, function);
    return backend_set_get(backend, name, function, 0);
}


static Error backend_set_get(
        RenderContextBackend* backend, const char* name, void (**fn)(void), u8 is_set
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
            {"create_surface", (FnT) &backend->create_surface},
            {"destroy_surface", (FnT) &backend->destroy_surface},
            {"surface_make_current", (FnT) &backend->surface_make_current},
            {"surface_present", (FnT) &backend->surface_present},
            {"get_proc_addr", (FnT) &backend->get_proc_addr},
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
