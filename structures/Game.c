#include "Game.h"

Game* Game_create(){
  Game* game = (Game*)malloc(sizeof(Game));

  if(!game){
    SDL_Log("Failed to allocate memory for Game: %s\n", SDL_GetError());
    return NULL;
  }

  game->window = NULL;
  game->renderer = NULL;
  game->isRunning = false;
  game->isGameOver = false;
  return game;
}

int Game_init(Game* game, const char* title, int x, int y, int w, int h, int flags){
  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    SDL_Log("Failed to initialize SDL: %s\n", SDL_GetError());
  }

  game->window = SDL_CreateWindow(title, x, y, w, h, flags);
  if(!game->window){
    SDL_Log("Failed to create window: %s\n", SDL_GetError());
    return -1;
  }

  game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
  if(!game->renderer){
    SDL_Log("Failed to create renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(game->window);
    return -1;
  }

  srand((unsigned int) time(NULL));

  Piece_createAndLoadPiecesInVector(&game->piecesVector);

  game->isRunning   = true;
  game->isGameOver  = false;
  Board_create(&game->board);
  game->currentPiece = Piece_createRandom(&game->piecesVector);
  game->nextPiece    = Piece_createRandom(&game->piecesVector);
  game->lastDropTime = SDL_GetTicks();
  game->dropInterval = 1000;

  return 0;
}

void Game_restart(Game* game){
  Board_create(&game->board);
  game->isGameOver = false;

  game->currentPiece = Piece_createRandom(&game->piecesVector);
  game->nextPiece = Piece_createRandom(&game->piecesVector);
  game->lastDropTime = SDL_GetTicks();
  game->dropInterval = 1000;
  SDL_SetWindowTitle(game->window, "Tetris - TheVoids");
}


void Game_update(Game* game){
  if (game->isGameOver) return;

  Uint32 now = SDL_GetTicks();

  game->dropInterval = Board_getDropInterval(&game->board);

  if(now - game->lastDropTime >= (Uint32)game->dropInterval){
    game->lastDropTime = now;

    if(Board_isValidPosition(&game->board, &game->currentPiece,
                             game->currentPiece.row + 1,
                             game->currentPiece.col)){
      Piece_moveRow(&game->currentPiece, 1);
    } else {
      Board_lockPiece(&game->board, &game->currentPiece);

      int lines = Board_clearLines(&game->board);
      if(lines > 0){
        Board_updateScore(&game->board, lines);
      }

      game->currentPiece = game->nextPiece;
      game->nextPiece    = Piece_createRandom(&game->piecesVector);

      if(!Board_isValidPosition(&game->board, &game->currentPiece,
                                game->currentPiece.row,
                                game->currentPiece.col)){
        game->isGameOver = true;
        SDL_SetWindowTitle(game->window, "Tetris - GAME OVER");
      } else {
        char title[128];
        sprintf(title, "Tetris - Score: %d | Level: %d | Lines: %d",
                game->board.score, game->board.level, game->board.linesCleared);
        SDL_SetWindowTitle(game->window, title);
      }
    }
  }

}

void Game_render(Game* game){
  SDL_Renderer* r = game->renderer;

  SDL_SetRenderDrawColor(r, 8, 8, 18, 255);
  SDL_RenderClear(r);

  SDL_Rect boardBg = {
    .x = BOARD_OFFSET_X,
    .y = BOARD_OFFSET_Y,
    .w = BOARD_COLS * BOARD_CELLS_SIZE,
    .h = BOARD_ROWS * BOARD_CELLS_SIZE
  };

  SDL_SetRenderDrawColor(r, 18, 18, 35, 255);
  SDL_RenderFillRect(r, &boardBg);

  Board_draw(&game->board, r, BOARD_OFFSET_X, BOARD_OFFSET_Y, BOARD_CELLS_SIZE);

  Board_drawGrid(r, BOARD_OFFSET_X, BOARD_OFFSET_Y, BOARD_CELLS_SIZE);

  if(!game->isGameOver){
    Board_drawGhost(&game->board, &game->currentPiece, r, BOARD_OFFSET_X, BOARD_OFFSET_Y, BOARD_CELLS_SIZE);
  }

  Piece_draw(&game->currentPiece, r, BOARD_OFFSET_X, BOARD_OFFSET_Y, BOARD_CELLS_SIZE);

  Panel_render(r,&game->nextPiece, game->board.score, game->board.level, game->board.linesCleared);

  if(game->isGameOver){
    Panel_gameOver(r);
  }

  SDL_RenderPresent(r);
}

void Game_handleEvents(Game* game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT) {
            game->isRunning = false;
            return;
        }



        if (event.type == SDL_KEYDOWN) {
            SDL_Keycode key = event.key.keysym.sym;

            if (key == SDLK_ESCAPE) {
                game->isRunning = false;
                return;
            }

            if (key == SDLK_r) {
                Game_restart(game);
                return;
            }

            if (game->isGameOver) return;

            switch (key) {
                case SDLK_LEFT:
                    if (Board_isValidPosition(
                            &game->board,
                            &game->currentPiece,
                            game->currentPiece.row,
                            game->currentPiece.col - 1
                    )) {
                        Piece_moveCol(&game->currentPiece, -1);
                    }
                    break;

                case SDLK_RIGHT:
                    if (Board_isValidPosition(
                            &game->board,
                            &game->currentPiece,
                            game->currentPiece.row,
                            game->currentPiece.col + 1
                    )) {
                        Piece_moveCol(&game->currentPiece, 1);
                    }
                    break;

                case SDLK_DOWN:
                    if (Board_isValidPosition(
                            &game->board,
                            &game->currentPiece,
                            game->currentPiece.row + 1,
                            game->currentPiece.col
                    )) {
                        Piece_moveRow(&game->currentPiece, 1);
                        game->lastDropTime = SDL_GetTicks();
                    }
                    break;

                case SDLK_SPACE:
                    if (event.key.repeat) break;
                    while (Board_isValidPosition(
                            &game->board,
                            &game->currentPiece,
                            game->currentPiece.row + 1,
                            game->currentPiece.col
                    )) {
                        Piece_moveRow(&game->currentPiece, 1);
                    }
                    game->lastDropTime = 0;
                    break;

                case SDLK_UP: {
                    if (event.key.repeat) break;

                    Piece temp = game->currentPiece;
                    Piece_rotate(&temp);

                    if (Board_isValidPosition(&game->board, &temp, temp.row, temp.col)) {
                        game->currentPiece = temp;
                    }
                    else if (Board_isValidPosition(&game->board, &temp, temp.row, temp.col + 1)) {
                        temp.col += 1;
                        game->currentPiece = temp;
                    }
                    else if (Board_isValidPosition(&game->board, &temp, temp.row, temp.col - 1)) {
                        temp.col -= 1;
                        game->currentPiece = temp;
                    }
                    break;
                }
            }
        }
    }
}

void Game_clean(Game* game){
  SDL_DestroyRenderer(game->renderer);
  SDL_DestroyWindow(game->window);
  SDL_Quit();
  Vector_destroy(&game->piecesVector);
  free(game);
}

bool Game_isRunning(Game* game){
  return game->isRunning;
}
