#include <stdlib.h>
#include <string.h>
#include "Piece.h"
#include "Board.h"

int Piece_createAndLoadPiecesInVector(Vector *v){
    if(!Vector_create(v, PIECE_COUNT)){
        printf("Error al crear el vector de piezas.\n");
        return 0;
    }

   const int pieces[PIECE_COUNT][PIECE_GRID_SIZE][PIECE_GRID_SIZE] = {
        [PIECE_I] = {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},
        [PIECE_T] = {{0,0,0,0}, {0,2,2,2}, {0,0,2,0}, {0,0,0,0}},
        [PIECE_O] = {{0,0,0,0}, {0,3,3,0}, {0,3,3,0}, {0,0,0,0}},
        [PIECE_S] = {{0,0,0,0}, {0,4,4,0}, {4,4,0,0}, {0,0,0,0}},
        [PIECE_Z] = {{0,0,0,0}, {5,5,0,0}, {0,5,5,0}, {0,0,0,0}},
        [PIECE_J] = {{0,0,0,0}, {6,6,6,0}, {0,0,6,0}, {0,0,0,0}},
        [PIECE_L] = {{0,0,0,0}, {7,7,7,0}, {7,0,0,0}, {0,0,0,0}}
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

void Piece_moveRow(Piece* p, int dRow){
    if(!p) return;
    p->row += dRow;
}

void Piece_moveCol(Piece* p, int dCol){
    if(!p) return;
    p->col += dCol;
}

SDL_Color Piece_getColor(PieceType type) {
    switch(type) {
        case PIECE_I: return (SDL_Color){0, 255, 255, 255}; // Cyan
        case PIECE_T: return (SDL_Color){128, 0, 128, 255}; // Purple
        case PIECE_O: return (SDL_Color){255, 255, 0, 255}; // Yellow
        case PIECE_S: return (SDL_Color){0, 255, 0, 255};   // Green
        case PIECE_Z: return (SDL_Color){255, 0, 0, 255};   // Red
        case PIECE_J: return (SDL_Color){0, 0, 255, 255};   // Blue
        case PIECE_L: return (SDL_Color){255, 165, 0, 255}; // Orange
        default:      return (SDL_Color){255, 255, 255, 255}; // White for unknown types
    }
}

void Piece_draw(Piece *p, SDL_Renderer *renderer, int offsetX, int offsetY, int cellSize) {
    if (!p || !renderer) return;

    SDL_Color color = Piece_getColor(p->type);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Si la posición de la pieza es inválida (ej. -1), se dibuja de forma relativa al offset
    bool usePiecePosition = (p->row < BOARD_ROWS && p->col < BOARD_COLS && p->row >= 0 && p->col >= 0);

    for (int r = 0; r < PIECE_GRID_SIZE; r++) {
        for (int c = 0; c < PIECE_GRID_SIZE; c++) {
            if (p->shape[r][c]) {
                int px = offsetX + (usePiecePosition ? (p->col + c) : c) * cellSize;
                int py = offsetY + (usePiecePosition ? (p->row + r) : r) * cellSize;

                SDL_Rect rect = { px, py, cellSize, cellSize };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}