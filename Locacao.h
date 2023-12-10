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
    char status[11];
} Locacao;


typedef struct
{
    char data_retirada[11];
    char data_devolucao[11];
    char data_entrega[11];
    float seguro;
    float diaria;
    int dias_atraso;
    float valor_multa_por_dias;
    float valor_multa_porcentagem;
    float subtotal;
    float desconto;
    float valor_aluguel;
    float alguel_acordado;
    int codigo;
    char status_locacao[11];
    int codigo_cliente;
} Faturamento;

void cadastrar_locacao(const char *data_retirada, const char *data_devolucao, int seguro, int codigo_cliente, int num_ocupantes);
void dar_baixa_locacao(int codigo_locacao);
void mostrar_locacoes_ativas_cliente(int codigo_cliente);
int diferenca_dias(const char *data_retirada, const char *data_devolucao);
void calcular_pontos_fidelidade(int codigo_cliente, int diarias);
void remover_locacao(int index_locacao);
void exibir_todas_locacoes_ativas();
void exibirTotalGeral();
void exibirRelatorioFaturamento();
void salvarFaturamento(const Faturamento *faturamento);
void exibirRelatorioFaturamentoCliente(int codigo_cliente);
void premiar_clientes(int codigo_cliente);
#endif
