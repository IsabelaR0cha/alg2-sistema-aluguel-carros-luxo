// Sistema Integrado de Locadora de Carros de Luxo - Arquivo principal
#include <stdio.h>
#include <stdlib.h>

#include "modelos.h"
#include "cliente.h"
#include "carro.h"
#include "aluguel.h"
#include "relatorios.h"
#include "util.h"

/*
 * Exibe o menu principal e encaminha o usuário
 * para os submenus de clientes, carros e aluguéis.
 *
 * Os ponteiros duplos permitem que os submenus alterem
 * os vetores após chamadas de realloc().
 */

void menuPrincipal(
    Cliente **clientes,
    int *qtdClientes,
    Carro **carros,
    int *qtdCarros,
    Aluguel **alugueis,
    int *qtdAlugueis)
{
    int opcao = 0;

    while (opcao != 5)
    {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Gerenciar Clientes\n");
        printf("2. Gerenciar Carros\n");
        printf("3. Gerenciar Alugueis\n");
        printf("4. Gerar Relatorios\n");
        printf("5. Sair do Sistema\n");
        printf("Escolha uma Opcao: ");

        if (scanf("%d", &opcao) != 1)
        {
            if (feof(stdin))
            {
                printf("\nEntrada encerrada. Encerrando o sistema de locacao...\n");
                opcao = 5;
                break;
            }

            limparBuffer();
            printf("\n[ERRO] Digite apenas numeros!\n");
            opcao = 0;
            continue;
        }

        limparBuffer();

        if (opcao == 1)
        {
            submenuClientes(clientes, qtdClientes);
        }
        else if (opcao == 2)
        {
            submenuCarros(carros, qtdCarros);
        }
        else if (opcao == 3)
        {
            submenuAlugueis(
                alugueis,
                qtdAlugueis,
                *clientes,
                *qtdClientes,
                *carros,
                *qtdCarros);
        }
        else if (opcao == 4)
        {
            /*
             * Os relatórios apenas consultam os dados.
             * Por isso recebem os vetores e quantidades sem ponteiro duplo.
             */
            menuRelatorios(
                *clientes,
                *qtdClientes,
                *carros,
                *qtdCarros,
                *alugueis,
                *qtdAlugueis);
        }
        else if (opcao == 5)
        {
            printf("Encerrando o sistema de locacao...\n");
        }
        else
        {
            printf("\n[ERRO] Opcao invalida! Tente novamente.\n");
        }
    }
}

int main(void)
{
    Cliente *clientes = NULL;
    int qtdClientes = 0;

    Carro *carros = NULL;
    int qtdCarros = 0;

    Aluguel *alugueis = NULL;
    int qtdAlugueis = 0;

    // Carrega os clientes salvos
    carregarClientesArquivo(&clientes, &qtdClientes);

    // Carrega os carros salvos
    carros = carregarCarros(&qtdCarros);

    // Carrega os alugueis salvos
    carregarAlugueis(&alugueis, &qtdAlugueis);

    // Inicia o sistema
    menuPrincipal(
        &clientes,
        &qtdClientes,
        &carros,
        &qtdCarros,
        &alugueis,
        &qtdAlugueis
    );

    // Salva todos os registros antes de encerrar
    salvarClientesArquivo(clientes, qtdClientes);
    salvarCarros(carros, qtdCarros);
    salvarAlugueis(alugueis, qtdAlugueis);

    // Libera a memória dinâmica
    free(clientes);
    free(carros);
    free(alugueis);

    return 0;
}
