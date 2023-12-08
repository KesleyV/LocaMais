// locacao.h
#include "Cliente.h"
#include "Veiculo.h"
#ifndef LOCACAO_H
#define LOCACAO_H

typedef struct
{
    int codigo;
    char data_retirada[11];
    char data_devolucao[11];
    int seguro;
    int quantidade_dias;
    int codigo_cliente;
    int codigo_veiculo;
    float valor_aluguel;
} Locacao;

typedef struct
{
    float total;
} Total;

void cadastrar_locacao(int codigo_locacao, const char *data_retirada, const char *data_devolucao, int seguro, int codigo_cliente, int num_ocupantes);
void dar_baixa_locacao(Locacao locacoes[], int *num_locacoes, Veiculo veiculos[], Cliente clientes[], int *num_veiculos, int *num_clientes);
void mostrar_locacoes_cliente(Locacao locacoes[], int num_locacoes, int codigo_cliente);
int diferenca_dias(const char *data_retirada, const char *data_devolucao);
void calcular_pontos_fidelidade(int codigo_cliente, int diarias);
void remover_locacao(int index_locacao);
void exibir_todas_locacoes();
void premiar_clientes();
void exibirTotalGeral();
void exibirRelatorioFaturamento();

#endif
