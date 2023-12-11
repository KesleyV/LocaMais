#include <stdio.h>
#include <string.h>
#include "locacao.h"
#include "veiculo.h"
#include "Cliente.h"
#include "Decorador.h"
#include <time.h>
#include <conio.h>
#include "constants.h"
#include <locale.h>
#include "Menu.h"
#include <io.h>

// funções auxiliares

int diferenca_dias_entrada(const char *data_retirada, const char *data_devolucao)
{
    struct tm tm_retirada = {0};
    struct tm tm_devolucao = {0};

    sscanf(data_retirada, "%d/%d/%d", &tm_retirada.tm_mday, &tm_retirada.tm_mon, &tm_retirada.tm_year);
    sscanf(data_devolucao, "%d/%d/%d", &tm_devolucao.tm_mday, &tm_devolucao.tm_mon, &tm_devolucao.tm_year);

    tm_retirada.tm_year -= 1900;
    tm_devolucao.tm_year -= 1900;
    tm_retirada.tm_mon -= 1;
    tm_devolucao.tm_mon -= 1;

    time_t t_retirada = mktime(&tm_retirada);
    time_t t_devolucao = mktime(&tm_devolucao);

    double diferenca_segundos = difftime(t_devolucao, t_retirada);
    int diferenca_dias = diferenca_segundos / (60 * 60 * 24);

    return diferenca_dias;
}

int locacao_existe(int codigo_locacao)
{
    FILE *arquivo_locacoes = fopen("locacoes.bin", "rb");

    Locacao locacao_atual;
    int locacao_existe = 0;

    while (fread(&locacao_atual, sizeof(Locacao), 1, arquivo_locacoes) == 1)
    {
        if (locacao_atual.codigo == codigo_locacao)
        {
            locacao_existe = 1;
            break;
        }
    }

    fclose(arquivo_locacoes);
    return locacao_existe;
}

int buscar_veiculo_disponivel(const char *status, int num_ocupantes)
{
    FILE *arquivo_veiculos = fopen("veiculos.bin", "rb");
    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veículos para leitura.\n");
        return -1;
    }

    int veiculo_disponivel = -1;
    Veiculo veiculo;

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (strcmp(veiculo.status, status) == 0 && veiculo.ocupantes >= num_ocupantes)
        {
            veiculo_disponivel = veiculo.codigo;
            break;
        }
    }

    fclose(arquivo_veiculos);

    if (veiculo_disponivel == -1)
    {
        printf("Nenhum veiculo disponivel para locacao.\n");
        return -1;
    }

    return veiculo_disponivel;
}

float diariaVeiculo(int codigo_veiculo)
{
    FILE *arquivo_veiculos = fopen("veiculos.bin", "rb");
    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veículos para leitura.\n");
        return -1;
    }

    Veiculo veiculo;
    int veiculo_encontrado = 0;
    float valor_diaria = -1;

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (veiculo.codigo == codigo_veiculo)
        {
            veiculo_encontrado = 1;
            valor_diaria = veiculo.valor_diaria;
            break;
        }
    }

    fclose(arquivo_veiculos);

    if (!veiculo_encontrado)
    {
        printf("Veiculo nao encontrado.\n");
        return -1;
    }

    return valor_diaria;
}

int temDesconto(int codigo_cliente)
{
    FILE *arquivo_cliente = fopen("clientes.bin", "rb");
    if (arquivo_cliente == NULL)
    {
        printf("Erro ao abrir o arquivo de veículos para leitura.\n");
        return -1;
    }

    Cliente cliente;
    int cliente_encontrado = 0;
    int desconto = 0;

    while (fread(&cliente, sizeof(Cliente), 1, arquivo_cliente) == 1)
    {
        if (cliente.codigo == codigo_cliente)
        {
            cliente_encontrado = 1;
            if (cliente.desconto == 1)
            {
                desconto = 1;
                fclose(arquivo_cliente);
                return desconto;
            }
            else
            {
                desconto = 0;
                fclose(arquivo_cliente);
                return desconto;
            }
        }
    }

    fclose(arquivo_cliente);

    if (!cliente_encontrado)
    {
        printf("Cliente nao encontrado.\n");
        return -1;
    }

    return desconto;
}

void atualizar_status_veiculo(int codigo_veiculo, const char *novo_status)
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
            strcpy(veiculo.status, novo_status);

            fseek(arquivo_veiculos, -sizeof(Veiculo), SEEK_CUR);
            fwrite(&veiculo, sizeof(Veiculo), 1, arquivo_veiculos);

            fclose(arquivo_veiculos);
            return;
        }
    }

    fclose(arquivo_veiculos);
    printf("Veiculo com codigo %d nao encontrado.\n", codigo_veiculo);
}

float calcular_valor_aluguel(int quantidade_dias, float valor_diaria, int seguro)
{
    float valor_aluguel = quantidade_dias * valor_diaria;

    if (seguro == 1)
    {
        valor_aluguel += 50.0;
    }

    return valor_aluguel;
}

int calcular_pontos_fidelidade(int codigo_cliente, int diarias)
{
    FILE *arquivo_clientes = fopen("clientes.bin", "rb+");

    if (arquivo_clientes == NULL)
    {
        perror("Erro ao abrir arquivo de clientes");
        return 0;
    }

    fseek(arquivo_clientes, 0, SEEK_SET);

    Cliente cliente;

    while (fread(&cliente, sizeof(Cliente), 1, arquivo_clientes) == 1)
    {
        if (cliente.codigo == codigo_cliente)
        {
            cliente.pontos_fidelidade += diarias * 10;

            fseek(arquivo_clientes, -sizeof(Cliente), SEEK_CUR);
            fwrite(&cliente, sizeof(Cliente), 1, arquivo_clientes);

            fclose(arquivo_clientes);
            return cliente.pontos_fidelidade;
        }
    }

    fclose(arquivo_clientes);
}

int debitar_pontos_fidelidade(int codigo_cliente)
{
    FILE *arquivo_clientes = fopen("clientes.bin", "rb+");

    if (arquivo_clientes == NULL)
    {
        printf("Erro ao abrir arquivo de clientes.\n");
        return 0;
    }

    fseek(arquivo_clientes, 0, SEEK_SET);

    Cliente cliente;

    while (fread(&cliente, sizeof(Cliente), 1, arquivo_clientes) == 1)
    {
        if (cliente.codigo == codigo_cliente)
        {
            cliente.pontos_fidelidade -= 500;

            fseek(arquivo_clientes, -sizeof(Cliente), SEEK_CUR);
            fwrite(&cliente, sizeof(Cliente), 1, arquivo_clientes);

            fclose(arquivo_clientes);
            return cliente.pontos_fidelidade;
        }
    }

    fclose(arquivo_clientes);
    return 0;
}

void habilitaDesconto(int codigo_cliente)
{
    FILE *arquivo_clientes = fopen("clientes.bin", "rb+");

    if (arquivo_clientes == NULL)
    {
        printf("Erro ao abrir arquivo de clientes.\n");
    }

    fseek(arquivo_clientes, 0, SEEK_SET);

    Cliente cliente;

    while (fread(&cliente, sizeof(Cliente), 1, arquivo_clientes) == 1)
    {
        if (cliente.codigo == codigo_cliente)
        {
            cliente.desconto = 1;

            fseek(arquivo_clientes, -sizeof(Cliente), SEEK_CUR);
            fwrite(&cliente, sizeof(Cliente), 1, arquivo_clientes);

            fclose(arquivo_clientes);
        }
    }

    fclose(arquivo_clientes);
}

void desabilitaDesconto(int codigo_cliente)
{
    FILE *arquivo_clientes = fopen("clientes.bin", "rb+");

    if (arquivo_clientes == NULL)
    {
        printf("Erro ao abrir arquivo de clientes.\n");
    }

    fseek(arquivo_clientes, 0, SEEK_SET);

    Cliente cliente;

    while (fread(&cliente, sizeof(Cliente), 1, arquivo_clientes) == 1)
    {
        if (cliente.codigo == codigo_cliente)
        {
            cliente.desconto = 0;

            fseek(arquivo_clientes, -sizeof(Cliente), SEEK_CUR);
            fwrite(&cliente, sizeof(Cliente), 1, arquivo_clientes);

            fclose(arquivo_clientes);
        }
    }

    fclose(arquivo_clientes);
}

int validar_data_entrega(const char *data_retirada, const char *data_devolucao)
{
    int dia_ret, mes_ret, ano_ret, dia_dev, mes_dev, ano_dev;

    sscanf(data_retirada, "%d/%d/%d", &dia_ret, &mes_ret, &ano_ret);
    sscanf(data_devolucao, "%d/%d/%d", &dia_dev, &mes_dev, &ano_dev);

    if (ano_dev < ano_ret || (ano_dev == ano_ret && (mes_dev < mes_ret || (mes_dev == mes_ret && dia_dev < dia_ret))))
    {
        return 0;
    }

    return 1;
}

void remover_locacao(int codigo_locacao)
{
    if (!locacao_existe(codigo_locacao))
    {
        printf("Locacao com o codigo especificado nao existe.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }

    FILE *arquivo_locacoes = fopen("locacoes.bin", "rb");
    if (arquivo_locacoes == NULL)
    {
        printf("Erro ao abrir o arquivo de locacoes para leitura.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }

    FILE *novo_arquivo = fopen("locacoes_temp.bin", "wb");
    if (novo_arquivo == NULL)
    {
        printf("Erro ao criar o arquivo temporario para escrita.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        fclose(arquivo_locacoes);
        return;
    }

    Locacao locacao_atual;

    while (fread(&locacao_atual, sizeof(Locacao), 1, arquivo_locacoes) == 1)
    {
        if (locacao_atual.codigo != codigo_locacao)
        {
            fwrite(&locacao_atual, sizeof(Locacao), 1, novo_arquivo);
        }
    }

    fclose(arquivo_locacoes);
    fclose(novo_arquivo);

    if (access("locacoes.bin", F_OK) != -1)
    {
        if (remove("locacoes.bin") == 0)
        {
            if (rename("locacoes_temp.bin", "locacoes.bin") != 0)
            {
                printf("Erro ao renomear o arquivo temporário.\n");
                printf("\nPressione Enter para continuar...\n");
                while (getch() != '\r')
                    ;
            }
            else
            {
            }
        }
        else
        {
            printf("Erro ao remover o arquivo original.\n");
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
}

void imprimirRelatorioLocacao(Locacao locacao, char data_entrega[], int dias_atraso, float valor_multa_por_dias, float valor_multa_porcentagem, float subtotal, float diaria, int desconto, float valor_aluguel)
{
    int lin1 = 1, col1 = 1, lin2 = 44, col2 = 60;
    setlocale(LC_ALL, "C");
    float seguro = 0;
    float desconto_cliente;

    if (locacao.seguro == 1)
    {
        seguro = 50;
    }
    else
    {
        seguro = 0;
    }

    if (desconto == 1)
    {
        desconto_cliente = subtotal * 0.10;
    }
    else
    {
        desconto_cliente = 0;
    }

    box(lin1, col1, lin2, col2);
    linhaCol(lin1 + 2, col1 + 25);
    printf("LOCA MAIS");
    linhaCol(lin1 + 4, col1 + 20);
    printf("RECIBO DE FATURAMENTO");
    linhaCol(lin1 + 6, col1 + 2);
    drawLine(1, 55, 1);
    linhaCol(lin1 + 8, col1 + 2);
    printf("Codigo do faturamento: %d", locacao.codigo);
    linhaCol(lin1 + 10, col1 + 2);
    printf("Codigo do cliente: %d", locacao.codigo_cliente);
    linhaCol(lin1 + 12, col1 + 2);
    printf("Data de retirada do veiculo: %s", locacao.data_retirada);
    linhaCol(lin1 + 14, col1 + 2);
    printf("Data de devolucao acordada: %s", locacao.data_devolucao);
    linhaCol(lin1 + 16, col1 + 2);
    printf("Data de Entrega: %s\n", data_entrega);
    linhaCol(lin1 + 18, col1 + 2);
    printf("Valor do seguro: %.2f\n", seguro);
    linhaCol(lin1 + 20, col1 + 2);
    printf("Valor da diaria do carro: %.2f", diaria);
    linhaCol(lin1 + 22, col1 + 2);
    printf("Dias de Atraso: %d\n", dias_atraso);
    linhaCol(lin1 + 24, col1 + 2);
    printf("Valor do aluguel: %.2f\n", valor_aluguel);
    linhaCol(lin1 + 26, col1 + 2);
    printf("Situacao da locacao: %s", locacao.status);
    linhaCol(lin1 + 28, col1 + 2);
    printf("Desconto: %.2f\n", desconto_cliente);
    linhaCol(lin1 + 30, col1 + 2);
    drawLine(1, 55, 1);
    linhaCol(lin1 + 32, col1 + 2);
    printf("Subtotal: %.2f\n", subtotal);
    linhaCol(lin1 + 34, col1 + 2);
    drawLine(1, 55, 1);
    linhaCol(lin1 + 36, col1 + 2);
    printf("Valor da multa por dias de atraso: %.2f\n", valor_multa_por_dias);
    linhaCol(lin1 + 38, col1 + 2);
    printf("Taxa de 5%% pelo atraso: %.2f\n", valor_multa_porcentagem);
    linhaCol(lin1 + 40, col1 + 2);
    drawLine(1, 55, 1);
    linhaCol(lin1 + 42, col1 + 2);
    printf("Total: %.2f\n", locacao.valor_aluguel);
    linhaCol(lin1 + 44, col1 + 2);

    setlocale(LC_ALL, "");

    Faturamento faturamento;
    faturamento.codigo = locacao.codigo;
    faturamento.codigo_cliente = locacao.codigo_cliente;
    strcpy(faturamento.data_retirada, locacao.data_retirada);
    strcpy(faturamento.data_devolucao, locacao.data_devolucao);
    strcpy(faturamento.data_entrega, data_entrega);
    faturamento.seguro = seguro;
    faturamento.diaria = diaria;
    faturamento.dias_atraso = dias_atraso;
    faturamento.alguel_acordado = valor_aluguel;
    strcpy(faturamento.status_locacao, "Finalizada");
    faturamento.desconto = desconto_cliente;
    faturamento.subtotal = subtotal;
    faturamento.valor_multa_por_dias = valor_multa_por_dias;
    faturamento.valor_multa_porcentagem = valor_multa_porcentagem;
    faturamento.valor_aluguel = locacao.valor_aluguel;

    salvarFaturamento(&faturamento);
}

void salvarFaturamento(const Faturamento *faturamento)
{
    FILE *arquivo_faturamento = fopen("faturamento.bin", "ab");

    if (arquivo_faturamento == NULL)
    {
        printf("Erro ao abrir o arquivo de faturamento para escrita.\n");
        fclose(arquivo_faturamento);
        return;
    }

    fwrite(faturamento, sizeof(Faturamento), 1, arquivo_faturamento);

    fclose(arquivo_faturamento);

    Total totais;

    FILE *arquivo_totais = fopen("totais.bin", "rb+");
    if (arquivo_totais == NULL)
    {
        arquivo_totais = fopen("totais.bin", "wb");
        totais.total = 0;
    }
    else
    {
        fread(&totais, sizeof(Total), 1, arquivo_totais);
    }

    totais.total += faturamento->valor_aluguel;

    fseek(arquivo_totais, 0, SEEK_SET);
    fwrite(&totais, sizeof(Total), 1, arquivo_totais);

    fclose(arquivo_totais);
}

int obter_proximo_codigo_locacao()
{
    FILE *arquivo_codigo = fopen("codigo_locacao.bin", "rb+");

    if (arquivo_codigo == NULL)
    {
        arquivo_codigo = fopen("codigo_locacao.bin", "wb+");
        CodigoLocacao valor_inicial = {1};
        fwrite(&valor_inicial, sizeof(CodigoLocacao), 1, arquivo_codigo);
        fclose(arquivo_codigo);
        return valor_inicial.codigo;
    }

    CodigoLocacao codigo_locacao;
    fread(&codigo_locacao, sizeof(CodigoLocacao), 1, arquivo_codigo);

    codigo_locacao.codigo++;

    rewind(arquivo_codigo);
    fwrite(&codigo_locacao, sizeof(CodigoLocacao), 1, arquivo_codigo);

    fclose(arquivo_codigo);

    return codigo_locacao.codigo;
}

void premiar_clientes(int codigo_cliente)
{
    FILE *file_clientes = fopen("clientes.bin", "rb");

    if (file_clientes == NULL)
    {
        printf("Erro ao abrir arquivo de clientes.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        fclose(file_clientes);
        return;
    }

    Cliente cliente;
    int lin1 = 12, col1 = 1, lin2 = 44, col2 = 60;

    while (fread(&cliente, sizeof(Cliente), 1, file_clientes) == 1)
    {
        if (cliente.codigo == codigo_cliente)
        {
            setlocale(LC_ALL, "C");
            box(lin1, col1, lin2, col2);
            linhaCol(lin1 + 2, col1 + 25);
            printf("LOCA MAIS");
            linhaCol(lin1 + 4, col1 + 22);
            printf("CLIENTE PREMIADO");
            linhaCol(lin1 + 6, col1 + 2);
            drawLine(1, 55, 1);
            linhaCol(lin1 + 8, col1 + 2);
            printf("O cliente ganhou 10%% de desconto no valor da locacao.");
            linhaCol(lin1 + 10, col1 + 2);
            drawLine(1, 55, 1);
            linhaCol(lin1 + 12, col1 + 2);
            printf("Codigo: %d", cliente.codigo);
            linhaCol(lin1 + 14, col1 + 2);
            printf("Nome: %s", cliente.nome);
            linhaCol(lin1 + 16, col1 + 2);
            printf("Endereco: %s", cliente.endereco);
            linhaCol(lin1 + 18, col1 + 2);
            printf("Telefone: %s", cliente.telefone);
            linhaCol(lin1 + 20, col1 + 2);
            printf("Situacao do cliente: %s", cliente.status);
            linhaCol(lin1 + 22, col1 + 2);
            printf("Pontos de fidelidade conquistados: %d", cliente.pontos_fidelidade);
            int pontos_restantes = debitar_pontos_fidelidade(cliente.codigo);
            linhaCol(lin1 + 24, col1 + 2);
            drawLine(1, 55, 1);
            linhaCol(lin1 + 26, col1 + 2);
            printf("Pontos de fidelidade debitados: 500");
            linhaCol(lin1 + 28, col1 + 2);
            drawLine(1, 55, 1);
            linhaCol(lin1 + 30, col1 + 2);
            printf("Pontos de fidelidade restantes: %d", pontos_restantes);
            linhaCol(lin1 + 44, col1 + 2);
            setlocale(LC_ALL, "");
            habilitaDesconto(cliente.codigo);
        }
    }
    fclose(file_clientes);
}

// funções principais
void cadastrar_locacao(const char *data_retirada, const char *data_devolucao, int seguro, int codigo_cliente, int num_ocupantes)
{
    int codigo_locacao = obter_proximo_codigo_locacao();
    if (locacao_existe(codigo_locacao))
    {
        printf("Codigo de locacao ja existe.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }

    FILE *arquivo_clientes = fopen("clientes.bin", "rb");
    if (arquivo_clientes == NULL)
    {
        printf("Erro ao abrir o arquivo de clientes para leitura.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        fclose(arquivo_clientes);
        return;
    }

    Cliente cliente_encontrado;

    int cliente_existe = 0;
    char cliente_status[11];

    while (fread(&cliente_encontrado, sizeof(Cliente), 1, arquivo_clientes) == 1)
    {
        if (cliente_encontrado.codigo == codigo_cliente)
        {
            cliente_existe = 1;
            strcpy(cliente_status, cliente_encontrado.status);
            break;
        }
    }

    if (!cliente_existe)
    {
        printf("Cliente nao encontrado.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        fclose(arquivo_clientes);
        return;
    }

    if (strcmp(cliente_status, "Desativado") == 0)
    {
        printf("\nNao e possivel cadastrar uma locacao para uma cliente que esta desativado.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        fclose(arquivo_clientes);
        return;
    }

    fclose(arquivo_clientes);

    int veiculo_disponivel = buscar_veiculo_disponivel("Disponivel", num_ocupantes);

    if (veiculo_disponivel == -1)
    {
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        fclose(arquivo_clientes);
        return;
    }

    FILE *arquivo_locacoes = fopen("locacoes.bin", "ab+");

    float diaria = diariaVeiculo(veiculo_disponivel);

    Locacao nova_locacao;
    nova_locacao.codigo = codigo_locacao;
    strcpy(nova_locacao.data_retirada, data_retirada);
    strcpy(nova_locacao.data_devolucao, data_devolucao);
    nova_locacao.seguro = seguro;
    nova_locacao.codigo_cliente = codigo_cliente;
    nova_locacao.codigo_veiculo = veiculo_disponivel;
    nova_locacao.quantidade_dias = diferenca_dias_entrada(data_retirada, data_devolucao);
    nova_locacao.valor_aluguel = calcular_valor_aluguel(nova_locacao.quantidade_dias, diaria, nova_locacao.seguro);
    strcpy(nova_locacao.status, "Ativa");

    printf("\nLocacao de numero %d realizada com sucesso\n", nova_locacao.codigo);

    int pontos_atuais = calcular_pontos_fidelidade(codigo_cliente, nova_locacao.quantidade_dias);
    int pontos_adicionados = nova_locacao.quantidade_dias * 10;

    printf("\nForam adicionados %d pontos de fidelidade ao cliente %s de codigo %d.\n", pontos_adicionados, cliente_encontrado.nome, cliente_encontrado.codigo);

    if (pontos_atuais >= 500)
    {
        premiar_clientes(cliente_encontrado.codigo);
    }

    fwrite(&nova_locacao, sizeof(Locacao), 1, arquivo_locacoes);

    fclose(arquivo_locacoes);

    FILE *arquivo_veiculos = fopen("veiculos.bin", "rb+");
    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veículos para atualização de status.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        fclose(arquivo_clientes);
        fclose(arquivo_veiculos);
        fclose(arquivo_clientes);
        return;
    }

    atualizar_status_veiculo(veiculo_disponivel, "Alugado");

    fclose(arquivo_veiculos);
    fclose(arquivo_clientes);
    fclose(arquivo_locacoes);

    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
}

void dar_baixa_locacao(int codigo_locacao)
{
    float valor_diaria, valor_multa_porcentagem, valor_multa_por_dias = 0, valor_aluguel;
    int codigo_veiculo;

    FILE *arquivo_locacoes = fopen("locacoes.bin", "rb+");
    if (arquivo_locacoes == NULL)
    {
        printf("Erro ao abrir o arquivo de locacoes para leitura/escrita.\n");
        printf("Pressione Enter para continuar...");
        while (getch() != '\r')
            ;
        return;
    }

    int index_locacao = -1;
    Locacao locacao_atual;

    while (fread(&locacao_atual, sizeof(Locacao), 1, arquivo_locacoes) == 1)
    {
        if (locacao_atual.codigo == codigo_locacao)
        {
            codigo_veiculo = locacao_atual.codigo_veiculo;
            index_locacao = ftell(arquivo_locacoes) / sizeof(Locacao) - 1;
            break;
        }
    }

    if (index_locacao == -1)
    {
        printf("Locacao nao encontrada.\n");
        fclose(arquivo_locacoes);
        printf("Pressione Enter para continuar...");
        while (getch() != '\r')
            ;
        fclose(arquivo_locacoes);
        return;
    }

    FILE *arquivo_veiculos = fopen("veiculos.bin", "rb");
    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veiculos para leitura.\n");
        printf("Pressione Enter para continuar...");
        while (getch() != '\r')
            ;
        fclose(arquivo_locacoes);
        fclose(arquivo_veiculos);
        return;
    }

    valor_diaria = diariaVeiculo(codigo_veiculo);

    if (valor_diaria == -1)
    {
        printf("Veiculo nao encontrado.\n");
        printf("Pressione Enter para continuar...");
        while (getch() != '\r')
            ;
        fclose(arquivo_locacoes);
        fclose(arquivo_veiculos);
        return;
    }

    fseek(arquivo_locacoes, index_locacao * sizeof(Locacao), SEEK_SET);
    Locacao locacao_modificada;

    if (fread(&locacao_modificada, sizeof(Locacao), 1, arquivo_locacoes) != 1)
    {
        printf("Erro ao ler a locacao do arquivo.\n");
        printf("Pressione Enter para continuar...");
        while (getch() != '\r')
            ;
        fclose(arquivo_locacoes);
        fclose(arquivo_veiculos);
        return;
    }

    valor_aluguel = calcular_valor_aluguel(locacao_modificada.quantidade_dias, valor_diaria, locacao_modificada.seguro);
    locacao_modificada.valor_aluguel = valor_aluguel;

    char data_entrega[11];
    do
    {
        printf("Digite a data de entrega (formato: dd/mm/yyyy): ");
        if (scanf("%s", data_entrega) != 1 || !validarFormatoData(data_entrega) || !validar_data_entrega(locacao_modificada.data_devolucao, data_entrega))
        {
            printf("Entrada invalida. Formato da data incorreto ou intervalo invalido.\n");
            while (getchar() != '\n')
                ;
        }
        else
        {
            break;
        }
    } while (1);
    float subtotal = locacao_modificada.valor_aluguel;
    int desconto = temDesconto(locacao_modificada.codigo_cliente);

    if (desconto == 1)
    {
        subtotal -= subtotal * 0.10;
    }

    int dif_dias = diferenca_dias_entrada(locacao_modificada.data_devolucao, data_entrega);
    if (dif_dias > 0)
    {
        valor_multa_por_dias = (dif_dias * 30) + (dif_dias * valor_diaria);
        valor_multa_porcentagem = (locacao_modificada.valor_aluguel + valor_multa_por_dias) * 0.05;
        locacao_modificada.valor_aluguel += valor_multa_por_dias + valor_multa_porcentagem;
    }
    else
    {
        valor_multa_por_dias = 0.0;
        valor_multa_porcentagem = 0.0;
    }

    strcpy(locacao_modificada.status, "Finalizada");

    fwrite(&locacao_modificada, sizeof(Locacao), 1, arquivo_locacoes);

    fclose(arquivo_locacoes);
    fclose(arquivo_veiculos);

    atualizar_status_veiculo(locacao_modificada.codigo_veiculo, "Disponivel");

    imprimirRelatorioLocacao(locacao_modificada, data_entrega, dif_dias, valor_multa_por_dias, valor_multa_porcentagem, subtotal, valor_diaria, desconto, valor_aluguel);

    desabilitaDesconto(locacao_modificada.codigo_cliente);

    remover_locacao(codigo_locacao);

    printf("\n");
    printf("\n");
    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
}

void exibir_todas_locacoes_ativas()
{
    int aux = 0;

    FILE *arquivo_locacoes = fopen("locacoes.bin", "rb");

    if (arquivo_locacoes == NULL)
    {
        printf("Erro ao abrir o arquivo de locacoes para leitura.\n");
        return;
    }

    Locacao locacao_atual;
    FILE *arquivo_clientes = fopen("clientes.bin", "rb");

    if (arquivo_clientes == NULL)
    {
        printf("Erro ao abrir o arquivo de clientes para leitura.\n");
        fclose(arquivo_clientes);
    }

    Cliente cliente_atual;

    FILE *arquivo_veiculos = fopen("veiculos.bin", "rb");

    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veículos para leitura.\n");
        printf("Pressione Enter para continuar...");
        while (getch() != '\r')
            ;
        return;
    }

    Veiculo veiculo_atual;

    int lin1 = 1, col1 = 1, lin2 = 56, col2 = 60;
    while (fread(&locacao_atual, sizeof(Locacao), 1, arquivo_locacoes) == 1)
    {
        setlocale(LC_ALL, "C");
        if (strcmp(locacao_atual.status, "Ativa") == 0)
        {
            box(lin1, col1, lin2, col2);
            linhaCol(lin1 + 2, col1 + 25);
            printf("LOCA MAIS");
            linhaCol(lin1 + 4, col1 + 20);
            printf("LOCACAO DO CLIENTE");
            linhaCol(lin1 + 6, col1 + 2);
            drawLine(1, 56, 1);
            linhaCol(lin1 + 8, col1 + 2);
            printf("Codigo da Locacao: %d\n", locacao_atual.codigo);
            linhaCol(lin1 + 10, col1 + 2);
            printf("Data de Retirada: %s\n", locacao_atual.data_retirada);
            linhaCol(lin1 + 12, col1 + 2);
            printf("Data de Devolucao: %s\n", locacao_atual.data_devolucao);
            linhaCol(lin1 + 14, col1 + 2);
            printf("Seguro: %s\n", (locacao_atual.seguro == 1) ? "Sim" : "Nao");
            linhaCol(lin1 + 16, col1 + 2);
            printf("Quantidade de Dias alugado: %d\n", locacao_atual.quantidade_dias);
            linhaCol(lin1 + 18, col1 + 2);
            printf("Valor do Aluguel: %.2f\n", locacao_atual.valor_aluguel);
            linhaCol(lin1 + 20, col1 + 2);
            printf("Situacao da locacao: %s\n", locacao_atual.status);
            linhaCol(lin1 + 22, col1 + 2);
            drawLine(1, 56, 1);
            linhaCol(lin1 + 24, col1 + 2);
            printf("Dados do cliente: ");
            linhaCol(lin1 + 26, col1 + 2);
            fseek(arquivo_clientes, 0, SEEK_SET);
            while (fread(&cliente_atual, sizeof(Cliente), 1, arquivo_clientes) == 1)
            {
                if (cliente_atual.codigo == locacao_atual.codigo_cliente)
                {
                    printf("Codigo: %d\n", cliente_atual.codigo);
                    linhaCol(lin1 + 28, col1 + 2);
                    printf("Endereco: %s\n", cliente_atual.endereco);
                    linhaCol(lin1 + 30, col1 + 2);
                    printf("Telefone: %s\n", cliente_atual.telefone);
                    linhaCol(lin1 + 32, col1 + 2);
                    printf("Pontos de fidelidade: %d\n", cliente_atual.pontos_fidelidade);
                    linhaCol(lin1 + 34, col1 + 2);
                    puts((cliente_atual.desconto == 1) ? "Desconto: 10% de desconto nessa locacao." : "Desconto: Nao");
                    linhaCol(lin1 + 36, col1 + 2);
                }
            }
            drawLine(1, 56, 1);
            linhaCol(lin1 + 38, col1 + 2);
            printf("Dados do veiculo: ");
            fseek(arquivo_veiculos, 0, SEEK_SET);
            linhaCol(lin1 + 40, col1 + 2);
            while (fread(&veiculo_atual, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
            {
                if (veiculo_atual.codigo == locacao_atual.codigo_veiculo)
                {
                    printf("Codigo: %d\n", veiculo_atual.codigo);
                    linhaCol(lin1 + 42, col1 + 2);
                    printf("Descricao: %s\n", veiculo_atual.descricao);
                    linhaCol(lin1 + 44, col1 + 2);
                    printf("Modelo: %s\n", veiculo_atual.modelo);
                    linhaCol(lin1 + 46, col1 + 2);
                    printf("Cor: %s\n", veiculo_atual.cor);
                    linhaCol(lin1 + 48, col1 + 2);
                    printf("Placa: %s\n", veiculo_atual.placa);
                    linhaCol(lin1 + 50, col1 + 2);
                    printf("Diaria: %.2f\n", veiculo_atual.valor_diaria);
                    linhaCol(lin1 + 52, col1 + 2);
                    printf("Numero de ocupantes: %d\n", veiculo_atual.ocupantes);
                    linhaCol(lin1 + 54, col1 + 2);
                    printf("Status: %s\n", veiculo_atual.status);
                    linhaCol(lin1 + 56, col1 + 2);
                }
            }
            aux = 1;

            for (int i = 0; i < 2; i++)
            {
                printf("\n");
            }

            lin1 = lin2 + 2;
            lin2 = lin1 + 56;
        }
    }

    if (aux == 0)
    {
        printf("Nao tem nenhuma locacao ativa.");
    }

    printf("\n");
    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;

    fclose(arquivo_locacoes);
    fclose(arquivo_clientes);
    fclose(arquivo_veiculos);
}

void mostrar_locacoes_ativas_cliente(int codigo_cliente)
{
    int aux = 0;
    FILE *arquivo_locacoes = fopen("locacoes.bin", "rb");
    if (arquivo_locacoes == NULL)
    {
        printf("Erro ao abrir o arquivo de locacoes para leitura.\n");
        printf("Pressione Enter para continuar...");
        while (getch() != '\r')
            ;
        return;
    }

    FILE *arquivo_clientes = fopen("clientes.bin", "rb");

    if (arquivo_clientes == NULL)
    {
        printf("Erro ao abrir o arquivo de clientes para leitura.\n");
        fclose(arquivo_clientes);
    }

    Cliente cliente_atual;

    FILE *arquivo_veiculos = fopen("veiculos.bin", "rb");

    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veículos para leitura.\n");
        printf("Pressione Enter para continuar...");
        while (getch() != '\r')
            ;
        return;
    }

    Veiculo veiculo_atual;

    if (!cliente_existe(codigo_cliente))
    {

        printf("Cliente nao encontrado.\n");
        printf("Pressione Enter para continuar...");
        while (getch() != '\r')
            ;
        return;
    }

    Locacao locacao_atual;
    int lin1 = 1, col1 = 1, lin2 = 56, col2 = 60;

    while (fread(&locacao_atual, sizeof(Locacao), 1, arquivo_locacoes) == 1)
    {
        setlocale(LC_ALL, "C");

        if (locacao_atual.codigo_cliente == codigo_cliente)
        {
            if (strcmp(locacao_atual.status, "Ativa") == 0)
            {
                box(lin1, col1, lin2, col2);
                linhaCol(lin1 + 2, col1 + 25);
                printf("LOCA MAIS");
                linhaCol(lin1 + 4, col1 + 20);
                printf("LOCACAO DO CLIENTE");
                linhaCol(lin1 + 6, col1 + 2);
                drawLine(1, 56, 1);
                linhaCol(lin1 + 8, col1 + 2);
                printf("Codigo da Locacao: %d\n", locacao_atual.codigo);
                linhaCol(lin1 + 10, col1 + 2);
                printf("Data de Retirada: %s\n", locacao_atual.data_retirada);
                linhaCol(lin1 + 12, col1 + 2);
                printf("Data de Devolucao: %s\n", locacao_atual.data_devolucao);
                linhaCol(lin1 + 14, col1 + 2);
                printf("Seguro: %s\n", (locacao_atual.seguro == 1) ? "Sim" : "Nao");
                linhaCol(lin1 + 16, col1 + 2);
                printf("Quantidade de Dias alugado: %d\n", locacao_atual.quantidade_dias);
                linhaCol(lin1 + 18, col1 + 2);
                printf("Valor do Aluguel: %.2f\n", locacao_atual.valor_aluguel);
                linhaCol(lin1 + 20, col1 + 2);
                printf("Situacao da locacao: %s\n", locacao_atual.status);
                linhaCol(lin1 + 22, col1 + 2);
                drawLine(1, 56, 1);
                linhaCol(lin1 + 24, col1 + 2);
                printf("Dados do cliente: ");
                linhaCol(lin1 + 26, col1 + 2);
                fseek(arquivo_clientes, 0, SEEK_SET);
                while (fread(&cliente_atual, sizeof(Cliente), 1, arquivo_clientes) == 1)
                {
                    if (cliente_atual.codigo == locacao_atual.codigo_cliente)
                    {
                        printf("Codigo: %d\n", cliente_atual.codigo);
                        linhaCol(lin1 + 28, col1 + 2);
                        printf("Endereco: %s\n", cliente_atual.endereco);
                        linhaCol(lin1 + 30, col1 + 2);
                        printf("Telefone: %s\n", cliente_atual.telefone);
                        linhaCol(lin1 + 32, col1 + 2);
                        printf("Pontos de fidelidade: %d\n", cliente_atual.pontos_fidelidade);
                        linhaCol(lin1 + 34, col1 + 2);
                        puts((cliente_atual.desconto == 1) ? "Desconto: 10% de desconto nessa locacao." : "Desconto: Nao");
                        linhaCol(lin1 + 36, col1 + 2);
                    }
                }
                drawLine(1, 56, 1);
                linhaCol(lin1 + 38, col1 + 2);
                printf("Dados do veiculo: ");
                fseek(arquivo_veiculos, 0, SEEK_SET);
                linhaCol(lin1 + 40, col1 + 2);
                while (fread(&veiculo_atual, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
                {
                    if (veiculo_atual.codigo == locacao_atual.codigo_veiculo)
                    {
                        printf("Codigo: %d\n", veiculo_atual.codigo);
                        linhaCol(lin1 + 42, col1 + 2);
                        printf("Descricao: %s\n", veiculo_atual.descricao);
                        linhaCol(lin1 + 44, col1 + 2);
                        printf("Modelo: %s\n", veiculo_atual.modelo);
                        linhaCol(lin1 + 46, col1 + 2);
                        printf("Cor: %s\n", veiculo_atual.cor);
                        linhaCol(lin1 + 48, col1 + 2);
                        printf("Placa: %s\n", veiculo_atual.placa);
                        linhaCol(lin1 + 50, col1 + 2);
                        printf("Diaria: %.2f\n", veiculo_atual.valor_diaria);
                        linhaCol(lin1 + 52, col1 + 2);
                        printf("Numero de ocupantes: %d\n", veiculo_atual.ocupantes);
                        linhaCol(lin1 + 54, col1 + 2);
                        printf("Status: %s\n", veiculo_atual.status);
                        linhaCol(lin1 + 56, col1 + 2);
                    }
                }
                for (int i = 0; i < 2; i++)
                {
                    printf("\n");
                }

                lin1 = lin2 + 2;
                lin2 = lin1 + 56;
                aux = 1;
            }
        }
    }
    setlocale(LC_ALL, " ");
    if (aux == 0)
    {
        printf("O cliente nao tem locacoes ativas.\n");
    }

    printf("\n");
    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;

    fclose(arquivo_locacoes);
    fclose(arquivo_clientes);
    fclose(arquivo_veiculos);
}

void exibirTotalGeral()
{
    Total totais;

    FILE *arquivo_totais = fopen("totais.bin", "rb");
    if (arquivo_totais == NULL)
    {
        printf("Total Geral: R$ 0.00\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }

    fread(&totais, sizeof(Total), 1, arquivo_totais);

    int lin1 = 1, col1 = 1, lin2 = 10, col2 = 50;

    setlocale(LC_ALL, "C");
    box(lin1, col1, lin2, col2);
    linhaCol(lin1 + 2, col1 + 20);
    printf("LOCA MAIS");
    linhaCol(lin1 + 4, col1 + 10);
    printf("TOTAL FATURADO ATE O MOMENTO");
    linhaCol(lin1 + 6, col1 + 2);
    drawLine(1, 45, 1);
    linhaCol(lin1 + 8, col1 + 2);
    printf("RECEITA TOTAL: R$ %.2f\n", totais.total);
    linhaCol(lin1 + 10, col1 + 2);
    setlocale(LC_ALL, "");

    printf("\n");
    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
    fclose(arquivo_totais);
}

void exibirRelatorioFaturamento()
{
    int aux = 0;
    FILE *arquivo_faturamento = fopen("faturamento.bin", "rb");
    if (arquivo_faturamento == NULL)
    {
        printf("Erro ao abrir o arquivo de faturamento para leitura.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }

    Faturamento faturamento;

    int lin1 = 1, col1 = 1, lin2 = 44, col2 = 65;

    while (fread(&faturamento, sizeof(Faturamento), 1, arquivo_faturamento) == 1)
    {
        setlocale(LC_ALL, "C");

        box(lin1, col1, lin2, col2);
        linhaCol(lin1 + 2, col1 + 25);
        printf("LOCA MAIS");
        linhaCol(lin1 + 4, col1 + 20);
        printf("RECIBO DE FATURAMENTO");
        linhaCol(lin1 + 6, col1 + 2);
        drawLine(1, 60, 1);
        linhaCol(lin1 + 8, col1 + 2);
        printf("Codigo do faturamento: %d", faturamento.codigo);
        linhaCol(lin1 + 10, col1 + 2);
        printf("Codigo do cliente: %d", faturamento.codigo_cliente);
        linhaCol(lin1 + 12, col1 + 2);
        printf("Data de retirada do veiculo: %s", faturamento.data_retirada);
        linhaCol(lin1 + 14, col1 + 2);
        printf("Data de devolucao acordada: %s", faturamento.data_devolucao);
        linhaCol(lin1 + 16, col1 + 2);
        printf("Data de Entrega: %s\n", faturamento.data_entrega);
        linhaCol(lin1 + 18, col1 + 2);
        printf("Valor do seguro: %.2f\n", faturamento.seguro);
        linhaCol(lin1 + 20, col1 + 2);
        printf("Valor da diaria do carro: %.2f", faturamento.diaria);
        linhaCol(lin1 + 22, col1 + 2);
        printf("Dias de Atraso: %d\n", faturamento.dias_atraso);
        linhaCol(lin1 + 24, col1 + 2);
        printf("Valor do aluguel: %.2f\n", faturamento.alguel_acordado);
        linhaCol(lin1 + 26, col1 + 2);
        printf("Situacao da locacao: %s", faturamento.status_locacao);
        linhaCol(lin1 + 28, col1 + 2);
        printf("Desconto: %.2f\n", faturamento.desconto);
        linhaCol(lin1 + 30, col1 + 2);
        drawLine(1, 60, 1);
        linhaCol(lin1 + 32, col1 + 2);
        printf("Subtotal: %.2f\n", faturamento.subtotal);
        linhaCol(lin1 + 34, col1 + 2);
        drawLine(1, 60, 1);
        linhaCol(lin1 + 36, col1 + 2);
        printf("Valor da multa por dias de atraso: %.2f\n", faturamento.valor_multa_por_dias);
        linhaCol(lin1 + 38, col1 + 2);
        printf("Taxa de 5%% pelo atraso: %.2f\n", faturamento.valor_multa_porcentagem);
        linhaCol(lin1 + 40, col1 + 2);
        drawLine(1, 60, 1);
        linhaCol(lin1 + 42, col1 + 2);
        printf("Total: %.2f\n", faturamento.valor_aluguel);
        linhaCol(lin1 + 44, col1 + 2);
        setlocale(LC_ALL, "");
        aux = 1;
        for (int i = 0; i < 2; i++)
        {
            printf("\n");
        }

        lin1 = lin2 + 2;
        lin2 = lin1 + 44;
    }
    setlocale(LC_ALL, " ");

    if (aux == 0)
    {
        printf("\nAinda nao temos valores faturados.\n");
    }

    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
    fclose(arquivo_faturamento);
    return;
}

void exibirRelatorioFaturamentoCliente(int codigo_cliente)
{
    int aux = 0;
    FILE *arquivo_faturamento = fopen("faturamento.bin", "rb");
    if (arquivo_faturamento == NULL)
    {
        printf("Erro ao abrir o arquivo de faturamento para leitura.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }

    Faturamento faturamento;

    int lin1 = 1, col1 = 1, lin2 = 44, col2 = 65;

    while (fread(&faturamento, sizeof(Faturamento), 1, arquivo_faturamento) == 1)
    {
        if (strcmp(faturamento.status_locacao, "Finalizada") == 0)
        {
            if (faturamento.codigo_cliente == codigo_cliente)
            {
                setlocale(LC_ALL, "C");

                box(lin1, col1, lin2, col2);
                linhaCol(lin1 + 2, col1 + 25);
                printf("LOCA MAIS");
                linhaCol(lin1 + 4, col1 + 20);
                printf("RECIBO DE FATURAMENTO");
                linhaCol(lin1 + 6, col1 + 2);
                drawLine(1, 55, 1);
                linhaCol(lin1 + 8, col1 + 2);
                printf("Codigo do faturamento: %d", faturamento.codigo);
                linhaCol(lin1 + 10, col1 + 2);
                printf("Codigo do cliente: %d", faturamento.codigo_cliente);
                linhaCol(lin1 + 12, col1 + 2);
                printf("Data de retirada do veiculo: %s", faturamento.data_retirada);
                linhaCol(lin1 + 14, col1 + 2);
                printf("Data de devolucao acordada: %s", faturamento.data_devolucao);
                linhaCol(lin1 + 16, col1 + 2);
                printf("Data de Entrega: %s\n", faturamento.data_entrega);
                linhaCol(lin1 + 18, col1 + 2);
                printf("Valor do seguro: %.2f\n", faturamento.seguro);
                linhaCol(lin1 + 20, col1 + 2);
                printf("Valor da diaria do carro: %.2f", faturamento.diaria);
                linhaCol(lin1 + 22, col1 + 2);
                printf("Dias de Atraso: %d\n", faturamento.dias_atraso);
                linhaCol(lin1 + 24, col1 + 2);
                printf("Valor do aluguel: %.2f\n", faturamento.alguel_acordado);
                linhaCol(lin1 + 26, col1 + 2);
                printf("Situacao da locacao: %s", faturamento.status_locacao);
                linhaCol(lin1 + 28, col1 + 2);
                printf("Desconto: %.2f\n", faturamento.desconto);
                linhaCol(lin1 + 30, col1 + 2);
                drawLine(1, 55, 1);
                linhaCol(lin1 + 32, col1 + 2);
                printf("Subtotal: %.2f\n", faturamento.subtotal);
                linhaCol(lin1 + 34, col1 + 2);
                drawLine(1, 55, 1);
                linhaCol(lin1 + 36, col1 + 2);
                printf("Valor da multa por dias de atraso: %.2f\n", faturamento.valor_multa_por_dias);
                linhaCol(lin1 + 38, col1 + 2);
                printf("Taxa de 5%% pelo atraso: %.2f\n", faturamento.valor_multa_porcentagem);
                linhaCol(lin1 + 40, col1 + 2);
                drawLine(1, 55, 1);
                linhaCol(lin1 + 42, col1 + 2);
                printf("Total: %.2f\n", faturamento.valor_aluguel);
                linhaCol(lin1 + 44, col1 + 2);
                setlocale(LC_ALL, "");
                aux = 1;
                for (int i = 0; i < 2; i++)
                {
                    printf("\n");
                }

                lin1 = lin2 + 2;
                lin2 = lin1 + 44;
            }
        }
    }
    setlocale(LC_ALL, " ");

    if (aux == 0)
    {
        printf("\nO cliente ainda nao tem locacoes faturadas.\n");
    }

    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
    fclose(arquivo_faturamento);
    return;
}
