#include "game_loader.h"
#include "platform/memory.h"
#include "platform/dylib.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>


static dylib_handle* handle = NULL;
// static void at_exit(void) {
//     if (handle) {
//         dylib_close(handle);
//     }
// }

static void _ready_placeholder(void) {
}
static void _process_placeholder(double _) {
}
static void _physics_process_placeholder(double _) {
}

void* runtime_proc_loader(const char* name);

GameFunctions load_game(void) {
    GameFunctions game_functions;

    // atexit(at_exit);

    handle = dylib_open("build/linux/x86_64/release/libDev");
    // handle = dylib_open("build/mingw/i386/release/libDev");
    if (!handle) {
        HATE_FATAL("Failed to load game: %s", argv[1]);
    }

    void (*runtime_init)(void* (*) (const char* name)) =
            dylib_sym(handle, "___hate_engine_runtime_init");
    if (!runtime_init) {
        dylib_close(handle);
        HATE_FATAL("Failed to load game: %s", argv[1]);
    }

    runtime_init(runtime_proc_loader);

    void (*_ready)(void) = dylib_sym(handle, "_ready");
    if (_ready) {
        game_functions._ready = _ready;
    } else {
        game_functions._ready = _ready_placeholder;
        HATE_WARN("Game _ready function not found: %s", argv[1]);
    }

    void (*_process)(double) = dylib_sym(handle, "_process");
    if (_process) {
        game_functions._process = _process;
    } else {
        game_functions._process = _process_placeholder;
        HATE_WARN("Game _process function not found: %s", argv[1]);
    }

    void (*_physics_process)(double) = dylib_sym(handle, "_physics_process");
    if (_physics_process) {
        game_functions._physics_process = _physics_process;
    } else {
        game_functions._physics_process = _physics_process_placeholder;
        HATE_WARN("Game _physics_process function not found: %s", argv[1]);
    }

    return game_functions;
}

#include "api_sym_lookup_table.h"

void* runtime_proc_loader(const char* name) {
    for (int i = 0; i < sizeof(api_function_lookup_table) / sizeof(api_function_lookup_table[0]); i++) {
        if (strcmp(api_function_lookup_table[i].name, name) == 0) {
            return api_function_lookup_table[i].ptr;
        }
    }
    return NULL;
}
