#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <locale.h>
#include <stdbool.h>
#include "Decorador.h"

void limpar_console()
{
    system("cls");
    textColor(YELLOW, _BLACK);
}

void textColor(int letra, int fundo)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), letra + fundo);
}

void linhaCol(int lin, int col)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){col - 1, lin - 1}); // coorddenada na tela

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void box(int lin1, int col1, int lin2, int col2)
{
    int i, j, tamlin, tamcol;

    tamlin = lin2 - lin1;
    tamcol = col2 - col1;


    for (i = col1; i <= col2; i++)
    { 
        linhaCol(lin1, i);
        printf("%c", 196);
        linhaCol(lin2, i);
        printf("%c", 196);
    }

    for (i = lin1; i <= lin2; i++)
    { 
        linhaCol(i, col1);
        printf("%c", 179);
        linhaCol(i, col2);
        printf("%c", 179);
    }
    for (i = lin1 + 1; i < lin2; i++)
    {
        for (j = col1 + 1; j < col2; j++)
        {
            linhaCol(i, j);
            printf(" ");
        }
    }
    
    linhaCol(lin1, col1);
    printf("%c", 218);
    linhaCol(lin1, col2);
    printf("%c", 191);
    linhaCol(lin2, col1);
    printf("%c", 192);
    linhaCol(lin2, col2);
    printf("%c", 217);
}

int estiliza_menu(int qtd, char lista[qtd][40])
{
    int opc = 1, lin1, col1, lin2, col2, linha, i, tamMaxItem, tecla;

    tamMaxItem = strlen(lista[0]);
    for (i = 1; i < qtd; i++)
    {
        if (strlen(lista[i]) > tamMaxItem)
        {
            tamMaxItem = strlen(lista[i]);
        }
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int screenHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    lin1 = (screenHeight - (qtd * 2 + 2)) / 2;
    col1 = (screenWidth - (tamMaxItem + 4)) / 2;
    lin2 = lin1 + (qtd * 2 + 2);
    col2 = col1 + tamMaxItem + 4;

    textColor(YELLOW, _BLACK);
    setlocale(LC_ALL, "C");
    box(lin1, col1, lin2, col2);
    setlocale(LC_ALL, "");

    while (1)
    {
        linha = lin1 + 2;
        for (i = 0; i < qtd; i++)
        {
            if (i + 1 == opc)
                textColor(BLACK, _YELLOW);
            else
                textColor(YELLOW, _BLACK);
            linhaCol(linha, col1 + 2);
            printf("%s", lista[i]);
            linha += 2;
        }

        linhaCol(1, 1);
        tecla = getch();
        linhaCol(22, 1);

        if (tecla == 27)
        {
            opc = 0;
            break;
        }
        else if (tecla == 13)
        {
            break;
        }
        else if (tecla == 72)
        {
            if (opc > 1)
                opc--;
        }
        else if (tecla == 80)
        {
            if (opc < qtd)
                opc++;
        }
    }
    return opc;
}

void drawLine(int start, int end, int width) {
    setlocale(LC_ALL, "C");

    for (int w = 0; w < width; w++) {
        for (int i = start; i <= end; i++) {
            printf("%c", 196);
        }
    }
    setlocale(LC_ALL, "");
}

void setFullScreen() {
    HWND hwnd = GetConsoleWindow();

    MONITORINFO mi;
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &mi);

    SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, 
                 mi.rcMonitor.right - mi.rcMonitor.left, 
                 mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_SHOWWINDOW);
}

void manualPosition(int qtd, char lista[qtd][100]) {
    int lin1, col1, lin2, col2, linha, i, tamMaxItem;

    tamMaxItem = strlen(lista[0]);
    for (i = 1; i < qtd; i++) {
        if (strlen(lista[i]) > tamMaxItem) {
            tamMaxItem = strlen(lista[i]);
        }
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int screenHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    // Ajuste a margem superior para controlar a posição vertical
    int margemSuperior = 5; // Ajuste este valor conforme necessário

    lin1 = margemSuperior;
    col1 = (screenWidth - (tamMaxItem + 4)) / 2;
    lin2 = lin1 + (qtd * 2 + 2);
    col2 = col1 + tamMaxItem + 4;

    textColor(YELLOW, 0);
    setlocale(LC_ALL, "C");
    box(lin1, col1, lin2, col2);
    setlocale(LC_ALL, "");

    linha = lin1 + 2;
    for (i = 0; i < qtd; i++) {
        textColor(YELLOW, 0);
        linhaCol(linha, col1 + 2);
        printf("%s", lista[i]);
        linha += 2;
    }
}
