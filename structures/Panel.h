#ifndef PANEL_H_INCLUDED
#define PANEL_H_INCLUDED

#include <SDL2/SDL.h>
#include "Piece.h"

#define BOARD_OFFSET_X 20
#define BOARD_OFFSET_Y 20

#define BOARD_ROWS 20
#define BOARD_COLS 10
#define BOARD_CELLS_SIZE 30

#define PANEL_X       340    /* Origen X del panel (px) */
#define PANEL_W       140    /* Ancho del panel (px)    */
#define PANEL_CELL     22    /* Celda del preview "NEXT" */
#define PIXEL_SIZE      3    /* Tamaño de cada pixel del font (px) */
#define PIXEL_SIZE_BIG  6    /* Tamaño grande para texto de game over */

typedef struct { char ch; Uint8 rows[5]; } Glyph;

void Panel_drawString(SDL_Renderer* r, const char* str, int x, int y, int ps, SDL_Color c);

void Panel_drawChar(SDL_Renderer* r, char ch, int x, int y, int ps, SDL_Color c);

void Panel_drawNumber(SDL_Renderer* r, int n, int x, int y, int ps, SDL_Color c);

int Panel_textWidth(const char* str, int ps);

void Panel_gameOver(SDL_Renderer* r);

void Panel_render(SDL_Renderer* r, const Piece* nextPiece, int score, int level, int linesCleared);

#endif // PANEL_H_INCLUDED
