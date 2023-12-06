#include <stdio.h>
#include <string.h>
#include "locacao.h"
#include "veiculo.h"
#include "Cliente.h"
#include <time.h>
#include "constants.h"

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
    if (arquivo_locacoes == NULL)
    {
        printf("Erro ao abrir o arquivo de locações para leitura.\n");
        return 0;
    }

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

void exibir_cliente_por_codigo(int codigo_cliente)
{
    FILE *arquivo_clientes = fopen("clientes.bin", "rb");
    if (arquivo_clientes == NULL)
    {
        printf("Erro ao abrir o arquivo de clientes para leitura.\n");
        return;
    }

    Cliente cliente_atual;

    while (fread(&cliente_atual, sizeof(Cliente), 1, arquivo_clientes) == 1)
    {
        if (cliente_atual.codigo == codigo_cliente)
        {
            printf("Codigo: %d\n", cliente_atual.codigo);
            printf("Nome: %s\n", cliente_atual.nome);
            printf("Endereco: %s\n", cliente_atual.endereco);
            printf("Telefone: %s\n", cliente_atual.telefone);
            fclose(arquivo_clientes);
            return;
        }
    }

    fclose(arquivo_clientes);
    printf("Cliente nao encontrado.\n");
}

void exibir_veiculo_por_codigo(int codigo_veiculo)
{
    FILE *arquivo_veiculos = fopen("veiculos.bin", "rb");
    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veículos para leitura.\n");
        return;
    }

    Veiculo veiculo_atual;

    while (fread(&veiculo_atual, sizeof(Veiculo), 1, arquivo_veiculos) == 1)
    {
        if (veiculo_atual.codigo == codigo_veiculo)
        {
            printf("Codigo: %d\n", veiculo_atual.codigo);
            printf("Descricao: %s\n", veiculo_atual.descricao);
            printf("Modelo: %s\n", veiculo_atual.modelo);
            printf("Cor: %s\n", veiculo_atual.cor);
            printf("Placa: %s\n", veiculo_atual.placa);
            printf("Valor Diaria: %.2f\n", veiculo_atual.valor_diaria);
            printf("Ocupantes: %d\n", veiculo_atual.ocupantes);
            printf("Status: %s\n", veiculo_atual.status);
            fclose(arquivo_veiculos);
            return;
        }
    }

    fclose(arquivo_veiculos);
    printf("Veiculo nao encontrado.\n");
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

// funções principais
void cadastrar_locacao(Cliente clientes[], int num_clientes, Veiculo veiculos[], int num_veiculos, Locacao locacoes[], int *num_locacoes, int codigo_locacao, const char *data_retirada, const char *data_devolucao, int seguro, int codigo_cliente, int num_ocupantes)
{
    if (locacao_existe(codigo_locacao))
    {
        printf("Código de locacao ja existe.\n");
        return;
    }

    FILE *arquivo_clientes = fopen("clientes.bin", "rb");
    if (arquivo_clientes == NULL)
    {
        printf("Erro ao abrir o arquivo de clientes para leitura.\n");
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
        return;
    }

    int veiculo_disponivel = buscar_veiculo_disponivel("Disponivel", num_ocupantes);

    if (veiculo_disponivel == -1)
    {
        return;
    }

    FILE *arquivo_locacoes = fopen("locacoes.bin", "ab+");
    if (arquivo_locacoes == NULL)
    {
        printf("Erro ao abrir o arquivo de locações para escrita.\n");
        return;
    }

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

    printf("Locação realizada com sucesso\n");

    calcular_pontos_fidelidade(codigo_cliente, nova_locacao.quantidade_dias);

    fwrite(&nova_locacao, sizeof(Locacao), 1, arquivo_locacoes);

    fclose(arquivo_locacoes);

    FILE *arquivo_veiculos = fopen("veiculos.bin", "rb+");
    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veículos para atualização de status.\n");
        return;
    }

    atualizar_status_veiculo(veiculo_disponivel, "Alugado");

    fclose(arquivo_veiculos);

    (*num_locacoes)++;
}

void dar_baixa_locacao(Locacao locacoes[], int *num_locacoes, Veiculo veiculos[], Cliente clientes[], int *num_veiculos, int *num_clientes)
{
    int codigo_locacao;
    float valor_diaria;
    int codigo_veiculo;

    printf("Digite o codigo da locacao a dar baixa: ");
    scanf("%d", &codigo_locacao);

    FILE *arquivo_locacoes = fopen("locacoes.bin", "rb+");
    if (arquivo_locacoes == NULL)
    {
        printf("Erro ao abrir o arquivo de locações para leitura/escrita.\n");
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
    printf("%d\n", codigo_veiculo);

    if (index_locacao == -1)
    {
        printf("Locacao nao encontrada.\n");
        fclose(arquivo_locacoes);
        return;
    }

    FILE *arquivo_veiculos = fopen("veiculos.bin", "rb");
    if (arquivo_veiculos == NULL)
    {
        printf("Erro ao abrir o arquivo de veículos para leitura.\n");
        fclose(arquivo_locacoes);
        return;
    }

    valor_diaria = diariaVeiculo(codigo_veiculo);

    if (valor_diaria == -1)
    {
        printf("Veículo não encontrado.\n");
        fclose(arquivo_locacoes);
        return;
    }

    fseek(arquivo_locacoes, index_locacao * sizeof(Locacao), SEEK_SET);
    Locacao locacao_modificada;

    if (fread(&locacao_modificada, sizeof(Locacao), 1, arquivo_locacoes) != 1)
    {
        printf("Erro ao ler a locação do arquivo.\n");
        fclose(arquivo_locacoes);
        return;
    }

    locacao_modificada.valor_aluguel = calcular_valor_aluguel(locacao_modificada.quantidade_dias, valor_diaria, locacao_modificada.seguro);

    char data_entrega[11];
    printf("Digite a data de entrega (formato: dd/mm/yyyy): ");
    scanf("%s", data_entrega);

    int dif_dias = diferenca_dias(locacao_modificada.data_devolucao, data_entrega);
    if (dif_dias > 0)
    {
        float dias_atraso = (dif_dias * 30) + (dif_dias * valor_diaria);
        float valor_multa = (locacao_modificada.valor_aluguel + dias_atraso) * 0.05;
        locacao_modificada.valor_aluguel += dias_atraso + valor_multa;
    }

    fwrite(&locacao_modificada, sizeof(Locacao), 1, arquivo_locacoes);

    fclose(arquivo_locacoes);

    atualizar_status_veiculo(locacao_modificada.codigo_veiculo, "Disponivel");

    printf("Entrega do veículo realizada com sucesso. O valor total foi de %.2f.\n", locacao_modificada.valor_aluguel);

    remover_locacao(locacoes, num_locacoes, codigo_locacao);
}

void exibir_todas_locacoes()
{

    FILE *arquivo_locacoes = fopen("locacoes.bin", "rb");

    if (arquivo_locacoes == NULL)
    {
        printf("Erro ao abrir o arquivo de locacoes para leitura.\n");
        return;
    }

    Cliente clientes[MAX_CLIENTES];
    Veiculo veiculos[MAX_VEICULOS];
    int num_clientes, num_veiculos;

    carregar_clientes(clientes, &num_clientes);
    carregar_veiculos(veiculos, &num_veiculos);

    printf("\nLOCACOES:\n");

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

        printf("Seguro: %s\n", (locacao_atual.seguro == 1) ? "Sim" : "Não");
        printf("Quantidade de Dias: %d\n", locacao_atual.quantidade_dias);
        printf("Valor do Aluguel: %.2f\n", locacao_atual.valor_aluguel);
        printf("--------------------------\n");
    }

    fclose(arquivo_locacoes);
}

void mostrar_locacoes_cliente(Locacao locacoes[], int num_locacoes, int codigo_cliente)
{
    printf("Locações do cliente:\n");

    FILE *arquivo_locacoes = fopen("locacoes.bin", "rb");
    if (arquivo_locacoes == NULL)
    {
        printf("Erro ao abrir o arquivo de locações para leitura.\n");
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

            printf("Seguro: %s\n", (locacao_atual.seguro == 1) ? "Sim" : "Não");
            printf("Quantidade de Dias: %d\n", locacao_atual.quantidade_dias);
            printf("Valor do Aluguel: %.2f\n", locacao_atual.valor_aluguel);
            printf("--------------------------\n");
        }
    }

    fclose(arquivo_locacoes);
}

void premiar_clientes()
{
    FILE *file_clientes = fopen("clientes.bin", "rb");

    if (file_clientes == NULL)
    {
        printf("Erro ao abrir arquivo de clientes.\n");
        return;
    }

    Cliente cliente;

    printf("Clientes premiados com kit da LocaMais:\n");

    while (fread(&cliente, sizeof(Cliente), 1, file_clientes) == 1)
    {
        if (cliente.pontos_fidelidade >= 500)
        {
            printf("Código: %d, Nome: %s, Pontos de Fidelidade: %d\n", cliente.codigo, cliente.nome, cliente.pontos_fidelidade);
            debitar_pontos_fidelidade(cliente.codigo);
        }
    }

    fclose(file_clientes);
}

void remover_locacao(Locacao locacoes[], int *num_locacoes, int codigo_locacao)
{
    FILE *arquivo_entrada = fopen("locacoes.bin", "rb");
    if (arquivo_entrada == NULL)
    {
        printf("Erro ao abrir o arquivo de locações para leitura.\n");
        return;
    }

    FILE *arquivo_temporario = fopen("temp_locacoes.bin", "wb");
    if (arquivo_temporario == NULL)
    {
        printf("Erro ao abrir o arquivo temporário para escrita.\n");
        fclose(arquivo_entrada);
        return;
    }

    Locacao locacao_atual;

    while (fread(&locacao_atual, sizeof(Locacao), 1, arquivo_entrada) == 1)
    {
        if (locacao_atual.codigo != codigo_locacao)
        {
            fwrite(&locacao_atual, sizeof(Locacao), 1, arquivo_temporario);
        }
    }

    fclose(arquivo_entrada);
    fclose(arquivo_temporario);

    remove("locacoes.bin");
    rename("temp_locacoes.bin", "locacoes.bin");

    carregar_locacoes(locacoes, num_locacoes);
}

void salvar_locacoes(Locacao locacoes[], int num_locacoes)
{
    FILE *arquivo = fopen("locacoes.bin", "wb");
    if (arquivo != NULL)
    {
        fwrite(locacoes, sizeof(Locacao), num_locacoes, arquivo);
        fclose(arquivo);
    }
}

void carregar_locacoes(Locacao locacoes[], int *num_locacoes)
{
    FILE *arquivo = fopen("locacoes.bin", "rb");
    if (arquivo != NULL)
    {
        fread(locacoes, sizeof(Locacao), MAX_LOCACOES, arquivo);
        fclose(arquivo);
    }
}
