#include "game_loader_environment.h"

#include <platform/dylib.h>
#include <log.h>

#define CHECK_FATAL(var, fn_name)                                                                       \
    do {                                                                                                \
        if (!var) {                                                                                     \
            dylib_close(handle);                                                                        \
            LOG_FATAL("Failed to load game: function '%s' was not found at game '%s'", fn_name, name);  \
        }                                                                                               \
    } while (0)

#define LOAD_WITH_CHECK_FATAL(var, fn_name)                                                             \
    do {                                                                                                \
        var = dylib_sym(handle, fn_name);                                                               \
        CHECK_FATAL(var, fn_name);                                                                      \
    } while (0)

#define PLACEHOLDER_WARN(var, fn_name, placeholder)                                                     \
    do {                                                                                                \
        if (!var) {                                                                                     \
            LOG_WARN("Failed to load game: function '%s' was not found at game '%s'", fn_name, name);   \
            var = placeholder;                                                                          \
        }                                                                                               \
    } while (0)

#define LOAD_OR_PLACEHOLDER_WARN(var, fn_name, placeholder)                                             \
    do {                                                                                                \
        var = dylib_sym(handle, fn_name);                                                               \
        PLACEHOLDER_WARN(var, fn_name, placeholder);                                                    \
    } while (0)


// clang-format off
static void _setup_placeholder(void) {}
static void _ready_placeholder(void) {}
static void _process_placeholder(double _) {}
static void _physics_process_placeholder(double _) {}
// clang-format on


c_str SETUP_FN = "_setup";
c_str READY_FN = "_ready";
c_str PROCESS_FN = "_process";
c_str PHYSICS_PROCESS_FN = "_physics_process";

c_str RUNTIME_FN_INIT = "___hate_engine_runtime_init";
c_str WINDOW_SERVER_FN_INIT = "___hate_engine_runtime_init_window_server";
c_str RENDER_CONTEXT_FN_INIT = "___hate_engine_runtime_init_render_context";
c_str RENDER_SERVER_FN_INIT = "___hate_engine_runtime_init_render_server";

GameLoaderEnvironment load_environment(void) {
    GameLoaderEnvironment result;

    // TODO: Get game lib from settings.toml
    c_str name = "libDev";

    dylib_handle* handle = dylib_open(name);
    if (!handle) {
        LOG_FATAL("Failed to load game: game '%s' was not fonud", name);
    }

    LOAD_WITH_CHECK_FATAL(result._runtime_init, RUNTIME_FN_INIT);
    LOAD_WITH_CHECK_FATAL(result._window_server_init, WINDOW_SERVER_FN_INIT);
    LOAD_WITH_CHECK_FATAL(result._render_context_init, RENDER_CONTEXT_FN_INIT);
    LOAD_WITH_CHECK_FATAL(result._render_server_init, RENDER_SERVER_FN_INIT);

    LOAD_OR_PLACEHOLDER_WARN(result._setup, SETUP_FN, _setup_placeholder);
    LOAD_OR_PLACEHOLDER_WARN(result._ready, READY_FN, _ready_placeholder);
    LOAD_OR_PLACEHOLDER_WARN(result._process, PROCESS_FN, _process_placeholder);
    LOAD_OR_PLACEHOLDER_WARN(result._physics_process, PHYSICS_PROCESS_FN, _physics_process_placeholder);

    return result;
}
