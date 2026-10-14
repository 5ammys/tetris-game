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

void Game_restart(Game* game){
  Board_create(&game->board);
  game->isGameOver = false;

  game->currentPiece = Piece_createRandom(&game->piecesVector);
  game->nextPiece = Piece_createRandom(&game->piecesVector);
  game->lastDropTime = SDL_GetTicks();
  game->dropInterval = 1000;
  SDL_SetWindowTitle(game->window, "Tetris - TheVoids");
}

void Game_init(Game* game, const char* title, int x, int y, int w, int h, int flags, int delay){
  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    SDL_Log("Failed to initialize SDL: %s\n", SDL_GetError());
  }

  game->window = SDL_CreateWindow(title, x, y, w, h, flags);
  if(!game->window){
    SDL_Log("Failed to create window: %s\n", SDL_GetError());
    return;
  }

  game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
  if(!game->renderer){
    SDL_Log("Failed to create renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(game->window);
    return;
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
}

void Game_update(Game* game){
  if (game->isGameOver) return;


  // Update game logic here
}

void Game_render(Game* game){
  SDL_SetRenderDrawColor(game->renderer, 0, 30, 0, 255);
  SDL_RenderClear(game->renderer);

  //Renderizar tablero, piezas

  SDL_RenderPresent(game->renderer);
}

void Game_handleEvents(Game* game){
  SDL_Event event;
  while(SDL_PollEvent(&event)){

    if(event.type == SDL_QUIT) {
      game->isRunning = false;
      return;
    }

    if(event.type == SDL_KEYDOWN){
      SDL_Keycode key = event.key.keysym.sym;

      if(key == SDLK_ESCAPE){
        game->isRunning = false;
        return;
      }

      //Agregar: Comandos para controlar el juego
    }
  }

  /*
    Manejar I/O
  */
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
