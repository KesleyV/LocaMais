#include "Veiculo.h"
#include <stdio.h>
#include <string.h>
#include "Constants.h"
#include "Locacao.h"

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

// funções principais
void cadastrar_veiculo(Veiculo veiculos[], int *num_veiculos, int codigo, const char *descricao, const char *modelo, const char *cor, const char *placa, float valor_diaria, int ocupantes, const char *status)
{
    if (veiculo_existe(codigo))
    {
        printf("Veiculo ja cadastrado.\n");
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

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        printf("Codigo: %d\n", veiculo.codigo);
        printf("Descricao: %s\n", veiculo.descricao);
        printf("Modelo: %s\n", veiculo.modelo);
        printf("Cor: %s\n", veiculo.cor);
        printf("Placa: %s\n", veiculo.placa);
        printf("Valor diario: %.2f\n", veiculo.valor_diaria);
        printf("Ocupantes: %d\n", veiculo.ocupantes);
        printf("Status: %s\n", veiculo.status);
        printf("--------------------------\n");
    }

    fclose(arquivo_veiculos);
}

void pesquisar_veiculo(int codigo_veiculo)
{
    const char *nome_arquivo = "veiculos.bin";
    FILE *arquivo_veiculos = fopen(nome_arquivo, "rb");

    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para leitura.\n");
        return;
    }

    Veiculo veiculo;

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (veiculo.codigo == codigo_veiculo)
        {
            printf("Codigo: %d\n", veiculo.codigo);
            printf("Descricao: %s\n", veiculo.descricao);
            printf("Modelo: %s\n", veiculo.modelo);
            printf("Cor: %s\n", veiculo.cor);
            printf("Placa: %s\n", veiculo.placa);
            printf("Valor diario: %.2f\n", veiculo.valor_diaria);
            printf("Ocupantes: %d\n", veiculo.ocupantes);
            printf("Status: %s\n", veiculo.status);
            printf("--------------------------\n");

            fclose(arquivo_veiculos);
            return;
        }
    }

    fclose(arquivo_veiculos);
    printf("Veiculo nao encontrado.\n");
}

void remover_veiculo(int codigo)
{
    if (!veiculo_existe(codigo))
    {
        printf("Veiculo com o codigo especificado nao existe.\n");
        return;
    }

    FILE *arquivo_veiculos = fopen("veiculos.bin", "rb");
    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para leitura.\n");
        return;
    }

    FILE *novo_arquivo = fopen("temp.bin", "wb");
    if (novo_arquivo == NULL)
    {
        printf("Erro ao criar o arquivo temporario para escrita.\n");
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

    if (remove("veiculos.bin") == 0)
    {
        if (rename("temp.bin", "veiculos.bin") != 0)
        {
            printf("Erro ao renomear o arquivo temporário.\n");
        }
        else
        {
            printf("Veiculo removido com sucesso.\n");
        }
    }
    else
    {
        printf("Erro ao remover o arquivo original.\n");
    }
}

void exibir_veiculos_alugados()
{
    const char *nome_arquivo = "veiculos.bin";
    FILE *arquivo_veiculos = fopen(nome_arquivo, "rb");

    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para leitura.\n");
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
            printf("--------------------------\n");
        }
    }

    fclose(arquivo_veiculos);
}

void exibir_veiculos_em_manutencao()
{
    const char *nome_arquivo = "veiculos.bin";
    FILE *arquivo_veiculos = fopen(nome_arquivo, "rb");

    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para leitura.\n");
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
            printf("--------------------------\n");
        }
    }

    fclose(arquivo_veiculos);
}

void remover_veiculo_da_manutencao(int codigo_veiculo)
{
    atualizar_status(codigo_veiculo, "Disponivel");

    printf("\nO veiculo de codigo %d esta disponivel.\n", codigo_veiculo);
}

void colocar_veiculo_em_manutencao(int codigo_veiculo)
{
    atualizar_status(codigo_veiculo, "Manutencao");

    printf("\nO veiculo de codigo %d foi enviado para a manutencao.\n", codigo_veiculo);
}
