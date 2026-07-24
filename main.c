// Sistema Integrado de Locadora de Carros de Luxo - Arquivo principal
#include <stdio.h>
#include <stdlib.h>

#include "modelos.h"
#include "cliente.h"
#include "carro.h"
#include "aluguel.h"
#include "relatorios.h"

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
        scanf("%d", &opcao);

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

    /*
     * Inicializa os vetores dinâmicos sem memória alocada.
     * Cada cadastro aumentará o respectivo vetor com realloc().
     */
    Cliente *clientes = NULL;
    int qtdClientes = 0;

    Carro *carros = NULL;
    int qtdCarros = 0;

    Aluguel *alugueis = NULL;
    int qtdAlugueis = 0;

    // Inicia o sistema e permite que os menus alterem os vetores e quantidades.
    menuPrincipal(&clientes, &qtdClientes, &carros, &qtdCarros, &alugueis, &qtdAlugueis);

    // Libera toda a memória dinâmica antes de encerrar o programa.
    free(clientes);
    free(carros);
    free(alugueis);

    return 0;
}
