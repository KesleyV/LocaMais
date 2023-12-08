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

    // funcao para deixar o cursor invisivel
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void box(int lin1, int col1, int lin2, int col2)
{
    int i, j, tamlin, tamcol;

    // achar o tamanho do box
    tamlin = lin2 - lin1;
    tamcol = col2 - col1;

    // Monta o Box

    for (i = col1; i <= col2; i++)
    { // linhas
        linhaCol(lin1, i);
        printf("%c", 196);
        linhaCol(lin2, i);
        printf("%c", 196);
    }

    for (i = lin1; i <= lin2; i++)
    { // colunas
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
    // Posição dos cantos
    linhaCol(lin1, col1);
    printf("%c", 218);
    linhaCol(lin1, col2);
    printf("%c", 191);
    linhaCol(lin2, col1);
    printf("%c", 192);
    linhaCol(lin2, col2);
    printf("%c", 217);
}

int estiliza_menu(int lin1, int col1, int qtd, char lista[3][40])
{
    int opc = 1, lin2, col2, linha, i, tamMaxItem, tecla;

    // calcula as coordenadas
    tamMaxItem = strlen(lista[0]);
    // tamanho maximo do item
    for (i = 1; i < qtd; i++)
    {
        if (strlen(lista[i]) > tamMaxItem)
        {
            tamMaxItem = strlen(lista[i]);
        }
    }
    lin2 = lin1 + (qtd * 2 + 2);  // Aumente o multiplicador para aumentar a altura da caixa
    col2 = col1 + tamMaxItem + 4; // Ajuste conforme necessário

    // Monta Tela
    textColor(YELLOW, _BLACK);
    setlocale(LC_ALL, "C");
    box(lin1, col1, lin2, col2);
    setlocale(LC_ALL, "");
    // laço das opcões
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

        // Aguarda tecla
        linhaCol(1, 1);
        tecla = getch();
        linhaCol(22, 1);
        // Opção
        if (tecla == 27)
        { // ESC
            opc = 0;
            break;
        }
        else if (tecla == 13)
        { // ENTER
            break;
        }
        // Seta para cima
        else if (tecla == 72)
        { // se possivel seleciona o item anterior - seta para cima
            if (opc > 1)
                opc--; // se opcao for maior que 1, pode voltar
        }
        else if (tecla == 80)
        { // seta para baixo
            if (opc < qtd)
                opc++; // Se opcao for menor que quantidade de itens, posso avançar
        }
    }
    return opc;
}

void exibir_mensagem(int lin, int col, const char *mensagem, const char *parametro)
{
    linhaCol(lin, col);
    printf(mensagem, parametro);
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

void drawBoxAroundText(const char *text) {
    int length = strlen(text) + 4;  // Adiciona espaço para a margem
    printf("+");
    for (int i = 0; i < length; i++) {
        printf("-");
    }
    printf("+\n");
    
    printf("| %s |\n", text);
    
    printf("+");
    for (int i = 0; i < length; i++) {
        printf("-");
    }
    printf("+\n");
}