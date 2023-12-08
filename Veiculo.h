// veiculo.h
#ifndef VEICULO_H
#define VEICULO_H

typedef struct
{
    int codigo;
    char descricao[100];
    char modelo[50];
    char cor[20];
    char placa[10];
    float valor_diaria;
    int ocupantes;
    char status[20];
} Veiculo;

void cadastrar_veiculo(Veiculo veiculos[], int *num_veiculos, int codigo, const char *descricao, const char *modelo, const char *cor, const char *placa, float valor_diaria, int ocupantes, const char *status);
void exibir_veiculos();
void pesquisar_veiculo(int codigo_veiculo);
void remover_veiculo(int codigo);
void exibir_veiculos_alugados();
void remover_veiculo_da_manutencao(int codigo_veiculo);
void colocar_veiculo_em_manutencao(int codigo_veiculo);
void exibir_veiculos_em_manutencao();
void exibir_veiculos_disponiveis();

#endif