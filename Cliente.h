// cliente.h
#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct
{
    int codigo;
    char nome[50];
    char endereco[150];
    char telefone[13];
    int pontos_fidelidade;
} Cliente;

void cadastrar_cliente(int codigo, const char *nome, const char *endereco, const char *telefone);
void exibir_clientes();
void pesquisar_cliente(int codigo_cliente);
void remover_cliente(int codigo_cliente);
int cliente_existe(int codigo_cliente);

#endif
