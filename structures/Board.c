#include <stdlib.h>
#include <string.h>
#include "Board.h"

void Board_create(Board *b){
    memset(b->cells, 0, sizeof(b->cells));
    b->level        = 1;
    b->score        = 0;
    b->linesCleared = 0;
}

bool Board_isValidPosition(const Board *b, const Piece *piece, int row, int col){
    for(int r = 0; r < PIECE_GRID_SIZE; r++){
        for(int c = 0; c < PIECE_GRID_SIZE; c++){
            if(piece->shape[r][c]){
                int newRow = row + r;
                int newCol = col + c;

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

void Board_draw(const Board* b, SDL_Renderer* renderer,
                int offsetX, int offsetY, int cellSize) {
    int r, c;

    for(r = 0; r < BOARD_ROWS; r++) {
        for(c = 0; c < BOARD_COLS; c++) {
            int colorIdx = b->cells[r][c];
            SDL_Color color;
            if(colorIdx > 0 && colorIdx <= PIECE_COUNT) {
                color = Piece_getColor((PieceType)(colorIdx - 1));
            } else {
                color = (SDL_Color){ 18, 18, 30, 255 }; // 0 - vacío (fondo oscuro)
            }
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

            SDL_Rect rect = {
                offsetX + c * cellSize + 1,
                offsetY + r * cellSize + 1,
                cellSize - 2,
                cellSize - 2
            };
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void Board_drawGrid(SDL_Renderer* renderer, int offsetX, int offsetY, int cellSize) {
    // Líneas de la grilla en gris muy oscuro
    SDL_SetRenderDrawColor(renderer, 45, 45, 60, 255);

    int r, c;
    for(r = 0; r <= BOARD_ROWS; r++) {
        SDL_RenderDrawLine(renderer,
            offsetX,
            offsetY + r * cellSize,
            offsetX + BOARD_COLS * cellSize,
            offsetY + r * cellSize);
    }
    for(c = 0; c <= BOARD_COLS; c++) {
        SDL_RenderDrawLine(renderer,
            offsetX + c * cellSize,
            offsetY,
            offsetX + c * cellSize,
            offsetY + BOARD_ROWS * cellSize);
    }

    // Borde exterior más brillante
    SDL_SetRenderDrawColor(renderer, 120, 120, 160, 255);
    SDL_Rect border = {
        offsetX - 2,
        offsetY - 2,
        BOARD_COLS * cellSize + 4,
        BOARD_ROWS * cellSize + 4
    };
    SDL_RenderDrawRect(renderer, &border);
}

void Board_drawGhost(const Board *b, const Piece *p, SDL_Renderer *renderer, int offsetX, int offsetY, int cellSize) {
    if (!b || !p || !renderer) return;

    Piece ghost = *p;

    // Calcular la posición más baja posible
    while (Board_isValidPosition(b, &ghost, ghost.row + 1, ghost.col)) {
        ghost.row++;
    }

    // Si coincide con la posición de la pieza activa, no la dibujamos
    if (ghost.row == p->row) return;

    SDL_Color color = Piece_getColor(ghost.type);

    // Habilitar mezcla Alpha para la transparencia
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer,
        (Uint8)(color.r * 0.6f),
        (Uint8)(color.g * 0.6f),
        (Uint8)(color.b * 0.6f),
        255
    );

    for (int r = 0; r < PIECE_GRID_SIZE; r++) {
        for (int c = 0; c < PIECE_GRID_SIZE; c++) {
            if (ghost.shape[r][c]) {
                int px = offsetX + (ghost.col + c) * cellSize;
                int py = offsetY + (ghost.row + r) * cellSize;

                SDL_Rect rect = { px + 1, py + 1, cellSize - 2, cellSize - 2 };
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

int Board_clearLines(Board *b) {
    int linesCleared = 0;

    for(int r = BOARD_ROWS - 1; r >= 0; r--) {
        bool isFull = true;
        for(int c = 0; c < BOARD_COLS; c++) {
            if(b->cells[r][c] == 0) {
                isFull = false;
                c = BOARD_COLS; // Salir del bucle interno
            }
        }

        if(isFull) {
            linesCleared++;
            // Mover todas las filas superiores hacia abajo
            for(int rr = r; rr > 0; rr--) {
                memcpy(b->cells[rr], b->cells[rr - 1], sizeof(b->cells[rr]));
            }
            // Limpiar la fila superior
            memset(b->cells[0], 0, sizeof(b->cells[0]));
            // Re-evaluar la misma fila ya que se ha movido hacia abajo
            r++;
        }
    }

    return linesCleared;
}

void Board_updateScore(Board *b, int linesCleared) {
    b->linesCleared += linesCleared;

    // Actualizar nivel cada 10 líneas
    int newLevel = (b->linesCleared / 10) + 1;
    if(newLevel > b->level) {
        b->level = newLevel;
    }

    // Puntos según número de líneas eliminadas
    int points = 0;
    switch(linesCleared) {
        case 1: points = 100; break;
        case 2: points = 300; break;
        case 3: points = 500; break;
        case 4: points = 800; break;
        default: points = linesCleared * 200; break; // Bonus por más de 4 líneas
    }
    b->score += points * b->level; // Multiplicador por nivel
}

void Board_lockPiece(Board *b, const Piece *p) {
    for(int r = 0; r < PIECE_GRID_SIZE; r++) {
        for(int c = 0; c < PIECE_GRID_SIZE; c++) {
            if(p->shape[r][c]) {
                int boardRow = p->row + r;
                int boardCol = p->col + c;
                if(boardRow >= 0 && boardRow < BOARD_ROWS &&
                   boardCol >= 0 && boardCol < BOARD_COLS) {
                    b->cells[boardRow][boardCol] = p->type + 1;
                }
            }
        }
    }
}
