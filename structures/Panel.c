#include "Panel.h"

void Panel_drawChar(SDL_Renderer* r, char ch, int x, int y, int ps, SDL_Color c) {
    int i, row, col;
    Uint8 pattern[5] = {0};

    Glyph FONT[] = {
      /* Dígitos */
      {'0', {7,5,5,5,7}},
      {'1', {2,6,2,2,7}},
      {'2', {7,1,7,4,7}},
      {'3', {7,1,7,1,7}},
      {'4', {5,5,7,1,1}},
      {'5', {7,4,7,1,7}},
      {'6', {7,4,7,5,7}},
      {'7', {7,1,1,1,1}},
      {'8', {7,5,7,5,7}},
      {'9', {7,5,7,1,7}},
      /* Letras (mayúsculas) */
      {'A', {2,5,7,5,5}},
      {'C', {7,4,4,4,7}},
      {'E', {7,4,6,4,7}},
      {'G', {7,4,6,5,7}},
      {'I', {7,2,2,2,7}},
      {'L', {4,4,4,4,7}},
      {'M', {5,7,5,5,5}},
      {'N', {5,5,7,5,5}},
      {'O', {7,5,5,5,7}},
      {'R', {6,5,6,5,5}},
      {'S', {7,4,7,1,7}},
      {'T', {7,2,2,2,2}},
      {'V', {5,5,5,2,2}},
      {'X', {5,5,2,5,5}},
      {' ', {0,0,0,0,0}},
    };

    int FONT_SIZE = sizeof(FONT) / sizeof(Glyph);

    for(i = 0; i < FONT_SIZE; i++) {
        if(FONT[i].ch == ch) { memcpy(pattern, FONT[i].rows, 5); break; }
    }
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
    for(row = 0; row < 5; row++) {
        for(col = 0; col < 3; col++) {
            if(pattern[row] & (4 >> col)) {
                SDL_Rect rect = {x + col*ps, y + row*ps, ps-1, ps-1};
                SDL_RenderFillRect(r, &rect);
            }
        }
    }
}

/* Dibuja una cadena de texto */
void Panel_drawString(SDL_Renderer* r, const char* str, int x, int y, int ps, SDL_Color c) {
    int i;
    int cw = 3*ps + 2;  /* ancho de carácter + espacio */
    for(i = 0; str[i] != '\0'; i++) {
        Panel_drawChar(r, str[i], x + i*cw, y, ps, c);
    }
}

/* Dibuja un número entero */
void Panel_drawNumber(SDL_Renderer* r, int n, int x, int y, int ps, SDL_Color c) {
    char buf[16];
    sprintf(buf, "%d", n);
    Panel_drawString(r, buf, x, y, ps, c);
}

/* Retorna el ancho en píxeles de una cadena con pixelSize ps */
int Panel_textWidth(const char* str, int ps) {
    return (int)strlen(str) * (3*ps + 2);
}

void Panel_render(SDL_Renderer* r, const Piece* nextPiece, int score, int level, int linesCleared) {
    int px = PANEL_X;
    int py = BOARD_OFFSET_Y;

    /* Fondo del panel */
    SDL_Rect panelBg = {px - 8, py, PANEL_W + 8, BOARD_ROWS * BOARD_CELLS_SIZE};
    SDL_SetRenderDrawColor(r, 18, 18, 35, 255);
    SDL_RenderFillRect(r, &panelBg);
    SDL_SetRenderDrawColor(r, 60, 60, 90, 255);
    SDL_RenderDrawRect(r, &panelBg);

    SDL_Color white  = {225, 225, 235, 255};
    SDL_Color gold   = {240, 190,  20, 255};
    SDL_Color cyan   = {  0, 200, 210, 255};
    SDL_Color green  = { 60, 210,  80, 255};
    SDL_Color purple = {180,  60, 220, 255};
    SDL_Color dark   = { 12,  12,  24, 255};

    int cur_y = py + 8;

    /* ---- NEXT ---- */
    Panel_drawString(r, "NEXT", px + 4, cur_y, PIXEL_SIZE, gold);
    cur_y += 5*PIXEL_SIZE + 6;  /* altura del texto + margen */

    /* Fondo del preview */
    int previewW = 4 * PANEL_CELL;
    int previewH = 4 * PANEL_CELL;
    int previewX = px + (PANEL_W - previewW) / 2;
    SDL_Rect previewBg = {previewX - 2, cur_y - 2, previewW + 4, previewH + 4};
    SDL_SetRenderDrawColor(r, dark.r, dark.g, dark.b, 255);
    SDL_RenderFillRect(r, &previewBg);
    SDL_SetRenderDrawColor(r, 55, 55, 80, 255);
    SDL_RenderDrawRect(r, &previewBg);
    Piece_drawAt(nextPiece, r, previewX, cur_y, PANEL_CELL);
    cur_y += previewH + 18;

    /* Línea separadora */
    SDL_SetRenderDrawColor(r, 50, 50, 75, 255);
    SDL_RenderDrawLine(r, px, cur_y, px + PANEL_W, cur_y);
    cur_y += 10;

    /* ---- SCORE ---- */
    Panel_drawString(r, "SCORE", px + 4, cur_y, PIXEL_SIZE, cyan);
    cur_y += 5*PIXEL_SIZE + 5;
    Panel_drawNumber(r, score, px + 6, cur_y, PIXEL_SIZE + 1, white);
    cur_y += 5*(PIXEL_SIZE+1) + 16;

    /* ---- LEVEL ---- */
    Panel_drawString(r, "LEVEL", px + 4, cur_y, PIXEL_SIZE, green);
    cur_y += 5*PIXEL_SIZE + 5;
    Panel_drawNumber(r, level, px + 6, cur_y, PIXEL_SIZE + 1, white);
    cur_y += 5*(PIXEL_SIZE+1) + 16;

    /* ---- LINES ---- */
    Panel_drawString(r, "LINES", px + 4, cur_y, PIXEL_SIZE, purple);
    cur_y += 5*PIXEL_SIZE + 5;
    Panel_drawNumber(r, linesCleared, px + 6, cur_y, PIXEL_SIZE + 1, white);
    cur_y += 5*(PIXEL_SIZE+1) + 24;

    /* Línea separadora */
    SDL_SetRenderDrawColor(r, 50, 50, 75, 255);
    SDL_RenderDrawLine(r, px, cur_y, px + PANEL_W, cur_y);
    cur_y += 10;

    /* ---- Controles (indicadores de color) ---- */
    SDL_Color hint = {90, 90, 110, 255};
    Panel_drawString(r, "CONTROLS", px + 4, cur_y, 2, hint);
    cur_y += 5*2 + 8;

    SDL_Color hintVal = {130, 130, 150, 255};
    Panel_drawString(r, "ARROWS", px + 4, cur_y,  2, hintVal); cur_y += 5*2+5;
    Panel_drawString(r, "SPACE",  px + 4, cur_y,  2, hintVal); cur_y += 5*2+5;
    Panel_drawString(r, "R REST", px + 4, cur_y,  2, hintVal);
}

void Panel_gameOver(SDL_Renderer* r) {
    /* Overlay semitransparente */
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 10, 0, 0, 170);
    SDL_Rect overlay = {
        BOARD_OFFSET_X, BOARD_OFFSET_Y,
        BOARD_COLS * BOARD_CELLS_SIZE,
        BOARD_ROWS * BOARD_CELLS_SIZE
    };
    SDL_RenderFillRect(r, &overlay);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

    /* Texto "GAME" */
    int ps       = PIXEL_SIZE_BIG;
    int boardCX  = BOARD_OFFSET_X + (BOARD_COLS * BOARD_CELLS_SIZE) / 2;
    int boardCY  = BOARD_OFFSET_Y + (BOARD_ROWS * BOARD_CELLS_SIZE) / 2;
    SDL_Color red    = {230,  50,  50, 255};
    SDL_Color orange = {230, 150,  30, 255};

    int tw;
    tw = Panel_textWidth("GAME", ps);
    Panel_drawString(r, "GAME", boardCX - tw/2, boardCY - 5*ps - 4, ps, red);

    tw = Panel_textWidth("OVER", ps);
    Panel_drawString(r, "OVER", boardCX - tw/2, boardCY + 4, ps, red);

    /* Texto "R REST" */
    tw = Panel_textWidth("R REST", PIXEL_SIZE);
    Panel_drawString(r, "R REST", boardCX - tw/2,
               boardCY + 5*ps + 4 + 12, PIXEL_SIZE, orange);
}
