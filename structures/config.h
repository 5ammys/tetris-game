#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define BOARD_OFFSET_X 20
#define BOARD_OFFSET_Y 20

#define BOARD_ROWS 20
#define BOARD_COLS 10
#define BOARD_CELLS_SIZE 30

#define PANEL_X       340    /* Origen X del panel (px) */
#define PANEL_W       140    /* Ancho del panel (px)    */
#define PANEL_CELL     22    /* Celda del preview NEXT */
#define PIXEL_SIZE      3    /* Tamaño de cada pixel del font (px) */
#define PIXEL_SIZE_BIG  6 

#define WINDOW_W 500
#define WINDOW_H 640

#define FPS        60
#define DELAY_TIME (1000.0f / FPS)


#endif // CONFIG_H_INCLUDED
