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
    // SDL_Init(SDL_INIT_VIDEO);

    vfs_init();
    window_server_init();
    render_context_init();

    if (window_server_load_backend("SDL3")) {
        LOG_FATAL("oenuhoaentsuh")
    }

    if (render_context_load_backend("OpenGL 1.3", "SDL3")) {
        LOG_FATAL("oenuhoaentsuh")
    }

    WindowServer._init();
    RenderContext._init();
    // LOG_FATAL("Server: %p", WindowServer.create_window);

    GameFunctions game_functions = load_game();
    game_functions._ready();

    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
        };
        game_functions._process(0.166);
        SDL_DelayNS(16660000);
    }

    /*WindowServerWindow* win;
    // WindowServer.create_window("Hello", 800, 600, NULL, &win);
    // WindowServer.window_set_title(win, "Тест");


    // SDL_Window* window = SDL_CreateWindow("Hello", 800, 600, SDL_WINDOW_OPENGL);
    SDL_Window* window = SDL_CreateWindow("Hello", 800, 600, SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(0);



    SDL_Event event;
    for (int i = 0; i < 200000; i++) {
        while (SDL_PollEvent(&event)) {
            printf("EVENT\n");
            if (event.type == SDL_EVENT_QUIT) {
                return 0;
            }
        }

        // printf("%d\n", i);
        //  game_functions._process(0.166);
        SDL_GL_SwapWindow(window);
    }*/

    return 0;
}
