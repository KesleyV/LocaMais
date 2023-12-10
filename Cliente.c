// cliente.c
#include <stdio.h>
#include <string.h>
#include "cliente.h"
#include "Menu.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <conio.h>
#include "Decorador.h"
#include <locale.h>
#include "Constants.h"

// função auxiliar
int cliente_existe(int codigo_cliente)
{
    FILE *arquivo = fopen("clientes.bin", "rb");

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

int obter_proximo_codigo_cliente()
{
    FILE *arquivo_codigo = fopen("codigo_cliente.bin", "rb+");

    if (arquivo_codigo == NULL)
    {
        arquivo_codigo = fopen("codigo_cliente.bin", "wb+");
        CodigoCliente valor_inicial = {1};
        fwrite(&valor_inicial, sizeof(CodigoCliente), 1, arquivo_codigo);
        fclose(arquivo_codigo);
        return valor_inicial.codigo;
    }

    CodigoCliente codigo_cliente;
    fread(&codigo_cliente, sizeof(CodigoCliente), 1, arquivo_codigo);

    codigo_cliente.codigo++;

    rewind(arquivo_codigo);
    fwrite(&codigo_cliente, sizeof(CodigoCliente), 1, arquivo_codigo);

    fclose(arquivo_codigo);

    return codigo_cliente.codigo;
}

int verificaLocacao(int codigo_cliente)
{
    FILE *arquivo_locacao = fopen("locacoes.bin", "rb");
    if (arquivo_locacao == NULL)
    {
        fclose(arquivo_locacao);
        return 0;
    }
    Locacao locacao;

    while (fread(&locacao, sizeof(Locacao), 1, arquivo_locacao) == 1)
    {
        if (locacao.codigo_cliente == codigo_cliente)
        {
            fclose(arquivo_locacao);
            return 1;
        }
        else
        {
            fclose(arquivo_locacao);
            return 0;
        }
    }
}

int atualizar_status_cliente(int codigo_cliente, const char *novo_status)
{
    FILE *arquivo_cliente = fopen("clientes.bin", "rb+");
    if (arquivo_cliente == NULL)
    {
        printf("Erro ao abrir o arquivo de clientes para atualizacao.\n");
        return 2;
    }

    fseek(arquivo_cliente, 0, SEEK_SET);

    Cliente cliente;

    while (fread(&cliente, sizeof(Cliente), 1, arquivo_cliente) == 1)
    {
        if (cliente.codigo == codigo_cliente)
        {
            if (!verificaLocacao(codigo_cliente))
            {
                strcpy(cliente.status, novo_status);

                fseek(arquivo_cliente, -sizeof(Cliente), SEEK_CUR);
                fwrite(&cliente, sizeof(Cliente), 1, arquivo_cliente);

                fclose(arquivo_cliente);
                return 1;
            }
            else
            {
                fclose(arquivo_cliente);
                printf("Nao foi possivel atualizar o status do cliente acima pois o mesmo tem uma locacao ativa em seu nome.");
                return 0;
            }
        }
    }

    fclose(arquivo_cliente);
    printf("Veiculo com codigo %d nao encontrado.\n", codigo_cliente);
}

// funções principais
void cadastrar_cliente(const char *nome, const char *endereco, const char *telefone)
{
    int codigo = obter_proximo_codigo_cliente();

    if (cliente_existe(codigo))
    {
        printf("Cliente ja cadastrado.\n");
        printf("Pressione Enter para continuar...");
        while (getch() != '\r')
            ;
        return;
    }

    if (access("clientes.bin", F_OK) == -1)
    {
        FILE *arquivo_criacao = fopen("clientes.bin", "wb");
        if (arquivo_criacao == NULL)
        {
            printf("Erro ao criar o arquivo de clientes.\n");
            return;
        }
        fclose(arquivo_criacao);
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
    strcpy(novo_cliente.status, "Ativo");

    fwrite(&novo_cliente, sizeof(Cliente), 1, arquivo);

    fclose(arquivo);
    printf("Cliente cadastrado com sucesso.\n");

    printf("Pressione Enter para continuar...");
    while (getch() != '\r')
        ;
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

    int lin1 = 1, col1 = 1, lin2 = 22, col2 = 60;

    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1)
    {
        setlocale(LC_ALL, "C");
        box(lin1, col1, lin2, col2);

        linhaCol(lin1 + 2, col1 + 25);
        printf("LOCA MAIS");
        linhaCol(lin1 + 4, col1 + 26);
        printf("CLIENTE");
        linhaCol(lin1 + 6, col1 + 2);
        drawLine(1, 56, 1);
        linhaCol(lin1 + 8, col1 + 2);
        printf("Codigo: %d\n", cliente.codigo);
        linhaCol(lin1 + 10, col1 + 2);
        printf("Nome: %s\n", cliente.nome);
        linhaCol(lin1 + 12, col1 + 2);
        printf("Endereco: %s\n", cliente.endereco);
        linhaCol(lin1 + 14, col1 + 2);
        printf("Telefone: %s\n", cliente.telefone);
        linhaCol(lin1 + 16, col1 + 2);
        printf("Pontos de fidelidade: %d\n", cliente.pontos_fidelidade);
        linhaCol(lin1 + 18, col1 + 2);
        printf("Status: %s\n", cliente.status);
        linhaCol(lin1 + 20, col1 + 2);

        for (int i = 0; i < 2; i++)
        {
            printf("\n");
        }

        lin1 = lin2 + 2;
        lin2 = lin1 + 22;
    }

    setlocale(LC_ALL, " ");

    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
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
    setlocale(LC_ALL, "C");
    int lin1 = 1, col1 = 1, lin2 = 22, col2 = 60;

    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1)
    {
        if (cliente.codigo == codigo_cliente)
        {
            box(lin1, col1, lin2, col2);
            linhaCol(lin1 + 2, col1 + 25);
            printf("LOCA MAIS");
            linhaCol(lin1 + 4, col1 + 26);
            printf("CLIENTE");
            linhaCol(lin1 + 6, col1 + 2);
            drawLine(1, 56, 1);
            linhaCol(lin1 + 8, col1 + 2);
            printf("Codigo: %d\n", cliente.codigo);
            linhaCol(lin1 + 10, col1 + 2);
            printf("Nome: %s\n", cliente.nome);
            linhaCol(lin1 + 12, col1 + 2);
            printf("Endereco: %s\n", cliente.endereco);
            linhaCol(lin1 + 14, col1 + 2);
            printf("Telefone: %s\n", cliente.telefone);
            linhaCol(lin1 + 16, col1 + 2);
            printf("Pontos de fidelidade: %d\n", cliente.pontos_fidelidade);
            linhaCol(lin1 + 18, col1 + 2);
            printf("Status do cliente: %s\n", cliente.status);
            linhaCol(lin1 + 20, col1 + 2);

            setlocale(LC_ALL, "");
            printf("\n");
            printf("\nPressione Enter para continuar...\n");
            while (getch() != '\r')
                ;
            fclose(arquivo);
            return;
        }
    }
    setlocale(LC_ALL, "");
    fclose(arquivo);
    printf("Cliente nao encontrado.\n");
    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
}

void desativar_cliente(int codigo_cliente)
{
    if (!cliente_existe(codigo_cliente))
    {
        printf("Cliente com o codigo especificado nao existe.\n");
        printf("Pressione Enter para continuar...");
        while (getch() != '\r')
            ;
        return;
    }

    FILE *arquivo_original = fopen("clientes.bin", "rb");
    if (arquivo_original == NULL)
    {
        printf("Erro ao abrir o arquivo de clientes para leitura.\n");
        printf("Pressione Enter para continuar...");
        while (getch() != '\r')
            ;
        return;
    }

    Cliente cliente;

    setlocale(LC_ALL, "C");

    int verifica_locacao = verificaLocacao(cliente.codigo);

    while (fread(&cliente, sizeof(Cliente), 1, arquivo_original) == 1)
    {
        if (strcmp(cliente.status, "Ativo") == 0)
        {
            int lin1 = 2, col1 = 1, lin2 = 22, col2 = 60;
            box(lin1, col1, lin2, col2);

            linhaCol(lin1 + 2, col1 + 25);
            printf("LOCA MAIS");
            linhaCol(lin1 + 4, col1 + 26);
            printf("CLIENTE");
            linhaCol(lin1 + 6, col1 + 2);
            drawLine(1, 56, 1);
            linhaCol(lin1 + 8, col1 + 2);
            printf("Codigo: %d\n", cliente.codigo);
            linhaCol(lin1 + 10, col1 + 2);
            printf("Nome: %s\n", cliente.nome);
            linhaCol(lin1 + 12, col1 + 2);
            printf("Endereco: %s\n", cliente.endereco);
            linhaCol(lin1 + 14, col1 + 2);
            printf("Telefone: %s\n", cliente.telefone);
            linhaCol(lin1 + 16, col1 + 2);
            printf("Pontos de fidelidade: %d\n", cliente.pontos_fidelidade);
            linhaCol(lin1 + 18, col1 + 2);
            printf("Status: %s", cliente.status);
            linhaCol(lin1 + 20, col1 + 2);
            fclose(arquivo_original);
            break;
        }
        else
        {
            printf("\nNao e possivel desativar um cliente que ja esta desativado.\n");
            printf("\nPressione Enter para continuar...\n");
            while (getch() != '\r')
                ;
            fclose(arquivo_original);
            return;
        }
    }

    if (atualizar_status_cliente(codigo_cliente, "Desativado"))
    {
        printf("\nO cliente exibido acima foi desativado com sucesso. Agora nao e mais possivel realizar operacoes com o mesmo.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
    }
    else
    {
        printf("Nao foi possivel atualizar o status do cliente acima pois o mesmo tem uma locacao ativa em seu nome.");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
    }

    setlocale(LC_ALL, "");

    fclose(arquivo_original);
}

void reativar_cliente(int codigo_cliente)
{
    if (!cliente_existe(codigo_cliente))
    {
        printf("Cliente com o codigo especificado nao existe.\n");
        printf("Pressione Enter para continuar...");
        while (getch() != '\r')
            ;
        return;
    }

    FILE *arquivo_original = fopen("clientes.bin", "rb");
    if (arquivo_original == NULL)
    {
        printf("Erro ao abrir o arquivo de clientes para leitura.\n");
        printf("Pressione Enter para continuar...");
        while (getch() != '\r')
            ;
        return;
    }

    Cliente cliente;

    int lin1 = 2, col1 = 1, lin2 = 22, col2 = 60;

    setlocale(LC_ALL, "C");

    while (fread(&cliente, sizeof(Cliente), 1, arquivo_original) == 1)
    {
        if (strcmp(cliente.status, "Desativado") == 0)
        {
            box(lin1, col1, lin2, col2);

            linhaCol(lin1 + 2, col1 + 25);
            printf("LOCA MAIS");
            linhaCol(lin1 + 4, col1 + 20);
            printf("CLIENTE");
            linhaCol(lin1 + 6, col1 + 2);
            drawLine(1, 56, 1);
            linhaCol(lin1 + 8, col1 + 2);
            printf("Codigo: %d\n", cliente.codigo);
            linhaCol(lin1 + 10, col1 + 2);
            printf("Nome: %s\n", cliente.nome);
            linhaCol(lin1 + 12, col1 + 2);
            printf("Endereco: %s\n", cliente.endereco);
            linhaCol(lin1 + 14, col1 + 2);
            printf("Telefone: %s\n", cliente.telefone);
            linhaCol(lin1 + 16, col1 + 2);
            printf("Pontos de fidelidade: %d\n", cliente.pontos_fidelidade);
            linhaCol(lin1 + 18, col1 + 2);
            printf("Status: %s", cliente.status);
            linhaCol(lin1 + 20, col1 + 2);
            fclose(arquivo_original);
            break;
        }
        else
        {
            printf("\nNao e possivel ativar um cliente que ja esta Ativo.\n");
            printf("\nPressione Enter para continuar...\n");
            while (getch() != '\r')
                ;
            fclose(arquivo_original);
            return;
        }
    }

    setlocale(LC_ALL, "");

    atualizar_status_cliente(codigo_cliente, "Ativo");
    fclose(arquivo_original);

    printf("\nO cliente exibido acima foi ativado com sucesso.\n");
    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
}