#include "menu.h"
#include "Locacao.h"
#include "Cliente.h"
#include "Veiculo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Cliente clientes[MAX_CLIENTES];
Veiculo veiculos[MAX_VEICULOS];
Locacao locacoes[MAX_LOCACOES];

int num_clientes = 0;
int num_veiculos = 0;
int num_locacoes = 0;

// funções auxiliares
int validarFormatoData(const char *data)
{
    int dia, mes, ano;
    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) == 3)
    {
        if (ano >= 1000 && ano <= 9999)
        {
            return 1;
        }
    }
    return 0;
}

int validarIntervaloDatas(const char *data_retirada, const char *data_devolucao)
{
    int dia_ret, mes_ret, ano_ret, dia_dev, mes_dev, ano_dev;

    sscanf(data_retirada, "%d/%d/%d", &dia_ret, &mes_ret, &ano_ret);
    sscanf(data_devolucao, "%d/%d/%d", &dia_dev, &mes_dev, &ano_dev);

    if (ano_dev < ano_ret || (ano_dev == ano_ret && (mes_dev < mes_ret || (mes_dev == mes_ret && dia_dev <= dia_ret))))
    {
        return 0;
    }

    if (ano_dev == ano_ret && mes_dev == mes_ret && dia_dev - dia_ret < 1)
    {
        return 0;
    }

    return 1;
}

int validarTelefone(const char *telefone)
{
    int telefoneValido = 1;

    if (strlen(telefone) == 0 || isspace(telefone[0]) || strcspn(telefone, "\n") > 11)
    {
        telefoneValido = 0;
    }
    else
    {
        for (size_t i = 0; i < strlen(telefone); i++)
        {
            if (!isdigit(telefone[i]))
            {
                telefoneValido = 0;
                break;
            }
        }
    }

    return telefoneValido;
}

// função principal
void menu()
{
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
                char telefone[13];

                do
                {
                    printf("Digite o codigo do cliente: ");
                    if (scanf("%d", &codigo) != 1)
                    {
                        printf("Entrada invalida. O codigo deve ser um numero inteiro.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                while (getchar() != '\n')
                    ;
                do
                {
                    printf("Digite o nome do cliente: ");
                    fgets(nome, sizeof(nome), stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    int nomeValido = 1;
                    for (int i = 0; nome[i] != '\0'; i++)
                    {
                        if (!isalpha(nome[i]) && !isspace(nome[i]))
                        {
                            nomeValido = 0;
                            break;
                        }
                    }

                    if (strlen(nome) == 0 || isspace(nome[0]) || strcspn(nome, "\n") > 48 || !nomeValido)
                    {
                        printf("Entrada invalida. O nome nao pode estar em branco, exceder o numero maximo de caracteres e deve conter apenas letras.\n");
                        printf("Pressione ENTER para tentar novamente.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);
                do
                {
                    printf("Digite o endereco do cliente: ");
                    fgets(endereco, sizeof(endereco), stdin);
                    endereco[strcspn(endereco, "\n")] = '\0';

                    if (strlen(endereco) == 0 || isspace(endereco[0]) || strcspn(endereco, "\n") > 148)
                    {
                        printf("Entrada invalida. O endereco nao pode estar em branco e nem exceder o numero maximo de caracteres.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                do
                {
                    printf("Digite o telefone do cliente: ");
                    fgets(telefone, sizeof(telefone), stdin);
                    telefone[strcspn(telefone, "\n")] = '\0';

                    if (!validarTelefone(telefone))
                    {
                        printf("Entrada invalida. O telefone nao pode estar em branco, exceder o numero maximo de caracteres e deve conter apenas numeros de 0 a 9.\n");
                        printf("Pressione ENTER para tentar novamente.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);
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
                do
                {
                    printf("Digite o codigo do cliente: ");
                    if (scanf("%d", &codigo_cliente) != 1)
                    {
                        printf("Entrada invalida. O codigo deve ser um numero inteiro.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                while (getchar() != '\n')
                    ;
                pesquisar_cliente(codigo_cliente);
                break;
            }
            case 4:
            {
                int codigo_cliente;
                do
                {
                    printf("Digite o codigo do cliente: ");
                    if (scanf("%d", &codigo_cliente) != 1)
                    {
                        printf("Entrada invalida. O codigo deve ser um numero inteiro.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                while (getchar() != '\n')
                    ;
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
            printf("6. Exibir veiculos em manutencao\n");
            printf("7. Retirar veiculo da manutencao\n");
            printf("8. Enviar veiculo para manutencao\n");
            printf("9. Voltar\n");

            int opcao_veiculo;
            scanf("%d", &opcao_veiculo);

            switch (opcao_veiculo)
            {
            case 1:
            {
                int codigo;
                char descricao[100];
                char modelo[50];
                char cor[20];
                char placa[10];
                float valor_diaria;
                int ocupantes;
                char status[20];

                do
                {
                    printf("Digite o codigo do veiculo: ");
                    if (scanf("%d", &codigo) != 1)
                    {
                        printf("Entrada invalida. O codigo deve ser um numero inteiro.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                while (getchar() != '\n')
                    ;

                do
                {
                    printf("Digite a descricao do veiculo: ");
                    fgets(descricao, sizeof(descricao), stdin);
                    descricao[strcspn(descricao, "\n")] = '\0';

                    if (strlen(descricao) == 0 || isspace(descricao[0]) || strcspn(descricao, "\n") > 98)
                    {
                        printf("Entrada invalida. A descricao nao pode estar em branco e nem exceder o numero maximo de caracteres.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                do
                {
                    printf("Digite o modelo do veiculo: ");
                    fgets(modelo, sizeof(modelo), stdin);
                    modelo[strcspn(modelo, "\n")] = '\0';

                    if (strlen(modelo) == 0 || isspace(modelo[0]) || strcspn(modelo, "\n") > 48)
                    {
                        printf("Entrada invalida. O modelo nao pode estar em branco e nem exceder o numero maximo de caracteres.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                do
                {
                    printf("Digite a cor do veiculo: ");
                    fgets(cor, sizeof(cor), stdin);
                    cor[strcspn(cor, "\n")] = '\0';

                    if (strlen(cor) == 0 || isspace(cor[0]) || strcspn(cor, "\n") > 18)
                    {
                        printf("Entrada invalida. A cor nao pode estar em branco e nem exceder o numero maximo de caracteres.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                do
                {
                    printf("Digite a placa do veiculo: ");
                    fgets(placa, sizeof(placa), stdin);
                    placa[strcspn(placa, "\n")] = '\0';

                    if (strlen(placa) == 0 || isspace(placa[0]) || strcspn(placa, "\n") > 8)
                    {
                        printf("Entrada invalida. A placa nao pode estar em branco e nem exceder o numero maximo de caracteres.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                do
                {
                    printf("Digite o valor da diaria do veiculo: ");
                    if (scanf("%f", &valor_diaria) != 1)
                    {
                        printf("Entrada invalida. Insira um valor numerico para a diaria.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                while (getchar() != '\n')
                    ;

                do
                {
                    printf("Digite a quantidade de ocupantes do veiculo: ");
                    if (scanf("%d", &ocupantes) != 1)
                    {
                        printf("Entrada invalida. Insira um valor inteiro para quantidade de ocupantes.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                while (getchar() != '\n')
                    ;
                do
                {
                    printf("Digite o status do veiculo (Disponivel ou Manutencao): ");
                    fgets(status, sizeof(status), stdin);
                    status[strcspn(status, "\n")] = '\0';

                    if (strcmp(status, "Disponivel") == 0 || strcmp(status, "Manutencao") == 0)
                    {
                        break;
                    }
                    else
                    {
                        printf("Entrada invalida. O status deve ser 'Disponivel' ou 'Manutencao'.\n");
                        while (getchar() != '\n')
                            ;
                    }
                } while (1);

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
                do
                {
                    printf("Digite o codigo do veiculo: ");
                    if (scanf("%d", &codigo_veiculo) != 1)
                    {
                        printf("Entrada invalida. O codigo deve ser um numero inteiro.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                while (getchar() != '\n')
                    ;
                pesquisar_veiculo(codigo_veiculo);
                break;
            }
            case 4:
            {
                int codigo_veiculo;
                do
                {
                    printf("Digite o codigo do veiculo: ");
                    if (scanf("%d", &codigo_veiculo) != 1)
                    {
                        printf("Entrada invalida. O codigo deve ser um numero inteiro.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                while (getchar() != '\n')
                    ;
                remover_veiculo(codigo_veiculo);
                break;
            }
            case 5:
            {
                exibir_veiculos_alugados();
                break;
            }
            case 6:
            {
                exibir_veiculos_em_manutencao();
                break;
            }
            case 7:
            {
                int codigo_veiculo;
                do
                {
                    printf("Digite o codigo do veiculo: ");
                    if (scanf("%d", &codigo_veiculo) != 1)
                    {
                        printf("Entrada invalida. O codigo deve ser um numero inteiro.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                while (getchar() != '\n')
                    ;
                remover_veiculo_da_manutencao(codigo_veiculo);
                break;
            }
            case 8:
            {
                int codigo_veiculo;
                do
                {
                    printf("Digite o codigo do veiculo: ");
                    if (scanf("%d", &codigo_veiculo) != 1)
                    {
                        printf("Entrada invalida. O codigo deve ser um numero inteiro.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                while (getchar() != '\n')
                    ;
                colocar_veiculo_em_manutencao(codigo_veiculo);
                break;
            }
            case 9:
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

                do
                {
                    printf("Digite o codigo da locacao: ");
                    if (scanf("%d", &codigo_locacao) != 1)
                    {
                        printf("Entrada invalida. O codigo deve ser um numero inteiro.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                while (getchar() != '\n')
                    ;

                do
                {
                    printf("Digite a data de retirada (formato: dd/mm/yyyy): ");
                    if (scanf("%s", data_retirada) != 1 || !validarFormatoData(data_retirada))
                    {
                        printf("Entrada invalida. Formato da data incorreto.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                while (getchar() != '\n')
                    ;
                do
                {
                    printf("Digite a data de devolucao (formato: dd/mm/yyyy): ");
                    if (scanf("%s", data_devolucao) != 1 || !validarFormatoData(data_devolucao) || !validarIntervaloDatas(data_retirada, data_devolucao))
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

                while (getchar() != '\n')
                    ;
                do
                {
                    printf("Deseja contratar seguro? (1 para Sim, 0 para Nao): ");
                    if (scanf("%d", &seguro) != 1 || (seguro != 0 && seguro != 1))
                    {
                        printf("Entrada invalida. Insira 1 para Sim ou 0 para Nao.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                while (getchar() != '\n')
                    ;

                do
                {
                    printf("Digite o codigo do cliente: ");
                    if (scanf("%d", &codigo_cliente) != 1)
                    {
                        printf("Entrada invalida. O codigo do cliente deve ser um numero inteiro.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                while (getchar() != '\n')
                    ;

                do
                {
                    printf("Digite o numero de ocupantes: ");
                    if (scanf("%d", &numero_ocupantes) != 1)
                    {
                        printf("Entrada invalida. Insira um valor numerico para o numero de ocupantes.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                while (getchar() != '\n')
                    ;

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
                do
                {
                    printf("Digite o codigo do cliente: ");
                    if (scanf("%d", &codigo_cliente) != 1)
                    {
                        printf("Entrada invalida. O codigo deve ser um numero inteiro.\n");
                        while (getchar() != '\n')
                            ;
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                while (getchar() != '\n')
                    ;
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
}