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
int diferenca_dias(const char *data_retirada, const char *data_devolucao)
{
    struct tm tm_retirada = {0};
    struct tm tm_devolucao = {0};

    sscanf(data_retirada, "%d/%d/%d", &tm_retirada.tm_mday, &tm_retirada.tm_mon, &tm_retirada.tm_year);
    sscanf(data_devolucao, "%d/%d/%d", &tm_devolucao.tm_mday, &tm_devolucao.tm_mon, &tm_devolucao.tm_year);

    tm_retirada.tm_year -= 1900;
    tm_devolucao.tm_year -= 1900;

    time_t t_retirada = mktime(&tm_retirada);
    time_t t_devolucao = mktime(&tm_devolucao);

    double diferenca_segundos = difftime(t_devolucao, t_retirada);
    return diferenca_segundos / (60 * 60 * 24);
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

char *exibir_cliente_por_codigo(int codigo_cliente)
{
    static char informacoesCliente[500]; // Tamanho ajustado conforme necessário
    FILE *arquivo_clientes = fopen("clientes.bin", "rb");

    if (arquivo_clientes == NULL)
    {
        printf("Erro ao abrir o arquivo de clientes para leitura.\n");
        strcpy(informacoesCliente, "Erro ao abrir o arquivo de clientes.");
        fclose(arquivo_clientes);
        return informacoesCliente;
    }

    Cliente cliente_atual;
    int cliente_encontrado = 0;

    while (fread(&cliente_atual, sizeof(Cliente), 1, arquivo_clientes) == 1)
    {
        if (cliente_atual.codigo == codigo_cliente)
        {
            snprintf(informacoesCliente, sizeof(informacoesCliente),
                     "Codigo: %d\nNome: %s\nEndereco: %s\nTelefone: %s",
                     cliente_atual.codigo, cliente_atual.nome, cliente_atual.endereco, cliente_atual.telefone);

            cliente_encontrado = 1;
            break;
        }
    }

    fclose(arquivo_clientes);

    if (!cliente_encontrado)
    {
        snprintf(informacoesCliente, sizeof(informacoesCliente), "Cliente nao encontrado.");
    }

    return informacoesCliente;
}

char *exibir_veiculo_por_codigo(int codigo_veiculo)
{
    static char informacoesVeiculo[500]; // Tamanho ajustado conforme necessário
    FILE *arquivo_veiculos = fopen("veiculos.bin", "rb");

    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veículos para leitura.\n");
        strcpy(informacoesVeiculo, "Erro ao abrir o arquivo de veículos.");
        return informacoesVeiculo;
    }

    Veiculo veiculo_atual;
    int veiculo_encontrado = 0;

    while (fread(&veiculo_atual, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (veiculo_atual.codigo == codigo_veiculo)
        {
            snprintf(informacoesVeiculo, sizeof(informacoesVeiculo),
                     "Codigo: %d\nDescricao: %s\nModelo: %s\nCor: %s\nPlaca: %s\nValor Diaria: %.2f\nOcupantes: %d\nStatus: %s",
                     veiculo_atual.codigo, veiculo_atual.descricao, veiculo_atual.modelo,
                     veiculo_atual.cor, veiculo_atual.placa, veiculo_atual.valor_diaria,
                     veiculo_atual.ocupantes, veiculo_atual.status);

            veiculo_encontrado = 1;
            break;
        }
    }

    fclose(arquivo_veiculos);

    if (!veiculo_encontrado)
    {
        snprintf(informacoesVeiculo, sizeof(informacoesVeiculo), "Veiculo nao encontrado.");
    }

    return informacoesVeiculo;
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

void calcular_pontos_fidelidade(int codigo_cliente, int diarias)
{
    FILE *arquivo_clientes = fopen("clientes.bin", "rb+");

    if (arquivo_clientes == NULL)
    {
        printf("Erro ao abrir arquivo de clientes.\n");
        return;
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
            return;
        }
    }

    fclose(arquivo_clientes);
    printf("Cliente com codigo %d nao encontrado.\n", codigo_cliente);
}

void debitar_pontos_fidelidade(int codigo_cliente)
{
    FILE *arquivo_clientes = fopen("clientes.bin", "rb+");

    if (arquivo_clientes == NULL)
    {
        printf("Erro ao abrir arquivo de clientes.\n");
        return;
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
            return;
        }
    }

    fclose(arquivo_clientes);
    printf("Cliente com codigo %d nao encontrado.\n", codigo_cliente);
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

void imprimirRelatorioLocacao(Locacao locacao, char data_entrega[], int dias_atraso, int valor_multa_por_dias, float valor_multa_porcentagem, float subtotal)
{
    int seguro = 0;
    if (locacao.seguro == 1)
    {
        seguro = 50;
    }
    else
    {
        seguro = 0;
    }
    drawLine(1, 50, 1);
    printf("\nData de devolucao acordada: %s\n", locacao.data_devolucao);
    printf("Data de Entrega: %s\n", data_entrega);
    printf("Valor do seguro: %d\n", seguro);
    printf("Dias de Atraso: %d\n", dias_atraso);
    printf("Valor da multa por dias de atraso: %d\n", valor_multa_por_dias);
    printf("Taxa de 5%% pelo atraso: %.2f\n", valor_multa_porcentagem);
    printf("Subtotal: %.2f\n", subtotal);
    printf("Valor Total: %.2f\n", locacao.valor_aluguel);
    drawLine(1, 50, 1);
}

void salvarFaturamento(Locacao locacao, char *data_entrega, int dias_atraso, int valor_multa_por_dias, float valor_multa_porcentagem, float subtotal, float total)
{
    FILE *arquivo_faturamento = fopen("faturamento.bin", "ab");

    if (arquivo_faturamento == NULL)
    {
        printf("Erro ao abrir o arquivo de faturamento para escrita.\n");
        return;
    }

    fwrite(&locacao, sizeof(Locacao), 1, arquivo_faturamento);
    fwrite(data_entrega, sizeof(char), strlen(data_entrega) + 1, arquivo_faturamento);
    fwrite(&dias_atraso, sizeof(int), 1, arquivo_faturamento);
    fwrite(&valor_multa_por_dias, sizeof(int), 1, arquivo_faturamento);
    fwrite(&valor_multa_porcentagem, sizeof(float), 1, arquivo_faturamento);
    fwrite(&subtotal, sizeof(float), 1, arquivo_faturamento);
    fwrite(&total, sizeof(float), 1, arquivo_faturamento);

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

    totais.total += locacao.valor_aluguel;

    fseek(arquivo_totais, 0, SEEK_SET);
    fwrite(&totais, sizeof(Total), 1, arquivo_totais);

    fclose(arquivo_totais);
}

// funções principais
void cadastrar_locacao(int codigo_locacao, const char *data_retirada, const char *data_devolucao, int seguro, int codigo_cliente, int num_ocupantes)
{
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
        return;
    }

    Cliente cliente_encontrado;
    int cliente_existe = 0;

    while (fread(&cliente_encontrado, sizeof(Cliente), 1, arquivo_clientes) == 1)
    {
        if (cliente_encontrado.codigo == codigo_cliente)
        {
            cliente_existe = 1;
            break;
        }
    }

    fclose(arquivo_clientes);

    if (!cliente_existe)
    {
        printf("Cliente nao encontrado.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        fclose(arquivo_clientes);
        return;
    }

    int veiculo_disponivel = buscar_veiculo_disponivel("Disponivel", num_ocupantes);

    if (veiculo_disponivel == -1)
    {
        printf("\nNao temos veiculos disponiveis...\n");
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
    nova_locacao.quantidade_dias = diferenca_dias(data_retirada, data_devolucao);
    nova_locacao.valor_aluguel = calcular_valor_aluguel(nova_locacao.quantidade_dias, diaria, nova_locacao.seguro);

    printf("Locacao realizada com sucesso\n");

    calcular_pontos_fidelidade(codigo_cliente, nova_locacao.quantidade_dias);

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

void dar_baixa_locacao(Locacao locacoes[], int *num_locacoes, Veiculo veiculos[], Cliente clientes[], int *num_veiculos, int *num_clientes)
{
    int codigo_locacao;
    float valor_diaria, valor_multa_porcentagem;
    int codigo_veiculo, valor_multa_por_dias = 0;

    printf("Digite o codigo da locacao a dar baixa: ");
    scanf("%d", &codigo_locacao);

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
        return;
    }

    locacao_modificada.valor_aluguel = calcular_valor_aluguel(locacao_modificada.quantidade_dias, valor_diaria, locacao_modificada.seguro);

    char data_entrega[11];
    do
    {
        printf("Digite a data de entrega (formato: dd/mm/yyyy): ");
        if (scanf("%s", data_entrega) != 1 || !validarFormatoData(data_entrega) || !validar_data_entrega(locacao_modificada.data_devolucao, data_entrega))
        {
            printf("Entrada invalida. Formato da data incorreto ou intervalo inválido.\n");
            while (getchar() != '\n')
                ;
        }
        else
        {
            break;
        }
    } while (1);
    float subtotal = locacao_modificada.valor_aluguel;
    int dif_dias = diferenca_dias(locacao_modificada.data_devolucao, data_entrega);
    if (dif_dias > 0)
    {
        valor_multa_por_dias = (dif_dias * 30) + (dif_dias * valor_diaria);
        valor_multa_porcentagem = (locacao_modificada.valor_aluguel + valor_multa_por_dias) * 0.05;
        locacao_modificada.valor_aluguel += valor_multa_por_dias + valor_multa_porcentagem;
    }

    fwrite(&locacao_modificada, sizeof(Locacao), 1, arquivo_locacoes);

    fclose(arquivo_locacoes);
    fclose(arquivo_veiculos);

    atualizar_status_veiculo(locacao_modificada.codigo_veiculo, "Disponivel");

    imprimirRelatorioLocacao(locacao_modificada, data_entrega, dif_dias, valor_multa_por_dias, valor_multa_porcentagem, subtotal);

    salvarFaturamento(locacao_modificada, data_entrega, dif_dias, valor_multa_por_dias, valor_multa_porcentagem, subtotal, locacao_modificada.valor_aluguel);

    remover_locacao(codigo_locacao);

    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;
}

void exibir_todas_locacoes()
{
    int aux = 0;

    FILE *arquivo_locacoes = fopen("locacoes.bin", "rb");

    if (arquivo_locacoes == NULL)
    {
        printf("Erro ao abrir o arquivo de locacoes para leitura.\n");
        return;
    }

    Locacao locacao_atual;

    while (fread(&locacao_atual, sizeof(Locacao), 1, arquivo_locacoes) == 1)
    {
        printf("\nCodigo da Locacao: %d\n", locacao_atual.codigo);
        printf("Data de Retirada: %s\n", locacao_atual.data_retirada);
        printf("Data de Devolucao: %s\n", locacao_atual.data_devolucao);

        printf("Cliente: ");
        exibir_cliente_por_codigo(locacao_atual.codigo_cliente);

        printf("Veiculo: ");
        exibir_veiculo_por_codigo(locacao_atual.codigo_veiculo);

        printf("Seguro: %s\n", (locacao_atual.seguro == 1) ? "Sim" : "Nao");
        printf("Quantidade de Dias: %d\n", locacao_atual.quantidade_dias);
        printf("Valor do Aluguel: %.2f\n", locacao_atual.valor_aluguel);
        drawLine(1, 40, 1);
        aux = 1;
    }

    if (aux == 0)
    {
        printf("Nao tem nenhuma locacao feita.");
    }

    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;

    fclose(arquivo_locacoes);
}

void mostrar_locacoes_cliente(Locacao locacoes[], int num_locacoes, int codigo_cliente)
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

    if (!cliente_existe(codigo_cliente))
    {

        printf("Cliente nao encontrado.\n");
        printf("Pressione Enter para continuar...");
        while (getch() != '\r')
            ;
        return;
    }

    Locacao locacao_atual;

    while (fread(&locacao_atual, sizeof(Locacao), 1, arquivo_locacoes) == 1)
    {
        if (locacao_atual.codigo_cliente == codigo_cliente)
        {
            printf("\nCodigo da Locacao: %d\n", locacao_atual.codigo);
            printf("Data de Retirada: %s\n", locacao_atual.data_retirada);
            printf("Data de Devolucao: %s\n", locacao_atual.data_devolucao);

            printf("Cliente: ");
            exibir_cliente_por_codigo(locacao_atual.codigo_cliente);

            printf("Veiculo: ");
            exibir_veiculo_por_codigo(locacao_atual.codigo_veiculo);

            printf("Seguro: %s\n", (locacao_atual.seguro == 1) ? "Sim" : "Nao");
            printf("Quantidade de Dias: %d\n", locacao_atual.quantidade_dias);
            printf("Valor do Aluguel: %.2f\n", locacao_atual.valor_aluguel);
            drawLine(1, 40, 1);
            fclose(arquivo_locacoes);
            aux = 1;
        }
    }
    if (aux == 0)
    {
        printf("O cliente nao tem locacoes.\n");
    }
    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;

    fclose(arquivo_locacoes);
}

void premiar_clientes()
{
    int aux = 0;
    FILE *file_clientes = fopen("clientes.bin", "rb");

    if (file_clientes == NULL)
    {
        printf("Erro ao abrir arquivo de clientes.\n");
        printf("\nPressione Enter para continuar...\n");
        while (getch() != '\r')
            ;
        return;
    }

    Cliente cliente;

    while (fread(&cliente, sizeof(Cliente), 1, file_clientes) == 1)
    {
        if (cliente.pontos_fidelidade >= 500)
        {
            printf("Codigo: %d, Nome: %s, Pontos de Fidelidade: %d\n", cliente.codigo, cliente.nome, cliente.pontos_fidelidade);
            drawLine(1, 30, 1);
            debitar_pontos_fidelidade(cliente.codigo);
            aux = 1;
        }
    }

    if (aux == 0)
    {
        printf("Nenhum cliente atingiu o numero de pontos para ser premiado.\n");
    }
    printf("\nPressione Enter para continuar...\n");
    while (getch() != '\r')
        ;

    fclose(file_clientes);
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

    printf("Total Geral: R$ %.2f\n", totais.total);

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

    Locacao locacao;
    char data_entrega[11];
    int dias_atraso = 0, valor_multa_por_dias = 0;
    float valor_multa_porcentagem = 0, subtotal = 0, total = 0;

    printf("\nRelatorio de Faturamento:\n");

    while (fread(&locacao, sizeof(Locacao), 1, arquivo_faturamento) == 1 &&
           fread(data_entrega, sizeof(char), sizeof(data_entrega), arquivo_faturamento) == sizeof(data_entrega) &&
           fread(&dias_atraso, sizeof(int), 1, arquivo_faturamento) == 1 &&
           fread(&valor_multa_por_dias, sizeof(int), 1, arquivo_faturamento) == 1 &&
           fread(&valor_multa_porcentagem, sizeof(float), 1, arquivo_faturamento) == 1 &&
           fread(&subtotal, sizeof(float), 1, arquivo_faturamento) == 1 &&
           fread(&total, sizeof(float), 1, arquivo_faturamento) == 1)
    {
        drawLine(1, 50, 1);
        printf("\nCodigo da Locacao: %d\n", locacao.codigo);
        printf("Valor do Aluguel: %.2f\n", locacao.valor_aluguel);
        printf("Data de Entrega: %s\n", data_entrega);
        printf("Dias de Atraso: %d\n", dias_atraso);
        printf("Valor da Multa por Dias de Atraso: %d\n", valor_multa_por_dias);
        printf("Taxa de 5%% pelo Atraso: %.2f\n", valor_multa_porcentagem);
        printf("Subtotal: %.2f\n", subtotal);
        printf("Total: %.2f\n", total);
        drawLine(1, 50, 1);
        aux = 1;
    }
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
