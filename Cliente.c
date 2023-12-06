// cliente.c
#include <stdio.h>
#include <string.h>
#include "cliente.h"
#include "constants.h"

void cadastrar_cliente(int codigo, const char *nome, const char *endereco, const char *telefone)
{
    if (cliente_existe(codigo))
    {
        printf("Cliente já cadastrado.\n");
        return;
    }

    FILE *arquivo = fopen("clientes.bin", "ab");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de clientes para escrita.\n");
        return;
    }
    Cliente novo_cliente;
    novo_cliente.codigo = codigo;

    strncpy(novo_cliente.nome, nome, sizeof(novo_cliente.nome) - 1);
    novo_cliente.nome[sizeof(novo_cliente.nome) - 1] = '\0';

    strncpy(novo_cliente.endereco, endereco, sizeof(novo_cliente.endereco) - 1);
    novo_cliente.endereco[sizeof(novo_cliente.endereco) - 1] = '\0';

    strncpy(novo_cliente.telefone, telefone, sizeof(novo_cliente.telefone) - 1);
    novo_cliente.telefone[sizeof(novo_cliente.telefone) - 1] = '\0';

    novo_cliente.pontos_fidelidade = 0;

    fwrite(&novo_cliente, sizeof(Cliente), 1, arquivo);

    fclose(arquivo);
    printf("Cliente cadastrado com sucesso.\n");
}

void exibir_clientes()
{
    FILE *arquivo = fopen("clientes.bin", "rb");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de clientes para leitura.\n");
        return;
    }

    Cliente cliente;

    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1)
    {
        printf("Codigo: %d\n", cliente.codigo);
        printf("Nome: %s\n", cliente.nome);
        printf("Endereco: %s\n", cliente.endereco);
        printf("Telefone: %s\n", cliente.telefone);
        printf("Pontos de fidelidade: %d\n", cliente.pontos_fidelidade);
        printf("--------------------------\n");
    }

    fclose(arquivo);
}

void pesquisar_cliente(int codigo_cliente)
{
    FILE *arquivo = fopen("clientes.bin", "rb");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de clientes para leitura.\n");
        return;
    }

    Cliente cliente;
    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1)
    {
        if (cliente.codigo == codigo_cliente)
        {
            printf("Codigo: %d\n", cliente.codigo);
            printf("Nome: %s\n", cliente.nome);
            printf("Endereco: %s\n", cliente.endereco);
            printf("Telefone: %s\n", cliente.telefone);
            printf("Pontos de fidelidade: %d\n", cliente.pontos_fidelidade);
            printf("--------------------------\n");
            fclose(arquivo);
            return;
        }
    }

    fclose(arquivo);
    printf("Cliente nao encontrado.\n");
}

void remover_cliente(int codigo_cliente)
{
    FILE *arquivo_original = fopen("clientes.bin", "rb");
    if (arquivo_original == NULL)
    {
        printf("Erro ao abrir o arquivo de clientes para leitura.\n");
        return;
    }

    FILE *arquivo_temporario = fopen("clientes_temp.bin", "wb");
    if (arquivo_temporario == NULL)
    {
        printf("Erro ao criar o arquivo temporário para escrita.\n");
        fclose(arquivo_original);
        return;
    }

    Cliente cliente;

    while (fread(&cliente, sizeof(Cliente), 1, arquivo_original) == 1)
    {
        if (cliente.codigo != codigo_cliente)
        {
            fwrite(&cliente, sizeof(Cliente), 1, arquivo_temporario);
        }
    }

    fclose(arquivo_original);
    fclose(arquivo_temporario);

    if (remove("clientes.bin") == 0)
    {
        if (rename("clientes_temp.bin", "clientes.bin") != 0)
        {
            printf("Erro ao renomear o arquivo temporário.\n");
        }
    }
    else
    {
        printf("Erro ao remover o arquivo original.\n");
    }
}

int cliente_existe(int codigo_cliente)
{
    FILE *arquivo = fopen("clientes.bin", "rb");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de clientes para leitura.\n");
        return 1;
    }

    Cliente cliente;

    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1)
    {
        if (cliente.codigo == codigo_cliente)
        {
            fclose(arquivo);
            return 1;
        }
    }

    fclose(arquivo);
    return 0;
}

void salvar_clientes(Cliente clientes[], int num_clientes)
{
    FILE *arquivo = fopen("clientes.bin", "wb");
    if (arquivo != NULL)
    {
        fwrite(clientes, sizeof(Cliente), num_clientes, arquivo);
        fclose(arquivo);
        printf("Clientes salvos com sucesso.\n");
    }
    else
    {
        printf("Erro ao abrir o arquivo de clientes para escrita.\n");
    }
}

void carregar_clientes(Cliente clientes[], int *num_clientes)
{
    FILE *arquivo = fopen("clientes.bin", "rb");
    if (arquivo != NULL)
    {
        fread(clientes, sizeof(Cliente), MAX_CLIENTES, arquivo);
        fclose(arquivo);
    }
}
