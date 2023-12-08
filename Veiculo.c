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

void atualizar_status(int codigo_veiculo, const char *novo_status)
{
    FILE *arquivo_veiculos = fopen("veiculos.bin", "rb+");
    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para atualizacao.\n");
        return;
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
                return;
            }
            else
            {
                printf("Nao e possivel realizar esta operacao em veiculos alugados");
            }
        }
    }

    fclose(arquivo_veiculos);
    printf("Veiculo com codigo %d nao encontrado.\n", codigo_veiculo);
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
                return 1;
            }
            else if (strcmp(veiculo.status, "Manutencao") == 0)
            {
                printf("O status do veiculo nao pode ser alterado pois ja esta em %s\n", novo_status);
                printf("\nPressione Enter para continuar...\n");
                while (getch() != '\r')
                    ;
                return 1;
            }
        }
    }

    fclose(arquivo_veiculos);
}

// funções principais
void cadastrar_veiculo(Veiculo veiculos[], int *num_veiculos, int codigo, const char *descricao, const char *modelo, const char *cor, const char *placa, float valor_diaria, int ocupantes, const char *status)
{
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

    veiculos[*num_veiculos] = novo_veiculo;
    (*num_veiculos)++;

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
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
    }

    Veiculo veiculo;

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        printf("\nCodigo: %d\n", veiculo.codigo);
        printf("Descricao: %s\n", veiculo.descricao);
        printf("Modelo: %s\n", veiculo.modelo);
        printf("Cor: %s\n", veiculo.cor);
        printf("Placa: %s\n", veiculo.placa);
        printf("Valor diario: %.2f\n", veiculo.valor_diaria);
        printf("Ocupantes: %d\n", veiculo.ocupantes);
        printf("Status: %s\n", veiculo.status);
        drawLine(1, 60, 1);
    }

    fclose(arquivo_veiculos);
    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
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

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (veiculo.codigo == codigo_veiculo)
        {
            drawLine(1, 50, 1);
            printf("\nCodigo: %d\n", veiculo.codigo);
            printf("Descricao: %s\n", veiculo.descricao);
            printf("Modelo: %s\n", veiculo.modelo);
            printf("Cor: %s\n", veiculo.cor);
            printf("Placa: %s\n", veiculo.placa);
            printf("Valor diario: %.2f\n", veiculo.valor_diaria);
            printf("Ocupantes: %d\n", veiculo.ocupantes);
            printf("Status: %s\n", veiculo.status);
            fclose(arquivo_veiculos);
            drawLine(1, 50, 1);

            printf("\nPressione Enter para continuar...\n");
            while (getch() != '\r')
                ;
            return;
        }
        else
        {
            printf("Veiculo nao encontrado.\n");
            printf("\nPressione Enter para continuar...\n");
            while (getch() != '\r')
                ;
            return;
        }
    }

    fclose(arquivo_veiculos);
}

void remover_veiculo(int codigo)
{
    if (!veiculo_existe(codigo))
    {
        printf("Veiculo com o codigo especificado nao existe.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }

    FILE *arquivo_veiculos = fopen("veiculos.bin", "rb");
    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para leitura.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }

    FILE *novo_arquivo = fopen("veiculos_temp.bin", "wb");
    if (novo_arquivo == NULL)
    {
        printf("Erro ao criar o arquivo temporario para escrita.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        fclose(arquivo_veiculos);
        return;
    }

    Veiculo veiculo_atual;

    while (fread(&veiculo_atual, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (veiculo_atual.codigo != codigo)
        {
            fwrite(&veiculo_atual, sizeof(Veiculo), 1, novo_arquivo);
        }
    }

    fclose(arquivo_veiculos);
    fclose(novo_arquivo);

    if (access("veiculos.bin", F_OK) != -1)
    {
        if (remove("veiculos.bin") == 0)
        {
            if (rename("veiculos_temp.bin", "veiculos.bin") == 0)
            {
                printf("Veiculo removido com sucesso.\n");
                printf("\nPressione Enter para continuar...\n");
                while (getch() != '\r')
                    ;
            }
            else
            {
                perror("Erro ao renomear o arquivo temporário");
                printf("\nPressione Enter para continuar...\n");
                while (getch() != '\r')
                    ;
            }
        }
        else
        {
            perror("Erro ao remover o arquivo original");
            printf("\nPressione Enter para continuar...\n");
            while (getch() != '\r')
                ;
        }
    }
    else
    {
        printf("Arquivo original nao encontrado.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
    }

    fclose(arquivo_veiculos);
    fclose(novo_arquivo);
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

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (strcmp(veiculo.status, "Alugado") == 0)
        {
            printf("Codigo: %d\n", veiculo.codigo);
            printf("Descricao: %s\n", veiculo.descricao);
            printf("Modelo: %s\n", veiculo.modelo);
            printf("Cor: %s\n", veiculo.cor);
            printf("Placa: %s\n", veiculo.placa);
            printf("Valor diario: %.2f\n", veiculo.valor_diaria);
            printf("Ocupantes: %d\n", veiculo.ocupantes);
            printf("Status: %s\n", veiculo.status);
            drawLine(1, 60, 1);
            aux = 1;
        }
    }
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

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (strcmp(veiculo.status, "Manutencao") == 0)
        {
            printf("Codigo: %d\n", veiculo.codigo);
            printf("Descricao: %s\n", veiculo.descricao);
            printf("Modelo: %s\n", veiculo.modelo);
            printf("Cor: %s\n", veiculo.cor);
            printf("Placa: %s\n", veiculo.placa);
            printf("Valor diario: %.2f\n", veiculo.valor_diaria);
            printf("Ocupantes: %d\n", veiculo.ocupantes);
            printf("Status: %s\n", veiculo.status);
            drawLine(1, 60, 1);
            aux = 1;
        }
    }
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

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (strcmp(veiculo.status, "Disponivel") == 0)
        {
            printf("Codigo: %d\n", veiculo.codigo);
            printf("Descricao: %s\n", veiculo.descricao);
            printf("Modelo: %s\n", veiculo.modelo);
            printf("Cor: %s\n", veiculo.cor);
            printf("Placa: %s\n", veiculo.placa);
            printf("Valor diario: %.2f\n", veiculo.valor_diaria);
            printf("Ocupantes: %d\n", veiculo.ocupantes);
            printf("Status: %s\n", veiculo.status);
            drawLine(1, 60, 1);
            aux = 1;
        }
    }
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

    atualizar_status(codigo_veiculo, "Disponivel");

    printf("\nO veiculo de codigo %d esta disponivel.\n", codigo_veiculo);
    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
}

void colocar_veiculo_em_manutencao(int codigo_veiculo)
{
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
    atualizar_status(codigo_veiculo, "Manutencao");

    printf("\nO veiculo de codigo %d foi enviado para a manutencao.\n", codigo_veiculo);
    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
}
