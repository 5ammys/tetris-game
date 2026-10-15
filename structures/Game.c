#include "Game.h"

static Game* game = NULL;



Game* Game_create(){
  Game* game = (Game*)malloc(sizeof(Game));

  if(!game){
    SDL_Log("Failed to allocate memory for Game: %s\n", SDL_GetError());
    return NULL;
  }

  game->window=NULL;
  game->renderer=NULL;
  game->isRunning=false;
  game->isGameOver=false;
  return game;
}

void Game_reset(Game* game)
{
    if (!game) return;

    //limpiar la matriz del tablero y puntajes
    Board_create(&game->board);

    //reiniciar estados de la partida
    game->isGameOver=false;
    game->dropInterval=1000; //velocidad inicial (1 segundo)
    game->lastDropTime=SDL_GetTicks();

    //generar nuevas piezas
    game->currentPiece=Piece_createRandom();
    game->nextPiece=Piece_createRandom();
}

void Game_init(Game* game, const char* title, int x, int y, int w, int h, int flags, int delay)
{

    if (TTF_Init()==-1)
    {
        SDL_Log("Error al inicializar SDL2_ttf: %s", TTF_GetError());
        return;
    }

    //cargar fuente de tamanio 18px
    game->font=TTF_OpenFont("arial.ttf", 18);
    if (!game->font)
    {
        SDL_Log("Error al cargar la fuente: %s", TTF_GetError());
    }

    if(SDL_Init(SDL_INIT_VIDEO)<0)
    {
        SDL_Log("Failed to initialize SDL: %s\n", SDL_GetError());
    }

    game->window=SDL_CreateWindow(title, x, y, w, h, flags);
    if(!game->window)
    {
        SDL_Log("Failed to create window: %s\n", SDL_GetError());
        return;
    }

    game->renderer=SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if(!game->renderer)
    {
        SDL_Log("Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(game->window);
        return;
    }

    srand((unsigned int) time(NULL));

    game->isRunning=true;
    game->isGameOver=false;

    Board_create(&game->board);
    game->currentPiece=Piece_createRandom();
    game->nextPiece=Piece_createRandom();
    game->lastDropTime=SDL_GetTicks();
    game->dropInterval=500;
}

void lockPiece(Game* game)
{
    for (int r=0; r<PIECE_GRID_SIZE; r++)
    {
        for (int c=0; c<PIECE_GRID_SIZE; c++)
        {
            if (Piece_getBlock(game->currentPiece.type, game->currentPiece.rotation, r, c)!=0)
            {
                int boardRow=game->currentPiece.row + r;
                int boardCol=game->currentPiece.col + c;

                if (boardRow>=0 && boardRow<BOARD_ROWS && boardCol>=0 && boardCol<BOARD_COLS)
                {
                    //guardamos el tipo de pieza (o un 1) para indicar que esta celda ahora esta ocupada
                    game->board.cells[boardRow][boardCol]=game->currentPiece.type + 1;
                }
            }
        }
    }
}

void Game_update(Game* game)
{
if (!game || !game->isRunning || game->isGameOver) return;

    Uint32 currentTime=SDL_GetTicks();

    //verificamos si paso el tiempo configurado para hacer bajar la pieza
    if (currentTime - game->lastDropTime >= game->dropInterval)
    {

        //intentamos mover la pieza una celda hacia abajo
        if (Board_isValidPosition(&game->board, &game->currentPiece, game->currentPiece.col, game->currentPiece.row + 1))
        {
            game->currentPiece.row++;
        }
        else
        {
            //si no puede bajar mas, la estampamos en el tablero
            lockPiece(game);

            //limpiar linea
            int cleared=Board_clearLines(&game->board);

            if (cleared>0)
            {
            //sumar puntaje
            game->board.score+=cleared*100;
            SDL_Log("Lineas limpiadas: %d | Puntaje: %d", cleared, game->board.score);
            }

            //la pieza siguiente pasa a ser la activa, y creamos una nueva para la vista previa
            game->currentPiece=game->nextPiece;
            game->nextPiece=Piece_createRandom();

            //verificamos si la nueva pieza ya nace colisionando (game over)
            if (!Board_isValidPosition(&game->board, &game->currentPiece, game->currentPiece.col, game->currentPiece.row))
            {
                game->isGameOver=true;
                SDL_Log("Game Over!");
            }
        }

        //actualizamos el reloj de la ultima caida
        game->lastDropTime=currentTime;
    }
}

static void drawText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color)
{
    if (!font || !text) return;

    //crear una superficie para el texto
    SDL_Surface* surface=TTF_RenderText_Blended(font, text, color);
    if (!surface) return;

    //convertir la superficie a textura renderizada
    SDL_Texture* texture=SDL_CreateTextureFromSurface(renderer, surface);
    if (texture) {
        SDL_Rect destRect = { x, y, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
        SDL_DestroyTexture(texture);
    }

    SDL_FreeSurface(surface);
}

void Game_render(Game* game)
{
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);

    //dibujar el fondo del tablero (Gris oscuro para saber donde esta el area de juego)
    SDL_Rect boardBackground = {
        BOARD_OFFSET_X,
        BOARD_OFFSET_Y,
        BOARD_COLS * BOARD_CELLS_SIZE,
        BOARD_ROWS * BOARD_CELLS_SIZE
    };
    SDL_SetRenderDrawColor(game->renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(game->renderer, &boardBackground);

    //dibujar bloques fijos del tablero (cells)
    for (int r=0; r<BOARD_ROWS; r++) 
    {
        for (int c=0; c<BOARD_COLS; c++) 
        {
            int cellValue=game->board.cells[r][c];
            if (cellValue!=0) 
            {
                //obtener el color original del tipo de pieza guardado
                PieceType type=(PieceType)(cellValue-1);
                SDL_Color color=Piece_getColor(type);

                SDL_SetRenderDrawColor(game->renderer, color.r, color.g, color.b, color.a);

                SDL_Rect blockRect = 
                {
                    BOARD_OFFSET_X+(c*BOARD_CELLS_SIZE),
                    BOARD_OFFSET_Y+(r*BOARD_CELLS_SIZE),
                    BOARD_CELLS_SIZE-1,
                    BOARD_CELLS_SIZE-1
                };
                SDL_RenderFillRect(game->renderer, &blockRect);
            }
        }
    }

    //dibujar la pieza actual (currentPiece)
    SDL_Color currentColor=Piece_getColor(game->currentPiece.type);
    SDL_SetRenderDrawColor(game->renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    for (int r=0; r<PIECE_GRID_SIZE; r++)
    {
        for (int c=0; c<PIECE_GRID_SIZE; c++)
        {
            if (Piece_getBlock(game->currentPiece.type, game->currentPiece.rotation, r, c)!=0)
            {
                int boardRow=game->currentPiece.row + r;
                int boardCol=game->currentPiece.col + c;

                //solo dibujamos si esta dentro de la parte visible de la pantalla
                if (boardRow>=0)
                {
                    SDL_Rect pieceBlock =
                    {
                        BOARD_OFFSET_X + (boardCol * BOARD_CELLS_SIZE),
                        BOARD_OFFSET_Y + (boardRow * BOARD_CELLS_SIZE),
                        BOARD_CELLS_SIZE - 1,
                        BOARD_CELLS_SIZE - 1
                    };
                    SDL_RenderFillRect(game->renderer, &pieceBlock);
                }
            }
        }
    }

    //panel que muestra la siguiente pieza
    int nextPanelX=260;
    int nextPanelY=20;
    int nextPanelW=120;
    int nextPanelH=100;

    //fondo del panel secundario (gris oscuro)
    SDL_Rect nextPanelRect = { nextPanelX, nextPanelY, nextPanelW, nextPanelH };
    SDL_SetRenderDrawColor(game->renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(game->renderer, &nextPanelRect);

    //borde blanco
    SDL_SetRenderDrawColor(game->renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(game->renderer, &nextPanelRect);

    //dibujar los bloques de game->nextPiece dentro del panel
    SDL_Color nextColor=Piece_getColor(game->nextPiece.type);
    SDL_SetRenderDrawColor(game->renderer, nextColor.r, nextColor.g, nextColor.b, nextColor.a);

    //offset interno para centrar la pieza 4x4 dentro del panel de 120px
    int offsetX=nextPanelX + 10;
    int offsetY=nextPanelY + 10;

    for (int r=0; r<PIECE_GRID_SIZE; r++)
    {
        for (int c=0; c<PIECE_GRID_SIZE; c++)
        {
            //leemos la rotacion inicial (0) de nextPiece
            if (Piece_getBlock(game->nextPiece.type, 0, r, c)!=0)
            {
                SDL_Rect blockRect = 
                {
                    offsetX+(c*BOARD_CELLS_SIZE),
                    offsetY+(r*BOARD_CELLS_SIZE),
                    BOARD_CELLS_SIZE-1,
                    BOARD_CELLS_SIZE-1
                };
                SDL_RenderFillRect(game->renderer, &blockRect);
            }
        }
    }

    SDL_Color textColor = { 255, 255, 255, 255 };
    char buffer[64];

    //dibujar score
    snprintf(buffer, sizeof(buffer), "SCORE: %d", game->board.score);
    drawText(game->renderer, game->font, buffer, 260, 140, textColor);

    //dibujar lineas
    snprintf(buffer, sizeof(buffer), "LINES: %d", game->board.linesCleared);
    drawText(game->renderer, game->font, buffer, 260, 170, textColor);

    SDL_Color titleColor  = { 240, 240,   0, 255 }; // Amarillo para el título
    SDL_Color labelColor  = { 180, 180, 180, 255 }; // Gris claro para los controles

    drawText(game->renderer, game->font, "CONTROLES:", 260, 220, titleColor);

    //lista de controles
    drawText(game->renderer, game->font, "Izq/Der : Mover",    260, 255, labelColor);
    drawText(game->renderer, game->font, "Arriba  : Rotar",    260, 280, labelColor);
    drawText(game->renderer, game->font, "Abajo   : Caida",    260, 305, labelColor);
    drawText(game->renderer, game->font, "R       : Reiniciar", 260, 330, labelColor);
    drawText(game->renderer, game->font, "ESC     : Salir",     260, 355, labelColor);

    if (game->isGameOver)
    {
        //habilitar modo de mezcla para transparencia
        SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);

        //rectangulo negro semitransparente que cubre toda la ventana
        SDL_Rect overlay = { 0, 0, WINDOW_W, WINDOW_H };
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 200);
        SDL_RenderFillRect(game->renderer, &overlay);

        SDL_Color redColor = { 255, 50, 50, 255 };
        drawText(game->renderer, game->font, "GAME OVER", 180, 260, redColor);

        SDL_Color whiteColor = { 255, 255, 255, 255 };
        drawText(game->renderer, game->font, "Presiona 'R' para reiniciar", 130, 300, whiteColor);
    }

    SDL_RenderPresent(game->renderer);
}

void Game_handleEvents(Game* game)
{
    if (!game) return;

            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                //cierre de ventana (clic en la X de la ventana)
                if (event.type==SDL_QUIT)
                {
                    game->isRunning=false;
                }
                // Eventos de teclado
                else if (event.type==SDL_KEYDOWN)
                {
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            game->isRunning=false;
                            break;

                        case SDLK_LEFT:
                        {
                            //intentamos mover a la izquierda (columna - 1)
                            if (Board_isValidPosition(&game->board, &game->currentPiece, game->currentPiece.col - 1, game->currentPiece.row))
                            {
                                game->currentPiece.col--;
                            }
                            break;
                        }

                        case SDLK_RIGHT:
                        {
                            //intentamos mover a la derecha (columna + 1)
                            if (Board_isValidPosition(&game->board, &game->currentPiece, game->currentPiece.col + 1,game->currentPiece.row))
                            {
                                game->currentPiece.col++;
                            }
                            break;
                        }

                        case SDLK_UP:
                        {
                            //rotar la pieza: probamos la siguiente rotacion (0 a 3)
                            int nextRotation=(game->currentPiece.rotation+1) % 4;
                            int oldRotation=game->currentPiece.rotation;

                            //aplicamos temporalmente la rotacion para validar
                            game->currentPiece.rotation=nextRotation;

                            //si no es una posicion valida (p ej: choca contra la pared al girar)
                            if (!Board_isValidPosition(&game->board, &game->currentPiece, game->currentPiece.col, game->currentPiece.row))
                            {
                                //volvemos a la rotacion anterior
                                game->currentPiece.rotation=oldRotation;
                            }
                            break;
                        }

                        case SDLK_DOWN:
                        {
                            //caida rapida: mover una celda hacia abajo si se puede
                            if (Board_isValidPosition(&game->board, &game->currentPiece, game->currentPiece.col, game->currentPiece.row + 1))
                            {
                                game->currentPiece.row++;
                            }
                            break;
                        }
                        case SDLK_r:
                        {
                            Game_reset(game);
                            break;
                        }
                    }
                }
            }
}

void Game_clean(Game* game)
{

    if (game->font)
    {
        TTF_CloseFont(game->font);
        game->font=NULL;
    }
    TTF_Quit();
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
    free(game);
}

bool Game_isRunning(Game* game)
{
  return game->isRunning;
}
