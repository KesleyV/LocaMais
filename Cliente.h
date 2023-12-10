// cliente.h
#include "Constants.h"

#ifndef CLIENTE_H
#define CLIENTE_H


typedef struct
{
    int codigo;
    char nome[50];
    char endereco[150];
    char telefone[13];
    int pontos_fidelidade;
    int desconto;
    char status[20];
} Cliente;

void cadastrar_cliente(const char *nome, const char *endereco, const char *telefone);
void exibir_clientes();
void pesquisar_cliente(int codigo_cliente);
void desativar_cliente(int codigo_cliente);
int cliente_existe(int codigo_cliente);
void reativar_cliente(int codigo_cliente);
int verificaLocacao(int codigo_cliente);

#endif
