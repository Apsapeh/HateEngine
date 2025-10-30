// Define boolean flags
#define BOOLEAN_ARGS BOOLEAN_ARG(help, "-h", "Show help")

// Define optional arguments with defaults
#define OPTIONAL_ARGS                                                                                   \
    OPTIONAL_STRING_ARG(                                                                                \
            game_lib, "Dev", "--game-lib", "game-lib",                                                  \
            "Game library to load (without extension like .so, .dll and etc."                           \
    )


#include <args.h>
#include <easyargs.h>
#include "types/types.h"

c_str g_gameLibrary = NULL;

boolean parse_cli_args(int argc, char* argv[]) {
    args_t args = make_default_args();

    // Parse arguments
    if (!parse_args(argc, argv, &args) || args.help) {
        print_help(argv[0]);
        return false;
    }

    // LOG_FATAL_NO_ALLOC("game_library: %s", args.game_lib);
    g_gameLibrary = args.game_lib;
    return true;
}
