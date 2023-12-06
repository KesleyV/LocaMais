#include "Cliente.h"
#include "Veiculo.h"
#include "Locacao.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    Cliente clientes[MAX_CLIENTES];
    Veiculo veiculos[MAX_VEICULOS];
    Locacao locacoes[MAX_LOCACOES];

    int num_clientes = 0;
    int num_veiculos = 0;
    int num_locacoes = 0;
    int opcao = 0;

    while (opcao != 4)
    {
        printf("\nMENU:\n");
        printf("1. Menu Cliente\n");
        printf("2. Menu Veiculo\n");
        printf("3. Menu Locacao\n");
        printf("4. Sair\n");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
        {
            printf("\nMENU CLIENTE:\n");
            printf("1. Cadastrar Cliente\n");
            printf("2. Exibir todos os clientes\n");
            printf("3. Pesquisar Cliente\n");
            printf("4. Remover cliente\n");
            printf("5. Voltar\n");

            int opcao_cliente;
            scanf("%d", &opcao_cliente);

            switch (opcao_cliente)
            {
            case 1:
            {
                int codigo;
                char nome[50];
                char endereco[150];
                char telefone[11];

                printf("Digite o codigo do cliente: ");
                scanf("%d", &codigo);
                getchar();

                printf("Digite o nome do cliente: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';

                printf("Digite o endereco do cliente: ");
                fgets(endereco, sizeof(endereco), stdin);
                endereco[strlen(endereco) - 1] = '\0';

                printf("Digite o telefone do cliente: ");
                fgets(telefone, sizeof(telefone), stdin);
                telefone[strcspn(telefone, "\n")] = '\0';

                cadastrar_cliente(codigo, nome, endereco, telefone);
                break;
            }
            case 2:
            {
                exibir_clientes();
                break;
            }
            case 3:
            {
                int codigo_cliente;
                printf("Digite o código do cliente: ");
                scanf("%d", &codigo_cliente);
                pesquisar_cliente(codigo_cliente);
                break;
            }
            case 4:
            {
                int codigo_cliente;
                printf("Digite o código do cliente que deseja excluir (verifique se o mesmo nao tem nenhuma pendencia): ");
                scanf("%d", &codigo_cliente);
                remover_cliente(codigo_cliente);
                break;
            }
            case 5:
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }
            break;
        }
        case 2:
        {
            printf("\nMENU VEICULO:\n");
            printf("1. Cadastrar Veiculo\n");
            printf("2. Exibir todos os veiculos\n");
            printf("3. Pesquisar Veiculo\n");
            printf("4. Remover veiculo\n");
            printf("5. Exibir veiculos alugados\n");
            printf("6. Voltar\n");

            int opcao_veiculo;
            scanf("%d", &opcao_veiculo);

            switch (opcao_veiculo)
            {
            case 1:
            {
                int codigo;
                char descricao[50];
                char modelo[50];
                char cor[20];
                char placa[10];
                float valor_diaria;
                int ocupantes;
                char status[20];

                printf("Digite o codigo do veiculo: ");
                scanf("%d", &codigo);
                getchar();

                printf("Digite a descricao do veiculo: ");
                fgets(descricao, sizeof(descricao), stdin);
                descricao[strcspn(descricao, "\n")] = '\0';

                printf("Digite o modelo do veiculo: ");
                fgets(modelo, sizeof(modelo), stdin);
                modelo[strcspn(modelo, "\n")] = '\0';

                printf("Digite a cor do veiculo: ");
                fgets(cor, sizeof(cor), stdin);
                cor[strcspn(cor, "\n")] = '\0';

                printf("Digite a placa do veiculo: ");
                fgets(placa, sizeof(placa), stdin);
                placa[strcspn(placa, "\n")] = '\0';

                printf("Digite o valor da diaria do veiculo: ");
                scanf("%f", &valor_diaria);
                getchar();

                printf("Digite a quantidade de ocupantes do veiculo: ");
                scanf("%d", &ocupantes);
                getchar();

                printf("Digite o status atual do veiculo (Disponivel, Alugado ou Manutencao): ");
                fgets(status, sizeof(status), stdin);
                status[strcspn(status, "\n")] = '\0';

                cadastrar_veiculo(veiculos, &num_veiculos, codigo, descricao, modelo, cor, placa, valor_diaria, ocupantes, status);
                break;
            }
            case 2:
            {
                exibir_veiculos();
                break;
            }
            case 3:
            {
                int codigo_veiculo;
                printf("Digite o código do veículo: ");
                scanf("%d", &codigo_veiculo);
                pesquisar_veiculo(codigo_veiculo);
                break;
            }
            case 4:
            {
                int codigo_veiculo;
                printf("Digite o código do veículo (verifique se o veiculo nao tem nenhuma pendencia): ");
                scanf("%d", &codigo_veiculo);
                remover_veiculo(codigo_veiculo);
                break;
            }
            case 5:
            {
                exibir_veiculos_alugados();
                break;
            }
            case 6:
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }
            break;
        }
        case 3:
        {
            printf("\nMENU LOCACAO:\n");
            printf("1. Cadastrar Locacao\n");
            printf("2. Exibir todas as locacoes\n");
            printf("3. Mostrar Locacoes de Cliente\n");
            printf("4. Dar Baixa em Locacao\n");
            printf("5. Premiar clientes\n");
            printf("6. Voltar\n");

            int opcao_locacao;
            scanf("%d", &opcao_locacao);

            switch (opcao_locacao)
            {
            case 1:
            {
                int codigo_locacao;
                char data_retirada[11];
                char data_devolucao[11];
                int seguro;
                int codigo_cliente;
                int numero_ocupantes;

                printf("Digite o codigo da locação: ");
                scanf("%d", &codigo_locacao);

                printf("Digite a data de retirada (formato: dd/mm/yyyy): ");
                scanf("%s", data_retirada);

                printf("Digite a data de devolucao (formato: dd/mm/yyyy): ");
                scanf("%s", data_devolucao);

                printf("Deseja contratar seguro? (1 para Sim, 0 para Nao): ");
                scanf("%d", &seguro);

                printf("Digite o codigo do cliente: ");
                scanf("%d", &codigo_cliente);

                printf("Digite o numero de ocupantes: ");
                scanf("%d", &numero_ocupantes);

                cadastrar_locacao(clientes, num_clientes, veiculos, num_veiculos, locacoes, &num_locacoes, codigo_locacao, data_retirada, data_devolucao, seguro, codigo_cliente, numero_ocupantes);
                break;
            }
            case 2:
            {
                exibir_todas_locacoes();
                break;
            }
            case 3:
            {
                int codigo_cliente;
                printf("Digite o código do cliente: ");
                scanf("%d", &codigo_cliente);
                mostrar_locacoes_cliente(locacoes, num_locacoes, codigo_cliente);
                break;
            }
            case 4:
            {
                dar_baixa_locacao(locacoes, &num_locacoes, veiculos, clientes, &num_veiculos, &num_clientes);
                break;
            }
            case 5:
            {
                premiar_clientes();
                break;
            }
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }
            break;
        }
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    }
    return 0;
}
