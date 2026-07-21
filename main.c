/* Sistema Integrado de Locadora de Carros de Luxo - Arquivo principal */
#include <stdio.h>
#include <stdlib.h>

#include "modelos.h"
#include "cliente.h"
#include "carro.h"
#include "aluguel.h"

void menuPrincipal( 
    Cliente **vetorClientes,
    int *qtdClientes,
    int *capacidadeClientes,
    Carro carros[],
    int *cadastrados,
    Aluguel alugueis[],
    int *qtdAlugueis
) {
    // Inicialização defensiva: malloc() não zera a memória alocada, então
    // os campos 'ativo' começariam com valor indeterminado (lixo de
    // memória) se não fossem explicitamente zerados aqui.
    // (Nota: Como migramos para exclusão física com compactação, os campos 
    // são controlados dinamicamente via qtd, mas mantemos o laço seguro por compatibilidade).
    for(int i = 0; i < *capacidadeClientes; i++) {
        (*vetorClientes)[i].ativo = 0;
    }

    int opcao = 0;

    while(opcao != 4) {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Gerenciar clientes\n");
        printf("2. Gerenciar carros\n");
        printf("3. Gerenciar alugueis\n");
        printf("4. Sair do Sistema\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if(opcao == 1) {
            // Passamos &qtdClientes (endereço da variável) porque
            // submenuClientes() e, por consequência, cadastrarCliente()/
            // excluirCliente() precisam alterar o valor real de
            // qtdClientes, e não apenas uma cópia local.
            // Também repassamos &capacidadeClientes para gerenciar o realloc dinamicamente.
            submenuClientes(vetorClientes, qtdClientes, capacidadeClientes);
        } else if(opcao == 2) {
            submenuCarros(carros, cadastrados);
        } else if(opcao == 3) {
            submenuAlugueis(alugueis, qtdAlugueis);
        } else if(opcao == 4) {
            printf("Encerrando o sistema de locacao...\n");
            return;
        } else {
            printf("\n[ERRO] Opcao invalida! Tente novamente.\n");
        }
    }
}

int main() {
    int capacidadeClientes = MAX_CLIENTES;
    int qtdClientes = 0;

    // Alocação dinâmica inicial do vetor de clientes usando malloc
    Cliente *vetorClientes = malloc(sizeof(Cliente) * capacidadeClientes);
    if(vetorClientes == NULL) {
        printf("[ERRO] Falha ao alocar memoria inicial para os clientes!\n");
        return 1;
    }

    Carro carros[MAX_CAR];
    int cadastrados = 0;

    Aluguel alugueis[Maximocadastros];
    int qtdAlugueis = 0;

    // Chamada do menu principal passando os ponteiros de controle necessários para o realloc
    menuPrincipal(&vetorClientes, &qtdClientes, &capacidadeClientes, carros, &cadastrados, alugueis, &qtdAlugueis);

    // Liberação da memória alocada dinamicamente ao sair do sistema
    free(vetorClientes);

    return 0;
}
