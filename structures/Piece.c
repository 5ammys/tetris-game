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
        p.row = -1;
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

Piece Piece_createRandom(Vector* v) {
    Piece p;
    int index_random = rand() % PIECE_COUNT;

    //se copia la pieza del catalogo a la variable local 'p'
    Vector_getByPos(v, index_random, &p, sizeof(Piece));

    return p;
}

void Piece_rotate(Piece* p) {
    if (!p) return;

    int temp[PIECE_GRID_SIZE][PIECE_GRID_SIZE];

    for (int i = 0; i < PIECE_GRID_SIZE; i++) {
        for (int j = 0; j < PIECE_GRID_SIZE; j++) {
            //rotacion de 90 grados en sentido horario
            temp[i][j] = p->shape[PIECE_GRID_SIZE - 1 - j][i];
        }
    }

    memcpy(p->shape, temp, sizeof(temp));
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
        case PIECE_I: return (SDL_Color){0, 255, 255, 255}; 
        case PIECE_T: return (SDL_Color){255, 102, 204, 255};
        case PIECE_O: return (SDL_Color){255, 255, 0, 255}; 
        case PIECE_S: return (SDL_Color){0, 255, 0, 255};   
        case PIECE_Z: return (SDL_Color){255, 0, 0, 255};   
        case PIECE_J: return (SDL_Color){0, 0, 255, 255};   
        case PIECE_L: return (SDL_Color){255, 165, 0, 255}; 
        default:      return (SDL_Color){255, 255, 255, 255};
    }
}

void Piece_drawEx(const Piece *p, SDL_Renderer *renderer, int offsetX, int offsetY, int cellSize, bool useGridPosition) {
    if (!p || !renderer) return;

    SDL_Color color = Piece_getColor(p->type);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    for (int r = 0; r < PIECE_GRID_SIZE; r++) {
        for (int c = 0; c < PIECE_GRID_SIZE; c++) {
            if (p->shape[r][c]) {
                int colOffset = useGridPosition ? (p->col + c) : c;
                int rowOffset = useGridPosition ? (p->row + r) : r;

                int px = offsetX + colOffset * cellSize;
                int py = offsetY + rowOffset * cellSize;

                //se mantiene el margen interno de 1px para diferenciar los bloques
                SDL_Rect rect = {
                    .x = px + 1,
                    .y = py + 1,
                    .w = cellSize - 2,
                    .h = cellSize - 2
                };

                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

//wrapper para dibujar la pieza activa en el tablero
void Piece_draw(const Piece *p, SDL_Renderer *renderer, int boardOffsetX, int boardOffsetY, int cellSize) {
    Piece_drawEx(p, renderer, boardOffsetX, boardOffsetY, cellSize, true);
}

//wrapper para dibujar la pieza en UI / Next Piece / Hold
void Piece_drawAt(const Piece *p, SDL_Renderer *renderer, int screenX, int screenY, int cellSize) {
    Piece_drawEx(p, renderer, screenX, screenY, cellSize, false);
}
