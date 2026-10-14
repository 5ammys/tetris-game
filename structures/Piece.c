#include <stdlib.h>
#include <string.h>
#include "Piece.h"

int Piece_createAndLoadPiecesInVector(Vector *v){
    if(!Vector_create(v, PIECE_COUNT)){
        printf("Error al crear el vector de piezas.\n");
        return 0;
    }

   const int pieces[PIECE_COUNT][PIECE_GRID_SIZE][PIECE_GRID_SIZE] = {
        [PIECE_I] = {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},
        [PIECE_T] = {{0,0,0,0}, {0,1,1,1}, {0,0,1,0}, {0,0,0,0}},
        [PIECE_O] = {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}},
        [PIECE_S] = {{0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0}},
        [PIECE_Z] = {{0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0}},
        [PIECE_J] = {{0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0}},
        [PIECE_L] = {{0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0}}
    };

    for(int i=0; i < PIECE_COUNT; i++){
        Piece p;
        p.type = (PieceType)i;
        p.row = 0;
        p.col = 3;
        memcpy(p.shape, pieces[i], sizeof(p.shape));
        if(!Vector_append(v, &p, sizeof(Piece))){
            printf("Error al insertar la pieza en el vector.\n");
            Vector_destroy(v);
            return 0;
        }
    }

    return 1;
}



void Piece_rotate(Piece* p){
    if(!p) return;

    int temp[PIECE_GRID_SIZE][PIECE_GRID_SIZE];

    for(int i = 0 ; i < PIECE_GRID_SIZE ; i++ ){
        for(int j = 0 ; j < PIECE_GRID_SIZE ; j++ ){
            temp[i][j] = p->shape[PIECE_GRID_SIZE - 1 - i][j];
        }
    }
    memcpy(p->shape, temp, sizeof(temp));
}

Piece Piece_createRandom(Vector* v) {
    Piece p;
    int index_random = rand() % PIECE_COUNT;

    // Se copia la pieza del catálogo a la variable local 'p'
    Vector_getByPos(v, index_random, &p, sizeof(Piece));

    return p;
}
