#include "game_loader.h"
#include "game_loader_environment.h"

#include "platform/memory.h"
#include "platform/dylib.h"
#include "log.h"
#include "servers/render_context/render_context.h"
#include "servers/render_server/render_server.h"
#include "servers/window_server/window_server.h"
#include <stdlib.h>
#include <string.h>


void* runtime_proc_loader(const char* name);

GameFunctions load_game(void) {
    GameFunctions game_functions;

    GameLoaderEnvironment fn_env = load_environment();

    // Stage 1: Init var-functions
    fn_env._runtime_init(runtime_proc_loader);

    // Stage 2: Setup config
    // TODO: Add _setup logic
    fn_env._setup();
    render_context_load_backend("OpenGL 1.3", "SDL3");
    window_server_load_backend("SDL3");
    // render_server_load_backend("OpenGL 1.3");
    RenderContext._init();
    WindowServer._init();

    // Stage 3: Init servers
    fn_env._render_context_init(&RenderContext);
    fn_env._window_server_init(&WindowServer);
    // fn_env._render_server_init(&RenderServer);

    game_functions._ready = fn_env._ready;
    game_functions._process = fn_env._process;
    game_functions._physics_process = fn_env._physics_process;

    return game_functions;
}

// #include <api_sym_lookup_table.h>
// void* _runtime_proc_loader(const char* name) {
//     for (usize i = 0; i < sizeof(api_function_lookup_table) / sizeof(api_function_lookup_table[0]);
//          i++) {
//         if (strcmp(api_function_lookup_table[i].name, name) == 0) {
//             return api_function_lookup_table[i].ptr;
//         }
//     }
//     return NULL;
// }

#include <api_sym_lookup_table.h>
void* runtime_proc_loader(const char* name) {
    usize left = 0;
    usize right = sizeof(api_function_lookup_table) / sizeof(api_function_lookup_table[0]) - 1;

    while (left <= right) {
        usize mid = left + (right - left) / 2;
        usize cmp = strcmp(api_function_lookup_table[mid].name, name);

        if (cmp == 0)
            return api_function_lookup_table[mid].ptr;
        else if (cmp < 0)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return NULL;
}
