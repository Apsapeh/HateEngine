#pragma once

#include "servers/window_server/window_server.h"
#include "servers/render_context/render_context.h"
#include "servers/render_server/render_server.h"

typedef void (*SetupFn)(void);
typedef void (*ReadyFn)(void);
typedef void (*ProcessFn)(double);
typedef void (*PhysicsProcessFn)(double);

typedef void (*RuntimeInitFn)(void* (*proc_addr)(const char* name));
typedef void (*WindowServerInitFn)(WindowServerBackend* backend);
typedef void (*RenderContextInitFn)(RenderContextBackend* backend);
typedef void (*RenderServerIninFn)(RenderServerBackend* backend);

typedef struct GameLoaderEnvironment {
    SetupFn _setup;
    ReadyFn _ready;
    ProcessFn _process;
    PhysicsProcessFn _physics_process;

    // Init functions
    RuntimeInitFn _runtime_init;
    WindowServerInitFn _window_server_init;
    RenderContextInitFn _render_context_init;
    RenderServerIninFn _render_server_init;
} GameLoaderEnvironment;

GameLoaderEnvironment load_environment(void);
