#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED

#include <SDL2/SDL.h>
#include "Vector.h"
#include "Board.h"
#define PIECE_GRID_SIZE 4

typedef enum{
    PIECE_I=0,
    PIECE_T,
    PIECE_O,
    PIECE_S,
    PIECE_Z,
    PIECE_J,
    PIECE_L,
    PIECE_COUNT
}PieceType;

typedef struct{
    int shape[PIECE_GRID_SIZE][PIECE_GRID_SIZE];
    PieceType type;
    int row;
    int col;
}Piece;

int Piece_createAndLoadPiecesInVector(Vector *v);

Piece Piece_createRandom(Vector* v);

void Piece_rotate(Piece* p);

void Piece_moveRow(Piece* p, int dRow);

void Piece_moveCol(Piece* p, int dCol);

void Piece_draw(const Piece* p, SDL_Renderer* renderer, int offsetX, int offsetY, int cellSize);

void Piece_drawAt(const Piece* p, SDL_Renderer* renderer, int screenX, int screenY, int cellSize);

SDL_Color Piece_getColor(PieceType type);

#endif // PIECE_H_INCLUDED
