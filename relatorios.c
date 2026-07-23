#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "relatorios.h"
#include "cliente.h"
#include "carro.h"
#include "util.h"

    // 1. Menu principal dos relatorios
    void menuRelatorios(Cliente *clientes, int qtdClientes, Carro *carros, int qtdCarros, Aluguel *alugueis, int qtdAlugueis){
        
    int opcao;
    do {
        printf("\n--- SISTEMA DE RELATORIOS ---\n");
        printf("1. Relatorio por CPF\n");
        printf("2. Relatorio por Placa\n");
        printf("3. Relatorio por Valor Minimo\n");
        printf("0. Voltar\n");
        printf("Escolha uma opcao: ");
        
        // Protecao contra caracteres que nao sejam numeros
        if (scanf("%d", &opcao) != 1) { 
            printf("Erro: Digite apenas numeros.\n");
            limparBuffer();
            opcao = -1;
            continue;
        }
        
        limparBuffer();
        
        switch(opcao) {
            case 1: relatorioPorCPF(clientes, qtdClientes, carros, qtdCarros, alugueis, qtdAlugueis); break;
            case 2: relatorioPorPlaca(clientes, qtdClientes, carros, qtdCarros, alugueis, qtdAlugueis); break;
            case 3: relatorioPorValorMinimo(clientes, qtdClientes, carros, qtdCarros, alugueis, qtdAlugueis); break;
            case 0: printf("Voltando ao menu principal...\n"); break;
            default: printf("Opcao invalida! Tente novamente.\n");
        }
    } while(opcao != 0);
        
    }
    
    // 2. Relatorio de alugueis por CPF do cliente
    void relatorioPorCPF(Cliente *clientes, int qtdClientes, Carro *carros, int qtdCarros, Aluguel *alugueis, int qtdAlugueis){
        
    char cpfBusca[16]; 
    int encontrado = 0;

    printf("Digite o CPF para o relatorio (ex: 12345678901): ");
    fgets(cpfBusca, 15, stdin);
    limparTexto(cpfBusca); 

    FILE *f = fopen("relatorio_alugueis.txt", "w");
    if (f == NULL) {
        printf("Erro ao criar arquivo de relatorio!\n");
        return;
    }

    fprintf(f, "RELATORIO DE ALUGUEIS - FILTRO POR CPF: %s\n", cpfBusca);
    fprintf(f, "========================================================\n");

    for (int i = 0; i < qtdAlugueis; i++) {
        if (strcmp(alugueis[i].cpf_cliente, cpfBusca) == 0) {
            encontrado = 1;

            // BUSCA DO NOME
            int posicao = buscarPorCpf(clientes, qtdClientes, alugueis[i].cpf_cliente);
            char *nomeDoCliente;
            if (posicao != -1) {
                nomeDoCliente = clientes[posicao].nome;
            } else {
                nomeDoCliente = "Cliente nao encontrado";
            }

            // BUSCA DO CARRO
            Carro *carroAchado = buscarCarroPorPlaca(carros, qtdCarros, alugueis[i].placa);
            char *modeloDoCarro;
            if (carroAchado != NULL) {
                modeloDoCarro = carroAchado->modelo;
            } else {
                modeloDoCarro = "Carro nao encontrado";
            }

            // Grava os dados no arquivo
            fprintf(f, "Codigo Aluguel: %d | Dias: %d | Total: R$ %.2f\n", alugueis[i].codigo_aluguel, alugueis[i].dias, alugueis[i].valor_total);
            fprintf(f, "Cliente: %s | Carro: %s\n", nomeDoCliente, modeloDoCarro);
            fprintf(f, "--------------------------------------------------------\n");
        }
    }
    if (encontrado == 0) {
        fprintf(f, "Nenhum aluguel encontrado para o CPF: %s\n", cpfBusca);
    }

    fclose(f); 
    printf("\nRelatorio por CPF gerado em 'relatorio_alugueis.txt'!\n");

    }

    // 3. Relatorio de alugueis por Placa do carro
    void relatorioPorPlaca(Cliente *clientes, int qtdClientes, Carro *carros, int qtdCarros, Aluguel *alugueis, int qtdAlugueis){

    char placaBusca[8]; 
    int encontrado = 0;

    printf("Digite a placa para o relatorio (ex: ABC1D23): ");
    scanf("%7s", placaBusca);
    limparBuffer(); 

    FILE *f = fopen("relatorio_alugueis.txt", "w"); 
    if (f == NULL) {
        printf("Erro ao criar arquivo de relatorio!\n");
        return;
    }

    fprintf(f, "RELATORIO DE ALUGUEIS - FILTRO POR PLACA: %s\n", placaBusca);
    fprintf(f, "========================================================\n");

    for (int i = 0; i < qtdAlugueis; i++) {
        if (strcmp(alugueis[i].placa, placaBusca) == 0) {
            encontrado = 1;

            // BUSCA DO NOME
            int posicao = buscarPorCpf(clientes, qtdClientes, alugueis[i].cpf_cliente);
            char *nomeDoCliente;
            if (posicao != -1) { 
                nomeDoCliente = clientes[posicao].nome;
            } else {
                nomeDoCliente = "Cliente nao encontrado";
            }

            // BUSCA DO CARRO
            Carro *carroAchado = buscarCarroPorPlaca(carros, qtdCarros, alugueis[i].placa);
            char *modeloDoCarro;
            if (carroAchado != NULL) { 
                modeloDoCarro = carroAchado->modelo;
            } else {
                modeloDoCarro = "Carro nao encontrado";
            }

            // Grava os dados no arquivo
            fprintf(f, "Codigo: %d | Carro: %s | Total: R$ %.2f\n", 
                    alugueis[i].codigo_aluguel, modeloDoCarro, alugueis[i].valor_total);
            fprintf(f, "Cliente: %s | Dias: %d\n", nomeDoCliente, alugueis[i].dias);
            fprintf(f, "--------------------------------------------------------\n");
        }
    }
    if (encontrado == 0) {
        fprintf(f, "Nenhum aluguel registrado para a placa: %s\n", placaBusca);
    }

    fclose(f); 
    printf("Relatorio por placa gerado com sucesso em 'relatorio_alugueis.txt'!\n");

    }

    // 4. Relatorio por Valor Total Minimo
    void relatorioPorValorMinimo(Cliente *clientes, int qtdClientes, Carro *carros, int qtdCarros, Aluguel *alugueis, int qtdAlugueis){

    float valorCorte;
    int encontrado = 0;

    printf("Exibir alugueis com valor total maior ou igual a R$: ");
    scanf("%f", &valorCorte);
    limparBuffer();

    FILE *f = fopen("relatorio_alugueis.txt", "w");
    if (f == NULL) {
        printf("Erro ao criar arquivo!\n");
        return;
    }

    fprintf(f, "RELATORIO - ALUGUEIS ACIMA DE R$ %.2f\n", valorCorte);
    fprintf(f, "========================================================\n");

    for (int i = 0; i < qtdAlugueis; i++) {
        if (alugueis[i].valor_total >= valorCorte) {
            encontrado = 1;

            // BUSCA DO NOME
            int posicao = buscarPorCpf(clientes, qtdClientes, alugueis[i].cpf_cliente);
            char *nomeDoCliente;
            
            if (posicao != -1) {
                nomeDoCliente = clientes[posicao].nome;
            } else {
                nomeDoCliente = "Cliente nao encontrado";
            }

            // BUSCA DO CARRO
            Carro *carroAchado = buscarCarroPorPlaca(carros, qtdCarros, alugueis[i].placa);
            char *modeloDoCarro;
            
            if (carroAchado != NULL) {
                modeloDoCarro = carroAchado->modelo;
            } else {
                modeloDoCarro = "Carro nao encontrado";
            }

            // Grava os dados no arquivo
            fprintf(f, "Codigo: %d | Valor: R$ %.2f | Carro: %s\n",
                    alugueis[i].codigo_aluguel, alugueis[i].valor_total, modeloDoCarro);
            fprintf(f, "Cliente: %s\n", nomeDoCliente);
            fprintf(f, "--------------------------------------------------------\n");
        }
    }
    if (encontrado == 0) {
        fprintf(f, "Nenhum aluguel atingiu o valor de R$ %.2f\n", valorCorte);
    }

    fclose(f);
    printf("Relatorio de valores minimos gerado em 'relatorio_alugueis.txt'!\n");

    }
