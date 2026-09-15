#include <stdlib.h>
#include <string.h>
#include "Board.h"

void Board_create(Board *b){
    memset(b->cells, 0, sizeof(b->cells));
    b->level        = 1;
    b->score        = 0;
    b->linesCleared = 0;
}

bool Board_isValidPosition(Board *b, Piece *p, int x, int y){

}
