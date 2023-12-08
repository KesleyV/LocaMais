// decorador.h
#ifndef DECORADOR_H
#define DECORADOR_H

enum corLetra
{
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE

};

enum corFundo
{
    _BLACK = 0,
    _BLUE = 16,
    _GREEN = 32,
    _CYAN = 48,
    _RED = 64,
    _MAGENTA = 80,
    _BROWN = 96,
    _LIGHTGRAY = 112,
    _DARKGRAY = 128,
    _LIGHTBLUE = 144,
    _LIGHTGREEN = 160,
    _LIGHTCYAN = 176,
    _LIGHTRED = 192,
    _LIGHTMAGENTA = 208,
    _YELLOW = 224,
    _WHITE = 240
};

void linhaCol(int lin, int col);
void box(int lin1, int col1, int lin2, int col2);
int estiliza_menu(int lin1, int col1, int qtd, char lista[3][40]);
void textColor(int letras, int fundo);
void drawLine(int start, int end, int width);
void limpar_console();
void exibir_mensagem(int lin, int col, const char *mensagem, const char *parametro);

#endif