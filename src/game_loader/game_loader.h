#pragma once

typedef struct GameFunctions {
    void (*_ready)(void);
    void (*_process)(double);
    void (*_physics_process)(double);
} GameFunctions;

GameFunctions load_game(void);
