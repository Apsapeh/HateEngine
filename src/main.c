#include <string.h>
#include <stdio.h>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"
#include "platform/dylib.h"
#include "platform/memory.h"

#include "game_loader/game_loader.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindow("Hello", 800, 600, SDL_WINDOW_OPENGL);

    GameFunctions game_functions = load_game();
    game_functions._ready();

    SDL_Event event;
    for (int i = 0; i < 20000000; i++) {
        while (SDL_PollEvent(&event)) {
            printf("EVENT\n");
            if (event.type == SDL_EVENT_QUIT) {
                return 0;
            }
        }
        // game_functions._process(0.166);
    }

    return 0;
}
