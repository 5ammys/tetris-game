#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED

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

#endif // PIECE_H_INCLUDED
