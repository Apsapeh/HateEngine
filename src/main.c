#include <string.h>
#include <stdio.h>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "error.h"
#include "log.h"
#include "platform/dylib.h"
#include "platform/memory.h"

#include "game_loader/game_loader.h"
#include "servers/render_context/render_context.h"
#include "types/result.h"
#include "vfs/vfs.h"
#include "servers/window_server/window_server.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


int main(int argc, char** argv) {
    LOG_WARN("Warn")
    LOG_ERROR("Error")
    // LOG_FATAL("Fatal")

    vfs_init();
    window_server_init();
    render_context_init();

    GameFunctions game_functions = load_game();
    game_functions._ready();

    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                return 0;
            }
        };
        game_functions._process(0.166);
        SDL_DelayNS(16660000);
    }

    return 0;
}
