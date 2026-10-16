#include <stdlib.h>
#include <string.h>
#include "Board.h"

void Board_create(Board *b){
    memset(b->cells, 0, sizeof(b->cells));
    b->level        = 1;
    b->score        = 0;
    b->linesCleared = 0;
}

bool Board_isValidPosition(Board *b, Piece *piece, int x, int y){
    int shape[PIECE_GRID_SIZE][PIECE_GRID_SIZE];

    for(int r = 0; r < PIECE_GRID_SIZE; r++){
        for(int c = 0; c < PIECE_GRID_SIZE; c++){
            if(shape[r][c] != 0){
                int newRow = y + r;
                int newCol = x + c;

                // Verificar límites horizontales
                if(newCol < 0 || newCol >= BOARD_COLS) return false;

                // Verificar límite inferior
                if(newRow >= BOARD_ROWS) return false;

                // Verificar colisión con piezas bloqueadas
                if(newRow >= 0 && b->cells[newRow][newCol] != 0) return false;
            }
        }
    }
    return true;
}

int Board_getDropInterval(Board* b){
    int level = b->level;
    if(level >= 10) return 80;
    return 1000 - (level - 1) * 92;
}