#include <stdio.h>
#include <stdlib.h>
#ifdef __MINGW32__
    #define SDL_MAIN_HANDLED
    #include <SDL2/SDL_main.h>
#endif

#include <SDL2/SDL.h>
#include "structures/Game.h"

int main(int argc, char *argv[]) {
    Uint32 frameStart, frameTime;

    Game* game = Game_create();

    if(!game) {
        fprintf(stderr, "Error: cannot create game.\n");
        return 1;
    }

    if(Game_init(game,
              "Tetris - TheVoids",
              SDL_WINDOWPOS_CENTERED,
              SDL_WINDOWPOS_CENTERED,
              WINDOW_W,
              WINDOW_H,
              SDL_WINDOW_SHOWN
    ) == -1) {
        fprintf(stderr, "Error: cannot initialize game.\n");
        Game_clean(game);
        return 1;
    }

    while(Game_isRunning(game)) {
        frameStart = SDL_GetTicks();

        Game_handleEvents(game);
        Game_update(game);
        Game_render(game);

        frameTime = SDL_GetTicks() - frameStart;

        if(frameTime < (Uint32)DELAY_TIME) {
            SDL_Delay(DELAY_TIME - frameTime);
        }
    }

    Game_clean(game);
    return 0;
}
