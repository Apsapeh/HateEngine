#pragma once

#include "servers/window_server/window_server.h"
typedef struct GameFunctions {
    void (*_ready)(void);
    void (*_process)(double);
    void (*_physics_process)(double);

    // Init functions

    // void (*___hate_engine_runtime_init)(void* (*proc_addr)(const char* name));
    // void (*___hate_engine_runtime_init_window_server)(WindowServerBackend* backend);
} GameFunctions;

GameFunctions load_game(void);
