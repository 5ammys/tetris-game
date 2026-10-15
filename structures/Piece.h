#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED
#include <SDL2/SDL.h>

#define PIECE_GRID_SIZE 4

typedef enum{
    PIECE_I=0,
    PIECE_T,
    PIECE_0,
    PIECE_S,
    PIECE_Z,
    PIECE_J,
    PIECE_L,
    PIECE_COUNT
}PieceType;

typedef struct{
    PieceType type;
    int rotation;
    int row;
    int col;
}Piece;

Piece Piece_create(PieceType type);
Piece Piece_createRandom();
int Piece_getBlock(PieceType type, int rotation, int r, int c);
SDL_Color Piece_getColor(PieceType type);

#endif // PIECE_H_INCLUDED
