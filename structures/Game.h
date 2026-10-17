#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "Board.h"
#include "Piece.h"
#include "Vector.h"
#include "Panel.h"

#define WINDOW_W 500
#define WINDOW_H 640

#define BOARD_OFFSET_X 20
#define BOARD_OFFSET_Y 20

typedef struct{
  SDL_Window* window;
  SDL_Renderer* renderer;
  bool isGameOver;
  bool isRunning;

  Board board;
  Vector piecesVector;
  Piece currentPiece;
  Piece nextPiece;

  int dropInterval;
  Uint32 lastDropTime;
}Game;

Game* Game_create();
void Game_init(Game* game, const char* title, int x, int y, int w, int h, int flags, int delay);
void Game_update(Game* game);
void Game_render(Game* game);
void Game_clean(Game* game);
void Game_handleEvents(Game* game);
bool Game_isRunning(Game* game);
void Game_restart(Game* game);

#endif
