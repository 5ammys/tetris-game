#ifndef PANEL_H_INCLUDED
#define PANEL_H_INCLUDED

#include <SDL2/SDL.h>
#include "Piece.h"
#include "config.h"


typedef struct { char ch; Uint8 rows[5]; } CharPixel;

void Panel_drawString(SDL_Renderer* r, const char* str, int x, int y, int ps, SDL_Color c);

void Panel_drawChar(SDL_Renderer* r, char ch, int x, int y, int ps, SDL_Color c);

void Panel_drawNumber(SDL_Renderer* r, int n, int x, int y, int ps, SDL_Color c);

int Panel_textWidth(const char* str, int ps);

void Panel_gameOver(SDL_Renderer* r);

void Panel_render(SDL_Renderer* r, const Piece* nextPiece, int score, int level, int linesCleared);

#endif // PANEL_H_INCLUDED
