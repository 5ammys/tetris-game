#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED

#include "Vector.h"
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



#endif // PIECE_H_INCLUDED
