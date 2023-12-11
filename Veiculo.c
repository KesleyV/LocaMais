#include "Veiculo.h"
#include <stdio.h>
#include <string.h>
#include "Constants.h"
#include "Locacao.h"
#include <conio.h>
#include "Menu.h"
#include <locale.h>
#include "Decorador.h"
#include <io.h>

// funções auxiliares
int veiculo_existe(int codigo)
{
    FILE *arquivo_veiculos = fopen("veiculos.bin", "rb");

    Veiculo veiculo_encontrado;
    int veiculo_existe = 0;

    while (fread(&veiculo_encontrado, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (veiculo_encontrado.codigo == codigo)
        {
            veiculo_existe = 1;
            break;
        }
    }

    fclose(arquivo_veiculos);

    return veiculo_existe;
}

int atualizar_status(int codigo_veiculo, const char *novo_status)
{
    FILE *arquivo_veiculos = fopen("veiculos.bin", "rb+");
    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para atualizacao.\n");
        return 2;
    }

    fseek(arquivo_veiculos, 0, SEEK_SET);

    Veiculo veiculo;

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (veiculo.codigo == codigo_veiculo)
        {
            if (strcmp(veiculo.status, "Alugado") != 0)
            {
                strcpy(veiculo.status, novo_status);

                fseek(arquivo_veiculos, -sizeof(Veiculo), SEEK_CUR);
                fwrite(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos);

                fclose(arquivo_veiculos);
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }

    fclose(arquivo_veiculos);
}

int verifica_status(int codigo_veiculo, const char *novo_status)
{
    FILE *arquivo_veiculos = fopen("veiculos.bin", "rb+");
    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para atualizacao.\n");
        return 0;
    }

    fseek(arquivo_veiculos, 0, SEEK_SET);

    Veiculo veiculo;

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (veiculo.codigo == codigo_veiculo && strcmp(veiculo.status, novo_status) == 0)
        {
            if (strcmp(veiculo.status, "Disponivel") == 0)
            {
                printf("O status do veiculo nao pode ser alterado pois ja esta %s\n", novo_status);
                printf("\nPressione Enter para continuar...\n");
                while (getch() != '\r')
                    ;
                fclose(arquivo_veiculos);
                return 1;
            }
            else if (strcmp(veiculo.status, "Manutencao") == 0)
            {
                printf("O status do veiculo nao pode ser alterado pois ja esta em %s\n", novo_status);
                printf("\nPressione Enter para continuar...\n");
                while (getch() != '\r')
                    ;
                fclose(arquivo_veiculos);
                return 1;
            }
            else if (strcmp(veiculo.status, "Desativado") == 0)
            {
                printf("O status do veiculo nao pode ser alterado pois ja esta %s\n", novo_status);
                printf("\nPressione Enter para continuar...\n");
                while (getch() != '\r')
                    ;
                fclose(arquivo_veiculos);
                return 1;
            }
            else if (strcmp(veiculo.status, "Ativado") == 0)
            {
                printf("O status do veiculo nao pode ser alterado pois ja esta %s\n", novo_status);
                printf("\nPressione Enter para continuar...\n");
                while (getch() != '\r')
                    ;
                fclose(arquivo_veiculos);
                return 1;
            }
        }
    }

    fclose(arquivo_veiculos);
}

int obter_proximo_codigo_veiculo()
{
    FILE *arquivo_codigo = fopen("codigo_veiculo.bin", "rb+");

    if (arquivo_codigo == NULL)
    {
        arquivo_codigo = fopen("codigo_veiculo.bin", "wb+");
        CodigoVeiculo valor_inicial = {1};
        fwrite(&valor_inicial, sizeof(CodigoVeiculo), 1, arquivo_codigo);
        fclose(arquivo_codigo);
        return valor_inicial.codigo;
    }

    CodigoVeiculo codigo_veiculo;
    fread(&codigo_veiculo, sizeof(CodigoVeiculo), 1, arquivo_codigo);

    codigo_veiculo.codigo++;

    rewind(arquivo_codigo);
    fwrite(&codigo_veiculo, sizeof(CodigoVeiculo), 1, arquivo_codigo);

    fclose(arquivo_codigo);

    return codigo_veiculo.codigo;
}

// funções principais
void cadastrar_veiculo(const char *descricao, const char *modelo, const char *cor, const char *placa, float valor_diaria, int ocupantes, const char *status)
{
    int codigo = obter_proximo_codigo_veiculo();
    if (veiculo_existe(codigo))
    {
        printf("Veiculo ja cadastrado.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }

    Veiculo novo_veiculo;
    novo_veiculo.codigo = codigo;
    strncpy(novo_veiculo.descricao, descricao, sizeof(novo_veiculo.descricao) - 1);
    novo_veiculo.descricao[sizeof(novo_veiculo.descricao) - 1] = '\0';

    strncpy(novo_veiculo.modelo, modelo, sizeof(novo_veiculo.modelo) - 1);
    novo_veiculo.modelo[sizeof(novo_veiculo.modelo) - 1] = '\0';

    strncpy(novo_veiculo.cor, cor, sizeof(novo_veiculo.cor) - 1);
    novo_veiculo.cor[sizeof(novo_veiculo.cor) - 1] = '\0';

    strncpy(novo_veiculo.placa, placa, sizeof(novo_veiculo.placa) - 1);
    novo_veiculo.placa[sizeof(novo_veiculo.placa) - 1] = '\0';

    novo_veiculo.valor_diaria = valor_diaria;
    novo_veiculo.ocupantes = ocupantes;

    strncpy(novo_veiculo.status, status, sizeof(novo_veiculo.status) - 1);
    novo_veiculo.status[sizeof(novo_veiculo.status) - 1] = '\0';

    FILE *arquivo_veiculos = fopen("veiculos.bin", "ab+");

    fwrite(&novo_veiculo, sizeof(Veiculo), 1, arquivo_veiculos);

    fclose(arquivo_veiculos);

    printf("Veiculo cadastrado com sucesso.\n");

    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
}

void exibir_veiculos()
{
    FILE *arquivo_veiculos = fopen("veiculos.bin", "rb");
    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para leitura.\n");
        return;
    }

    Veiculo veiculo;

    int lin1 = 1, col1 = 1, lin2 = 24, col2 = 60;
    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        setlocale(LC_ALL, "C");

        box(lin1, col1, lin2, col2);

        linhaCol(lin1 + 2, col1 + 25);
        printf("LOCA MAIS");
        linhaCol(lin1 + 4, col1 + 25);
        printf("VEICULOS");
        linhaCol(lin1 + 6, col1 + 2);
        drawLine(1, 55, 1);
        linhaCol(lin1 + 8, col1 + 2);
        printf("Codigo: %d\n", veiculo.codigo);
        linhaCol(lin1 + 10, col1 + 2);
        printf("Descricao: %s\n", veiculo.descricao);
        linhaCol(lin1 + 12, col1 + 2);
        printf("Modelo: %s\n", veiculo.modelo);
        linhaCol(lin1 + 14, col1 + 2);
        printf("Cor: %s\n", veiculo.cor);
        linhaCol(lin1 + 16, col1 + 2);
        printf("Placa: %s\n", veiculo.placa);
        linhaCol(lin1 + 18, col1 + 2);
        printf("Valor diario: %.2f\n", veiculo.valor_diaria);
        linhaCol(lin1 + 20, col1 + 2);
        printf("Ocupantes: %d\n", veiculo.ocupantes);
        linhaCol(lin1 + 22, col1 + 2);
        printf("Status: %s\n", veiculo.status);

        for (int i = 0; i < 2; i++)
        {
            printf("\n");
        }

        lin1 = lin2 + 2;
        lin2 = lin1 + 24;
    }

    setlocale(LC_ALL, " ");

    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
    fclose(arquivo_veiculos);
}

void pesquisar_veiculo(int codigo_veiculo)
{
    const char *nome_arquivo = "veiculos.bin";
    FILE *arquivo_veiculos = fopen(nome_arquivo, "rb");

    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para leitura.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }

    Veiculo veiculo;
    int veiculo_encontrado = 0;

    int lin1 = 2, col1 = 1, lin2 = 26, col2 = 60;

    setlocale(LC_ALL, "C");

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (veiculo.codigo == codigo_veiculo)
        {
            box(lin1, col1, lin2, col2);
            linhaCol(lin1 + 2, col1 + 25);
            printf("LOCA MAIS");
            linhaCol(lin1 + 4, col1 + 25);
            printf("VEICULOS");
            linhaCol(lin1 + 6, col1 + 2);
            drawLine(1, 55, 1);
            linhaCol(lin1 + 8, col1 + 2);
            printf("Codigo: %d\n", veiculo.codigo);
            linhaCol(lin1 + 10, col1 + 2);
            printf("Descricao: %s\n", veiculo.descricao);
            linhaCol(lin1 + 12, col1 + 2);
            printf("Modelo: %s\n", veiculo.modelo);
            linhaCol(lin1 + 14, col1 + 2);
            printf("Cor: %s\n", veiculo.cor);
            linhaCol(lin1 + 16, col1 + 2);
            printf("Placa: %s\n", veiculo.placa);
            linhaCol(lin1 + 18, col1 + 2);
            printf("Valor diario: %.2f\n", veiculo.valor_diaria);
            linhaCol(lin1 + 20, col1 + 2);
            printf("Ocupantes: %d\n", veiculo.ocupantes);
            linhaCol(lin1 + 22, col1 + 2);
            printf("Status: %s\n", veiculo.status);
            linhaCol(lin1 + 24, col1 + 2);
            veiculo_encontrado = 1;
            break;
        }
    }

    fclose(arquivo_veiculos);
    setlocale(LC_ALL, " ");

    if (!veiculo_encontrado)
    {
        printf("Veiculo nao encontrado.\n");
    }

    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
}

void desativar_veiculo(int codigo_veiculo)
{
    const char *nome_arquivo = "veiculos.bin";
    FILE *arquivo_veiculos = fopen(nome_arquivo, "rb");

    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para leitura.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }
    if (!veiculo_existe(codigo_veiculo))
    {
        printf("Veiculo nao encontrado.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }
    if (verifica_status(codigo_veiculo, "Dasativado"))
    {
        return;
    }

    Veiculo veiculo;
    int veiculo_encontrado = 0;

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (veiculo.codigo == codigo_veiculo)
        {
            if (strcmp(veiculo.status, "Disponivel") == 0)
            {
                int lin1 = 2, col1 = 1, lin2 = 26, col2 = 60;
                setlocale(LC_ALL, "C");

                box(lin1, col1, lin2, col2);
                linhaCol(lin1 + 2, col1 + 25);
                printf("LOCA MAIS");
                linhaCol(lin1 + 4, col1 + 25);
                printf("VEICULOS");
                linhaCol(lin1 + 6, col1 + 2);
                drawLine(1, 55, 1);
                linhaCol(lin1 + 8, col1 + 2);
                printf("Codigo: %d\n", veiculo.codigo);
                linhaCol(lin1 + 10, col1 + 2);
                printf("Descricao: %s\n", veiculo.descricao);
                linhaCol(lin1 + 12, col1 + 2);
                printf("Modelo: %s\n", veiculo.modelo);
                linhaCol(lin1 + 14, col1 + 2);
                printf("Cor: %s\n", veiculo.cor);
                linhaCol(lin1 + 16, col1 + 2);
                printf("Placa: %s\n", veiculo.placa);
                linhaCol(lin1 + 18, col1 + 2);
                printf("Valor diario: %.2f\n", veiculo.valor_diaria);
                linhaCol(lin1 + 20, col1 + 2);
                printf("Ocupantes: %d\n", veiculo.ocupantes);
                linhaCol(lin1 + 22, col1 + 2);
                printf("Status: %s\n", veiculo.status);
                linhaCol(lin1 + 24, col1 + 2);
                veiculo_encontrado = 1;
                fclose(arquivo_veiculos);
                setlocale(LC_ALL, "");
                break;
            }
            else
            {
                printf("\nO status do veiculo e %s. Voce so pode desativar veiculos que estejam com o status Disponivel.\n", veiculo.status);
                printf("\nPressione Enter para continuar...\n");
                while (getch() != '\r')
                    ;
                fclose(arquivo_veiculos);
                return;
            }
        }
    }

    atualizar_status(codigo_veiculo, "Desativado");
    fclose(arquivo_veiculos);
    printf("\nO veiculo exibido acima foi desativado com sucesso. Agora nao e mais possivel realizar operacoes com o mesmo.\n");
    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
}

void reativar_veiculo(int codigo_veiculo)
{
    const char *nome_arquivo = "veiculos.bin";
    FILE *arquivo_veiculos = fopen(nome_arquivo, "rb");

    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para leitura.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }
    if (!veiculo_existe(codigo_veiculo))
    {
        printf("Veiculo nao encontrado.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }
    if (verifica_status(codigo_veiculo, "Disponivel"))
    {
        return;
    }

    Veiculo veiculo;
    int veiculo_encontrado = 0;

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (veiculo.codigo == codigo_veiculo)
        {
            if (strcmp(veiculo.status, "Desativado") == 0)
            {
                int lin1 = 2, col1 = 1, lin2 = 26, col2 = 60;

                setlocale(LC_ALL, "C");
                box(lin1, col1, lin2, col2);
                linhaCol(lin1 + 2, col1 + 25);
                printf("LOCA MAIS");
                linhaCol(lin1 + 4, col1 + 25);
                printf("VEICULOS");
                linhaCol(lin1 + 6, col1 + 2);
                drawLine(1, 55, 1);
                linhaCol(lin1 + 8, col1 + 2);
                printf("Codigo: %d\n", veiculo.codigo);
                linhaCol(lin1 + 10, col1 + 2);
                printf("Descricao: %s\n", veiculo.descricao);
                linhaCol(lin1 + 12, col1 + 2);
                printf("Modelo: %s\n", veiculo.modelo);
                linhaCol(lin1 + 14, col1 + 2);
                printf("Cor: %s\n", veiculo.cor);
                linhaCol(lin1 + 16, col1 + 2);
                printf("Placa: %s\n", veiculo.placa);
                linhaCol(lin1 + 18, col1 + 2);
                printf("Valor diario: %.2f\n", veiculo.valor_diaria);
                linhaCol(lin1 + 20, col1 + 2);
                printf("Ocupantes: %d\n", veiculo.ocupantes);
                linhaCol(lin1 + 22, col1 + 2);
                printf("Status: %s\n", veiculo.status);
                linhaCol(lin1 + 24, col1 + 2);
                veiculo_encontrado = 1;
                fclose(arquivo_veiculos);
                setlocale(LC_ALL, "C");
                break;
            }
            else
            {
                printf("\nO status do veiculo e %s. Voce so pode reativar veiculos que estejam com o status Desativado.\n", veiculo.status);
                printf("\nPressione Enter para continuar...\n");
                while (getch() != '\r')
                    ;
                fclose(arquivo_veiculos);
                return;
            }
        }
    }

    setlocale(LC_ALL, "");

    atualizar_status(codigo_veiculo, "Disponivel");
    fclose(arquivo_veiculos);

    printf("\nO veiculo exibido acima foi reativado com sucesso. Seu status agora e Disponivel.\n");
    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
}

void exibir_veiculos_alugados()
{
    int aux = 0;
    const char *nome_arquivo = "veiculos.bin";
    FILE *arquivo_veiculos = fopen(nome_arquivo, "rb");

    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para leitura.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }

    Veiculo veiculo;

    int lin1 = 1, col1 = 1, lin2 = 26, col2 = 60;
    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (strcmp(veiculo.status, "Alugado") == 0)
        {
            setlocale(LC_ALL, "C");
            box(lin1, col1, lin2, col2);

            linhaCol(lin1 + 2, col1 + 25);
            printf("LOCA MAIS");
            linhaCol(lin1 + 4, col1 + 25);
            printf("VEICULOS");
            linhaCol(lin1 + 6, col1 + 2);
            drawLine(1, 55, 1);
            linhaCol(lin1 + 8, col1 + 2);
            printf("Codigo: %d\n", veiculo.codigo);
            linhaCol(lin1 + 10, col1 + 2);
            printf("Descricao: %s\n", veiculo.descricao);
            linhaCol(lin1 + 12, col1 + 2);
            printf("Modelo: %s\n", veiculo.modelo);
            linhaCol(lin1 + 14, col1 + 2);
            printf("Cor: %s\n", veiculo.cor);
            linhaCol(lin1 + 16, col1 + 2);
            printf("Placa: %s\n", veiculo.placa);
            linhaCol(lin1 + 18, col1 + 2);
            printf("Valor diario: %.2f\n", veiculo.valor_diaria);
            linhaCol(lin1 + 20, col1 + 2);
            printf("Ocupantes: %d\n", veiculo.ocupantes);
            linhaCol(lin1 + 22, col1 + 2);
            printf("Status: %s\n", veiculo.status);
            linhaCol(lin1 + 24, col1 + 2);

            for (int i = 0; i < 2; i++)
            {
                printf("\n");
            }

            lin1 = lin2 + 2;
            lin2 = lin1 + 26;
            aux = 1;
        }
    }
    setlocale(LC_ALL, " ");
    if (aux == 0)
    {
        printf("Nenhum veiculo esta alugado.\n");
    }
    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
    fclose(arquivo_veiculos);
}

void exibir_veiculos_em_manutencao()
{
    int aux = 0;
    const char *nome_arquivo = "veiculos.bin";
    FILE *arquivo_veiculos = fopen(nome_arquivo, "rb");

    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para leitura.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }

    Veiculo veiculo;

    int lin1 = 1, col1 = 1, lin2 = 26, col2 = 60;
    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (strcmp(veiculo.status, "Manutencao") == 0)
        {
            setlocale(LC_ALL, "C");
            box(lin1, col1, lin2, col2);

            linhaCol(lin1 + 2, col1 + 25);
            printf("LOCA MAIS");
            linhaCol(lin1 + 4, col1 + 25);
            printf("VEICULOS");
            linhaCol(lin1 + 6, col1 + 2);
            drawLine(1, 55, 1);
            linhaCol(lin1 + 8, col1 + 2);
            printf("Codigo: %d\n", veiculo.codigo);
            linhaCol(lin1 + 10, col1 + 2);
            printf("Descricao: %s\n", veiculo.descricao);
            linhaCol(lin1 + 12, col1 + 2);
            printf("Modelo: %s\n", veiculo.modelo);
            linhaCol(lin1 + 14, col1 + 2);
            printf("Cor: %s\n", veiculo.cor);
            linhaCol(lin1 + 16, col1 + 2);
            printf("Placa: %s\n", veiculo.placa);
            linhaCol(lin1 + 18, col1 + 2);
            printf("Valor diario: %.2f\n", veiculo.valor_diaria);
            linhaCol(lin1 + 20, col1 + 2);
            printf("Ocupantes: %d\n", veiculo.ocupantes);
            linhaCol(lin1 + 22, col1 + 2);
            printf("Status: %s\n", veiculo.status);
            linhaCol(lin1 + 24, col1 + 2);

            for (int i = 0; i < 2; i++)
            {
                printf("\n");
            }

            lin1 = lin2 + 2;
            lin2 = lin1 + 26;
            aux = 1;
        }
    }
    setlocale(LC_ALL, " ");
    if (aux == 0)
    {
        printf("Nenhum veiculo esta em manutencao.\n");
    }
    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
    fclose(arquivo_veiculos);
}

void exibir_veiculos_disponiveis()
{
    int aux = 0;
    const char *nome_arquivo = "veiculos.bin";
    FILE *arquivo_veiculos = fopen(nome_arquivo, "rb");

    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para leitura.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }

    Veiculo veiculo;
    int lin1 = 1, col1 = 1, lin2 = 26, col2 = 60;

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (strcmp(veiculo.status, "Disponivel") == 0)
        {
            setlocale(LC_ALL, "C");

            box(lin1, col1, lin2, col2);

            linhaCol(lin1 + 2, col1 + 25);
            printf("LOCA MAIS");
            linhaCol(lin1 + 4, col1 + 25);
            printf("VEICULOS");
            linhaCol(lin1 + 6, col1 + 2);
            drawLine(1, 55, 1);
            linhaCol(lin1 + 8, col1 + 2);
            printf("Codigo: %d\n", veiculo.codigo);
            linhaCol(lin1 + 10, col1 + 2);
            printf("Descricao: %s\n", veiculo.descricao);
            linhaCol(lin1 + 12, col1 + 2);
            printf("Modelo: %s\n", veiculo.modelo);
            linhaCol(lin1 + 14, col1 + 2);
            printf("Cor: %s\n", veiculo.cor);
            linhaCol(lin1 + 16, col1 + 2);
            printf("Placa: %s\n", veiculo.placa);
            linhaCol(lin1 + 18, col1 + 2);
            printf("Valor diario: %.2f\n", veiculo.valor_diaria);
            linhaCol(lin1 + 20, col1 + 2);
            printf("Ocupantes: %d\n", veiculo.ocupantes);
            linhaCol(lin1 + 22, col1 + 2);
            printf("Status: %s\n", veiculo.status);
            linhaCol(lin1 + 24, col1 + 2);

            for (int i = 0; i < 2; i++)
            {
                printf("\n");
            }

            lin1 = lin2 + 2;
            lin2 = lin1 + 26;
            aux = 1;
        }
    }
    setlocale(LC_ALL, " ");
    if (aux == 0)
    {
        printf("Nenhum veiculo esta disponivel.\n");
    }
    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
    fclose(arquivo_veiculos);
}

void remover_veiculo_da_manutencao(int codigo_veiculo)
{
    const char *nome_arquivo = "veiculos.bin";
    FILE *arquivo_veiculos = fopen(nome_arquivo, "rb");

    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para leitura.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        fclose(arquivo_veiculos);
        return;
    }

    if (!veiculo_existe(codigo_veiculo))
    {
        printf("Veiculo nao encontrado.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        fclose(arquivo_veiculos);
        return;
    }
    if (verifica_status(codigo_veiculo, "Disponivel"))
    {
        fclose(arquivo_veiculos);
        return;
    }

    Veiculo veiculo;
    int veiculo_encontrado = 0;

    int lin1 = 2, col1 = 1, lin2 = 26, col2 = 60;

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (veiculo.codigo == codigo_veiculo)
        {
            if (strcmp(veiculo.status, "Manutencao") == 0)
            {
                setlocale(LC_ALL, "C");
                box(lin1, col1, lin2, col2);
                linhaCol(lin1 + 2, col1 + 25);
                printf("LOCA MAIS");
                linhaCol(lin1 + 4, col1 + 25);
                printf("VEICULOS");
                linhaCol(lin1 + 6, col1 + 2);
                drawLine(1, 55, 1);
                linhaCol(lin1 + 8, col1 + 2);
                printf("Codigo: %d\n", veiculo.codigo);
                linhaCol(lin1 + 10, col1 + 2);
                printf("Descricao: %s\n", veiculo.descricao);
                linhaCol(lin1 + 12, col1 + 2);
                printf("Modelo: %s\n", veiculo.modelo);
                linhaCol(lin1 + 14, col1 + 2);
                printf("Cor: %s\n", veiculo.cor);
                linhaCol(lin1 + 16, col1 + 2);
                printf("Placa: %s\n", veiculo.placa);
                linhaCol(lin1 + 18, col1 + 2);
                printf("Valor diario: %.2f\n", veiculo.valor_diaria);
                linhaCol(lin1 + 20, col1 + 2);
                printf("Ocupantes: %d\n", veiculo.ocupantes);
                linhaCol(lin1 + 22, col1 + 2);
                printf("Status: %s\n", veiculo.status);
                linhaCol(lin1 + 24, col1 + 2);
                veiculo_encontrado = 1;
                fclose(arquivo_veiculos);
                break;
            }
            else
            {
                printf("\nO status do veiculo e %s. So e possivel retirar veiculos da manutencao que estejam com o status Manutencao.\n", veiculo.status);
                printf("\nPressione Enter para continuar...\n");
                while (getch() != '\r')
                    ;
                fclose(arquivo_veiculos);
                return;
            }
        }
    }

    setlocale(LC_ALL, "");
    if (atualizar_status(codigo_veiculo, "Disponivel"))
    {
        printf("\nO veiculo exibido acima foi retirado da manutencao e encontra-se Disponivel.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        fclose(arquivo_veiculos);
        return;
    }
}

void colocar_veiculo_em_manutencao(int codigo_veiculo)
{
    const char *nome_arquivo = "veiculos.bin";
    FILE *arquivo_veiculos = fopen(nome_arquivo, "rb");

    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para leitura.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }
    if (!veiculo_existe(codigo_veiculo))
    {
        printf("Veiculo nao encontrado.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }
    if (verifica_status(codigo_veiculo, "Manutencao"))
    {
        return;
    }

    Veiculo veiculo;
    int veiculo_encontrado = 0;

    int lin1 = 2, col1 = 1, lin2 = 26, col2 = 60;

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (veiculo.codigo == codigo_veiculo)
        {
            if (strcmp(veiculo.status, "Disponivel") == 0)
            {
                setlocale(LC_ALL, "C");
                box(lin1, col1, lin2, col2);
                linhaCol(lin1 + 2, col1 + 25);
                printf("LOCA MAIS");
                linhaCol(lin1 + 4, col1 + 25);
                printf("VEICULOS");
                linhaCol(lin1 + 6, col1 + 2);
                drawLine(1, 55, 1);
                linhaCol(lin1 + 8, col1 + 2);
                printf("Codigo: %d\n", veiculo.codigo);
                linhaCol(lin1 + 10, col1 + 2);
                printf("Descricao: %s\n", veiculo.descricao);
                linhaCol(lin1 + 12, col1 + 2);
                printf("Modelo: %s\n", veiculo.modelo);
                linhaCol(lin1 + 14, col1 + 2);
                printf("Cor: %s\n", veiculo.cor);
                linhaCol(lin1 + 16, col1 + 2);
                printf("Placa: %s\n", veiculo.placa);
                linhaCol(lin1 + 18, col1 + 2);
                printf("Valor diario: %.2f\n", veiculo.valor_diaria);
                linhaCol(lin1 + 20, col1 + 2);
                printf("Ocupantes: %d\n", veiculo.ocupantes);
                linhaCol(lin1 + 22, col1 + 2);
                printf("Status: %s\n", veiculo.status);
                linhaCol(lin1 + 24, col1 + 2);
                veiculo_encontrado = 1;
                fclose(arquivo_veiculos);
                break;
            }
            else
            {
                printf("\nO status do veiculo e %s. So e possivel enviar veiculos para a manutencao que estejam com o status Disponivel.\n", veiculo.status);
                printf("\nPressione Enter para continuar...\n");
                while (getch() != '\r')
                    ;
                fclose(arquivo_veiculos);
                return;
            }
        }
    }

    setlocale(LC_ALL, "");

    if (atualizar_status(codigo_veiculo, "Manutencao"))
    {
        printf("\nO veiculo exibido acima foi enviado para a manutencao.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        fclose(arquivo_veiculos);
        return;
    }
}
