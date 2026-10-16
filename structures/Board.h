#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "Piece.h"

#define BOARD_ROWS 20
#define BOARD_COLS 10
#define BOARD_CELLS_SIZE 20

typedef struct{
  int cells[BOARD_ROWS][BOARD_COLS];
  int score;
  int level;
  int linesCleared;
}Board;

void Board_create(Board *b);

bool Board_isValidPosition(Board *b, Piece *piece, int x, int y);

int Board_getDropInterval(Board* b);

bool Board_lockPiece(Board *b, const Piece *piece);

int Board_clearLines(Board *b);

void Board_updateScore(Board *b, int linesCleared);

bool Board_isGameOver(const Board *b);

void Board_draw(const Board *b, SDL_Renderer *renderer, int offsetX, int offsetY, int cellSize);

void Board_drawGrid(SDL_Renderer *renderer, int offsetX, int offsetY, int cellSize);

#endif // BOARD_H_INCLUDED
