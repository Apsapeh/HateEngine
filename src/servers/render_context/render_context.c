#include "render_context.h"
#include "error.h"
#include "log.h"
#include <types/vector.h>

/* ==> Backends <== */
#include "opengl_13/sdl3/render_context_opengl_13_sdl3.h"
/* ================ */

RenderContextBackend RenderContext;

struct RenderContextBackendPair {
    const char* render_server_name;
    const char* window_server_name;
    RenderContextBackend* backend;
};


vector_template_def(RenderContextBackendPair, struct RenderContextBackendPair);
vector_template_impl(RenderContextBackendPair, struct RenderContextBackendPair);

static vec_RenderContextBackendPair registred_backends;

void render_context_init(void) {
    registred_backends = vec_RenderContextBackendPair_init();

    // Register default backend
    render_context_opengl_13_sdl3_backend_register();
}

Error render_context_register_backend(
        const char* render_server_name, const char* window_server_name, RenderContextBackend* backend
) {
    ERROR_ARGS_CHECK_3(render_server_name, window_server_name, backend);

    for (usize i = 0; i < registred_backends.size; i++) {
        if (strcmp(registred_backends.data[i].render_server_name, render_server_name) == 0 &&
            strcmp(registred_backends.data[i].window_server_name, window_server_name) == 0) {
            LOG_ERROR(
                    "Backend with RenderServer = '%s' and WindowServer = '%s' already registered",
                    render_server_name, window_server_name
            );
            return ERROR_ALREADY_EXISTS;
        }
    }

    vec_RenderContextBackendPair_push_back(
            &registred_backends,
            (struct RenderContextBackendPair) {render_server_name, window_server_name, backend}
    );

    return ERROR_SUCCESS;
}

Error render_context_load_backend(const char* render_server_name, const char* window_server_name) {
    ERROR_ARGS_CHECK_2(render_server_name, window_server_name);

    for (usize i = 0; i < registred_backends.size; i++) {
        if (strcmp(registred_backends.data[i].render_server_name, render_server_name) == 0 &&
            strcmp(registred_backends.data[i].window_server_name, window_server_name) == 0) {

            RenderContext = *registred_backends.data[i].backend;
            return ERROR_SUCCESS;
        }
    }

    return ERROR_NOT_FOUND;
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
    typedef void (**fn_t)(void);
    struct fn_pair {
        const char* name;
        fn_t function;
    };

    // TODO: generate with API Generator
    const struct fn_pair pairs[] = {
            {"_init", (fn_t) &backend->_init},
            {"_quit", (fn_t) &backend->_quit},
            {"create_surface", (fn_t) &backend->create_surface},
            {"destroy_surface", (fn_t) &backend->destroy_surface},
            {"surface_make_current", (fn_t) &backend->surface_make_current},
            {"surface_present", (fn_t) &backend->surface_present},
            {"get_proc_addr", (fn_t) &backend->get_proc_addr},
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
