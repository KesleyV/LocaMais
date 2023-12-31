#include "menu.h"
#include "Locacao.h"
#include "Cliente.h"
#include "Veiculo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <conio.h>
#include <locale.h>
#include <stdbool.h>
#include "Decorador.h"
#include <time.h>

// funções auxiliares
int validarFormatoData(const char *data)
{
    int dia, mes, ano;

    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) == 3)
    {
        if (ano >= 1000 && ano <= 9999)
        {
            if (mes >= 1 && mes <= 12)
            {
                int dias_no_mes;

                switch (mes)
                {
                case 4:
                case 6:
                case 9:
                case 11:
                    dias_no_mes = 30;
                    break;
                case 2:
                    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))
                    {
                        dias_no_mes = 29; // Ano bissexto
                    }
                    else
                    {
                        dias_no_mes = 28;
                    }
                    break;
                default:
                    dias_no_mes = 31;
                    break;
                }

                if (dia >= 1 && dia <= dias_no_mes)
                {
                    struct tm inputDate = {0};
                    inputDate.tm_year = ano - 1900;
                    inputDate.tm_mon = mes - 1;
                    inputDate.tm_mday = dia;

                    time_t inputTime = mktime(&inputDate);

                    if (inputTime != -1 && difftime(inputTime, time(NULL)) >= 0)
                    {
                        return 1; // Data válida
                    }
                }
            }
        }
    }

    return 0; // Data inválida
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
        limpar_console();

        char manual[5][100] = {
            "BEM VINDO A LOCA MAIS",
            "PARA NAVEGAR NO MENU UTILIZE AS SETAS DO SEU TECLADO",
            "PARA ACESSAR UMA OPCAO TECLE ENTER",
            "PARA VOLTAR NO MENU ANTERIOR UTILIZE A TECLA ESC OU A OPCAO VOLTAR DOS MENUS"};
        manualPosition(4, manual);

        char opcoes_menu[5][40] = {
            "MENU CLIENTE",
            "MENU VEICULO",
            "MENU LOCACAO",
            "SAIR"};

        opcao = estiliza_menu(4, opcoes_menu);

        switch (opcao)
        {
        case 1:
        {
            limpar_console();

            char manual[5][100] = {
                "BEM VINDO A LOCA MAIS",
                "PARA NAVEGAR NO MENU UTILIZE AS SETAS DO SEU TECLADO",
                "PARA ACESSAR UMA OPCAO TECLE ENTER",
                "PARA VOLTAR NO MENU ANTERIOR UTILIZE A TECLA ESC OU A OPCAO VOLTAR DOS MENUS"};
            manualPosition(4, manual);

            char opcoes_cliente[7][40] = {
                "Cadastrar Cliente",
                "Exibir todos os clientes",
                "Pesquisar Cliente",
                "Desativar cliente",
                "Reativar cliente",
                "Voltar"};

            int opcao_cliente = estiliza_menu(6, opcoes_cliente);

            switch (opcao_cliente)
            {
            case 1:
            {
                limpar_console();
                char nome[50];
                char endereco[150];
                char telefone[13];

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
                cadastrar_cliente(nome, endereco, telefone);
                break;
            }
            case 2:
            {
                limpar_console();
                exibir_clientes();
                break;
            }
            case 3:
            {
                limpar_console();
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
                limpar_console();
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
                desativar_cliente(codigo_cliente);
                break;
            }
            case 5:
            {
                limpar_console();
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
                reativar_cliente(codigo_cliente);
                break;
            }
            case 6:
            {
                limpar_console();
                break;
            }
            default:
            {
                limpar_console();
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }
            }
            break;
        }
        case 2:
        {
            limpar_console();

            char manual[5][100] = {
                "BEM VINDO A LOCA MAIS",
                "PARA NAVEGAR NO MENU UTILIZE AS SETAS DO SEU TECLADO",
                "PARA ACESSAR UMA OPCAO TECLE ENTER",
                "PARA VOLTAR NO MENU ANTERIOR UTILIZE A TECLA ESC OU A OPCAO VOLTAR DOS MENUS"};
            manualPosition(4, manual);

            char opcoes_veiculo[13][40] = {
                "Cadastrar Veiculo",
                "Exibir todos os veiculos",
                "Pesquisar Veiculo",
                "Desativar veiculo",
                "Reativar veiculo",
                "Exibir veiculos disponiveis",
                "Exibir veiculos alugados",
                "Exibir veiculos em manutencao",
                "Retirar veiculo da manutencao",
                "Enviar veiculo para manutencao",
                "Voltar"};

            int opcao_veiculo = estiliza_menu(11, opcoes_veiculo);

            switch (opcao_veiculo)
            {
            case 1:
            {
                limpar_console();
                char descricao[100];
                char modelo[50];
                char cor[20];
                char placa[10];
                float valor_diaria;
                int ocupantes;
                char status[20];

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

                cadastrar_veiculo(descricao, modelo, cor, placa, valor_diaria, ocupantes, status);
                break;
            }
            case 2:
            {
                limpar_console();
                exibir_veiculos();
                break;
            }
            case 3:
            {
                limpar_console();
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
                limpar_console();
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
                desativar_veiculo(codigo_veiculo);
                break;
            }
            case 5:
            {
                limpar_console();
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
                reativar_veiculo(codigo_veiculo);
                break;
            }
            case 6:
            {
                limpar_console();
                exibir_veiculos_disponiveis();
                break;
            }
            case 7:
            {
                limpar_console();
                exibir_veiculos_alugados();
                break;
            }
            case 8:
            {
                limpar_console();
                exibir_veiculos_em_manutencao();
                break;
            }
            case 9:
            {
                limpar_console();
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
            case 10:
            {
                limpar_console();
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
            case 11:
            {
                limpar_console();
                break;
            }
            default:
            {
                limpar_console();
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }
            }
            break;
        }
        case 3:
        {
            limpar_console();

            char manual[5][100] = {
                "BEM VINDO A LOCA MAIS",
                "PARA NAVEGAR NO MENU UTILIZE AS SETAS DO SEU TECLADO",
                "PARA ACESSAR UMA OPCAO TECLE ENTER",
                "PARA VOLTAR NO MENU ANTERIOR UTILIZE A TECLA ESC OU A OPCAO VOLTAR DOS MENUS"};
            manualPosition(4, manual);

            char opcoes_locacao[9][40] = {
                "Cadastrar Locacao",
                "Exibir locacoes ativas",
                "Exibir locacoes ativas do cliente",
                "Dar Baixa em Locacao",
                "Exibir faturamento do cliente",
                "Exibir faturamento geral",
                "Exibir total faturado",
                "Voltar"};

            int opcao_locacao = estiliza_menu(8, opcoes_locacao);

            switch (opcao_locacao)
            {
            case 1:
            {
                limpar_console();
                char data_retirada[11];
                char data_devolucao[11];
                int seguro;
                int codigo_cliente;
                int numero_ocupantes;

                do
                {
                    printf("Digite a data de retirada (formato: dd/mm/yyyy): ");
                    if (scanf("%s", data_retirada) != 1 || !validarFormatoData(data_retirada))
                    {
                        printf("Entrada invalida. A data tem que obedecer o formato correto e nenhum veiculo pode ser alugado com menos de um dia de antecedencia a contar da data de hoje.\n");
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
                        printf("Entrada invalida. A data tem que obedecer o formato correto e o seu intervalo tem que ser maior ou igual que 1 dia a partir da data de retirada.\n");
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

                cadastrar_locacao(data_retirada, data_devolucao, seguro, codigo_cliente, numero_ocupantes);
                break;
            }
            case 2:
            {
                limpar_console();
                exibir_todas_locacoes_ativas();
                break;
            }
            case 3:
            {
                limpar_console();
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
                mostrar_locacoes_ativas_cliente(codigo_cliente);
                break;
            }
            case 4:
            {
                limpar_console();
                int codigo_locacao;
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
                dar_baixa_locacao(codigo_locacao);
                break;
            }
            case 5:
            {
                limpar_console();
                int codigo_cliente;
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
                exibirRelatorioFaturamentoCliente(codigo_cliente);
                break;
            }
            case 6:
            {
                limpar_console();
                exibirRelatorioFaturamento();
                break;
            }
            case 7:
            {
                limpar_console();
                exibirTotalGeral();
                break;
            }
            case 8:
            {
                limpar_console();
                break;
            }
            default:
            {
                limpar_console();
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }
            }
            break;
        }
        default:
        {
            limpar_console();
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
        }
    }
}