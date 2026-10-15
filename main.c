#include <stdio.h>
#include <stdlib.h>
#ifdef __MINGW32__
    #define SDL_MAIN_HANDLED
    #include <SDL2/SDL_main.h>
#endif

#include <SDL2/SDL.h>
#include "structures/Game.h"

#define FPS 60
#define DELAY_TIME (1000.0f/FPS)

int main(int argc, char *argv[]) {
    Uint32 frameStart, frameTime;

    Game* game = Game_create();
    if(!game) {
        fprintf(stderr, "Error: no se pudo crear el juego.\n");
        return 1;
    }

    Game_init(game,
              "Tetris - ",
              SDL_WINDOWPOS_CENTERED,
              SDL_WINDOWPOS_CENTERED,
              WINDOW_W,
              WINDOW_H,
              SDL_WINDOW_SHOWN,
              0);

    while(Game_isRunning(game)) 
    {
        frameStart=SDL_GetTicks(); //marca la hora exacta al inicio del frame

        Game_handleEvents(game);    //procesa entradas (teclado/mouse)
        Game_update(game);          //modifica la logica (mover piezas, colisiones)
        Game_render(game);          //dibuja la escena en pantalla

        frameTime=SDL_GetTicks()-frameStart; //cuanto tiempo tardo desde que se invoco getTicks

        //control de FPS (Frame Limiter)
        if(frameTime<(Uint32)DELAY_TIME) 
        {
            SDL_Delay(DELAY_TIME-frameTime); //congela el juego los ms sobrantes
        }
    }

    Game_clean(game);
    return 0;
}
