/* Sistema Integrado de Locadora de Carros - Arquivo principal */
#include <stdio.h>
#include <stdlib.h>

#include "modelos.h"
#include "cliente.h"
#include "carro.h"
#include "aluguel.h"

// menuPrincipal: inicializa os dados e roda o loop do menu principal
// Recebe: vetorClientes (ponteiro duplo), qtd e capacidade de clientes,
//         carros, cadastrados, alugueis e qtdAlugueis | Retorna: void
void menuPrincipal(
    Cliente **vetorClientes,
    int *qtdClientes,
    int *capacidadeClientes,
    Carro carros[],
    int *cadastrados,
    Aluguel alugueis[],
    int *qtdAlugueis
) {
    // malloc não zera a memória — inicializa 'ativo' de cada posição para evitar lixo
    for(int i = 0; i < *capacidadeClientes; i++)
        (*vetorClientes)[i].ativo = 0;

    int opcao = 0;

    while(opcao != 4) {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Gerenciar clientes\n");
        printf("2. Gerenciar carros\n");
        printf("3. Gerenciar alugueis\n");
        printf("4. Sair do Sistema\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if(opcao == 1)
            // Passa ponteiro duplo e capacidade pois cadastrar/excluir podem realocar o vetor
            submenuClientes(vetorClientes, qtdClientes, capacidadeClientes);
        else if(opcao == 2)
            submenuCarros(carros, cadastrados);
        else if(opcao == 3)
            submenuAlugueis(&alugueis, qtdAlugueis, *vetorClientes, *qtdClientes, carros, *cadastrados);
        else if(opcao == 4)
            printf("Encerrando o sistema de locacao...\n");
        else
            printf("\n[ERRO] Opcao invalida! Tente novamente.\n");
    }
}

// main: aloca o vetor de clientes, carrega os dados salvos, roda o menu e libera a memória
// Recebe: nada | Retorna: 0 sucesso, 1 falha de alocação
int main() {
    int capacidadeClientes = MAX_CLIENTES;
    int qtdClientes = 0;

    // Alocação dinâmica inicial: reserva espaço para MAX_CLIENTES clientes na heap
    Cliente *vetorClientes = malloc(sizeof(Cliente) * capacidadeClientes);
    if(vetorClientes == NULL) {
        printf("[ERRO] Falha ao alocar memoria inicial para os clientes!\n");
        return 1;
    }

    Carro carros[MAX_CAR];
    int cadastrados = 0;

    Aluguel alugueis[Maximocadastros];
    int qtdAlugueis = 0;

    // Restaura os clientes do arquivo binário antes de abrir o menu
    carregarClientesArquivo(&vetorClientes, &qtdClientes, &capacidadeClientes);

    menuPrincipal(&vetorClientes, &qtdClientes, &capacidadeClientes, carros, &cadastrados, alugueis, &qtdAlugueis);

    // Persiste os dados no arquivo antes de encerrar
    salvarClientesArquivo(vetorClientes, qtdClientes);

    free(vetorClientes); // libera a memória alocada com malloc/realloc

    return 0;
}