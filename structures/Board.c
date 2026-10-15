#include <stdlib.h>
#include <string.h>
#include "Board.h"
#include "Piece.h"

void Board_create(Board *b)
{
    memset(b->cells, 0, sizeof(b->cells));
    b->level=1;
    b->score=0;
    b->linesCleared=0;
}

bool Board_isValidPosition(Board *b, Piece *p, int x, int y)
{

    for (int r=0; r<PIECE_GRID_SIZE; r++) 
    {
            for (int c=0; c<PIECE_GRID_SIZE; c++) 
            {

                //consulta si en esa celda de la pieza hay un bloque solido
                if (Piece_getBlock(p->type, p->rotation, r, c)!=0) 
                {

                    int boardCol=x+c;
                    int boardRow=y+r;

                    //validar limites laterales (izquierdo y derecho)
                    if (boardCol<0 || boardCol>=BOARD_COLS) return false;

                    //validar el piso (borde inferior)
                    if (boardRow>=BOARD_ROWS) return false;

                    //vValidar colision con bloques fijos del tablero
                    // (solo si la fila ya ingreso a la parte visible del tablero, row >= 0)
                    if (boardRow>=0) 
                    {
                        if (b->cells[boardRow][boardCol]!=0)  return false;
                    }
                }
            }
    }
        
        //si paso todas las pruebas, la posicion es valida
        return true;
}

int Board_clearLines(Board *b) 
{
    int linesCleared=0;

    for (int r=BOARD_ROWS-1; r>=0; r--) 
    {
        bool isFull=true;

        //comprobar si la fila r esta completamente llena
        for (int c=0; c<BOARD_COLS; c++) 
        {
            if (b->cells[r][c]==0) 
            {
                isFull=false;
                break;
            }
        }

        //si la fila esta llena, la borramos desplazando las superiores
        if (isFull) 
        {
            linesCleared++;

            //mover todas las filas superiores una celda hacia abajo
            for (int y=r; y>0; y--) 
            {
                for (int c=0; c<BOARD_COLS; c++) 
                {
                    b->cells[y][c]=b->cells[y-1][c];
                }
            }

            //vaciar la fila superior (fila 0)
            for (int c=0; c<BOARD_COLS; c++) 
            {
                b->cells[0][c]=0;
            }

            //volver a evaluar la misma fila r, ya que ahora tiene los datos de la fila superior
            r++;
        }
    }

    //actualizar el acumulador de lineas limpiadas en el tablero
    b->linesCleared+=linesCleared;

    return linesCleared;
}
