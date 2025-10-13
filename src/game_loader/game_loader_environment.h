#pragma once

#include "servers/window_server/window_server.h"
#include "servers/render_context/render_context.h"
typedef struct GameLoaderEnvironment {
    void (*_setup)(void);
    void (*_ready)(void);
    void (*_process)(double);
    void (*_physics_process)(double);

    // Init functions
    void (*_runtime_init)(void* (*proc_addr)(const char* name));
    void (*_window_server_init)(WindowServerBackend* backend);
    void (*_render_context_init)(RenderContextBackend* backend);
} GameLoaderEnvironment;

GameLoaderEnvironment load_environment(void);
