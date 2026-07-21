/* Sistema Integrado de Locadora de Carros de Luxo - Arquivo único */
#include <stdio.h>
#include <stdlib.h>

#include "modelos.h"
#include "cliente.h"
#include "carro.h"
#include "aluguel.h"

void menuPrincipal( Cliente *vetorClientes,
    Carro carros[],
    int *cadastrados,
    Aluguel alugueis[],
    int *qtdAlugueis
) {
    // Inicialização defensiva: malloc() não zera a memória alocada, então
    // os campos 'ativo' começariam com valor indeterminado (lixo de
    // memória) se não fossem explicitamente zerados aqui.
    for(int i = 0; i < MAX_CLIENTES; i++) {
        vetorClientes[i].ativo = 0;
    }

    int qtdClientes = 0;
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
            submenuClientes(vetorClientes, &qtdClientes);
        } else if(opcao == 2) {
            submenuCarros(carros, cadastrados);;
        } else if(opcao == 3) {
            submenuAlugueis(&alugueis, qtdAlugueis, vetorClientes, qtdClientes, carros, *cadastrados);;
        } else if(opcao == 4) {
            printf("Encerrando o sistema de locacao...\n");
            return;
        } else {
            printf("\n[ERRO] Opcao invalida! Tente novamente.\n");
        }
    }
}

int main() {
    Cliente *vetorClientes = malloc(sizeof(Cliente) * MAX_CLIENTES);
    if(vetorClientes == NULL) return 1;

    Carro carros[MAX_CAR];
    int cadastrados = 0;

    Aluguel alugueis[Maximocadastros];
    int qtdAlugueis = 0;

    menuPrincipal(vetorClientes, carros, &cadastrados, alugueis, &qtdAlugueis);

    free(vetorClientes);

    return 0;
}