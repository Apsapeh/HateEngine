#include "render_context.h"
#include "error.h"
#include "log.h"
#include <types/vector.h>

/* ==> Backends <== */
#include "opengl_13/sdl3/render_context_opengl_13_sdl3.h"
#include "types/types.h"
/* ================ */

RenderContextBackend RenderContext;

struct BackendPair {
    const char* render_server_name;
    const char* window_server_name;
    RenderContextBackend* backend;
};

// clang-format off

vector_template_def_static(backendPair, struct BackendPair)
vector_template_impl_static(backendPair, struct BackendPair)

// FIXME: Add mutex
static vec_backendPair g_registredBackends;
static boolean g_isLoaded = false;
// clang-format on


void render_context_init(void) {
    g_registredBackends = vec_backendPair_init();

    // Register default backend
    render_context_opengl_13_sdl3_backend_register();
}

void render_context_exit(void) {
    for (usize i = 0; i < g_registredBackends.size; i++)
        render_context_backend_free(g_registredBackends.data[i].backend);

    vec_backendPair_free(&g_registredBackends);
}

boolean render_context_register_backend(
        const char* render_server_name, const char* window_server_name, RenderContextBackend* backend
) {
    ERROR_ARGS_CHECK_3(render_server_name, window_server_name, backend, { return false; });

    for (usize i = 0; i < g_registredBackends.size; i++) {
        if (strcmp(g_registredBackends.data[i].render_server_name, render_server_name) == 0 &&
            strcmp(g_registredBackends.data[i].window_server_name, window_server_name) == 0) {
            LOG_ERROR(
                    "Backend with RenderServer = '%s' and WindowServer = '%s' already registered",
                    render_server_name, window_server_name
            );
            set_error(ERROR_ALREADY_EXISTS);
            return false;
        }
    }

    vec_backendPair_push_back(
            &g_registredBackends, (struct BackendPair) {render_server_name, window_server_name, backend}
    );

    return true;
}

boolean render_context_load_backend(const char* render_server_name, const char* window_server_name) {
    ERROR_ARGS_CHECK_2(render_server_name, window_server_name, { return false; });

    if (g_isLoaded) {
        LOG_ERROR("(render_context_load_backend) RenderContext already loaded");
        set_error(ERROR_INVALID_STATE);
        return false;
    }

    for (usize i = 0; i < g_registredBackends.size; i++) {
        if (strcmp(g_registredBackends.data[i].render_server_name, render_server_name) == 0 &&
            strcmp(g_registredBackends.data[i].window_server_name, window_server_name) == 0) {

            RenderContext = *g_registredBackends.data[i].backend;
            g_isLoaded = true;
            return true;
        }
    }

    set_error(ERROR_NOT_FOUND);
    return false;
}

boolean render_context_is_loaded(void) {
    return g_isLoaded;
}


/* ====================> RenderContextBackend functions <==================== */
static boolean backend_set_get(
        RenderContextBackend* backend, const char* name, void (**fn)(void), unsigned char is_set
);

RenderContextBackend* render_context_backend_new(void) {
    // FIXME: Add tmalloc check
    RenderContextBackend* backend = tmalloc(sizeof(RenderContextBackend));

    // TODO: Add default functions

    return backend;
}

boolean render_context_backend_free(RenderContextBackend* backend) {
    ERROR_ARG_CHECK(backend, { return false; });
    tfree(backend);
    return true;
}

boolean render_context_backend_set_function(
        RenderContextBackend* backend, const char* name, fptr function
) {
    ERROR_ARGS_CHECK_3(backend, name, function, { return false; });
    return backend_set_get(backend, name, (void (**)(void)) function, 1);
}

fptr render_context_backend_get_function(RenderContextBackend* backend, const char* name) {
    ERROR_ARGS_CHECK_2(backend, name, { return false; });
    fptr function = NULL;
    backend_set_get(backend, name, &function, 0);
    return function;
}


static boolean backend_set_get(
        RenderContextBackend* backend, const char* name, void (**fn)(void), unsigned char is_set
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
            return true;
        }
    }

    LOG_ERROR("Unknown function name: %s", name);
    set_error(ERROR_NOT_FOUND);
    return false;
}
