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



#endif // BOARD_H_INCLUDED
