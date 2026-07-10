#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "aluguel.h"

//*Verifica se o funcionario não esta usando um código repetido para cadastrar o aluguel.
int codigoExiste(Aluguel alugueis[], int qtdAlugueis, int codigo) {
    for(int i = 0; i < qtdAlugueis; i++) {
        if(alugueis[i].codigo_aluguel == codigo) {
            return 1;
        }
    }

    return 0;
}

void cadastrarAluguel(Aluguel alugueis[], int *qtdAlugueis) {
    int pos = *qtdAlugueis;

    if(*qtdAlugueis >= Maximocadastros) {
        printf("Limite de alugueis atingido!");
        return;
    }

    printf("\n--- CADASTRAR ALUGUEL ---\n");

    int codigo;

    do {
        printf("Codigo do aluguel: ");
        scanf("%d", &codigo);

        if(codigoExiste(alugueis, *qtdAlugueis, codigo)) {
            printf("Esse codigo ja esta cadastrado!\n");
        }

    } while(codigoExiste(alugueis, *qtdAlugueis, codigo));

    alugueis[pos].codigo_aluguel = codigo;
    getchar();

    int validarPlaca = 0;

    while(!validarPlaca) {
        printf("Placa do carro: ");
        fgets(alugueis[pos].placa, 8, stdin);

        alugueis[pos].placa[strcspn(alugueis[pos].placa, "\n")] = '\0';

        if(strlen(alugueis[pos].placa) != 7) {
            printf("Placa invalida!\n");
            continue;
        }

        int ok_placa = 1;

        for(int i = 0; i < 7; i++) {
            if(i < 3) {
                if(!isalpha(alugueis[pos].placa[i])) {
                    ok_placa = 0;
                }
            } else if(i == 3) {
                if(!isdigit(alugueis[pos].placa[i])) {
                    ok_placa = 0;
                }
            } else if(i == 4) {
                if(!isalpha(alugueis[pos].placa[i])) {
                    ok_placa = 0;
                }
            } else {
                if(!isdigit(alugueis[pos].placa[i])) {
                    ok_placa = 0;
                }
            }
        }

        if(!ok_placa) {
            printf("Placa invalida!\n");
        } else {
            validarPlaca = 1;
        }
    }

    int valido = 0;

    getchar();

    getchar();

    printf("CPF do cliente: ");
    fgets(alugueis[pos].cpf_cliente, 15, stdin);
    alugueis[pos].cpf_cliente[strcspn(alugueis[pos].cpf_cliente, "\n")] = '\0';

    printf("Data de retirada (dd/mm/aa): ");
    scanf(" %10s", alugueis[pos].data_retirada);

    printf("Data de devolucao (dd/mm/aa): ");
    scanf(" %10s", alugueis[pos].data_devolucao);

    printf("Valor total: ");
    scanf("%lf", &alugueis[pos].valor_total);

    (*qtdAlugueis)++;

    printf("Aluguel cadastrado com sucesso!\n");
}

//*a função lista todos alugueis que ja foram adicionados, mostrando todos os dados. 
void listarAlugueis(Aluguel alugueis[], int qtdAlugueis) {
    if(qtdAlugueis == 0){
        printf("Nenhum aluguel cadastrado.");
        return;
    }
    printf("\n--- LISTA DE ALUGUEIS ---\n");

    for(int i = 0; i < qtdAlugueis; i++){
        printf("\nAluguel %d\n", i + 1);
        printf("Codigo do aluguel: %d\n", alugueis[i].codigo_aluguel);
        printf("Placa do carro: %s", alugueis[i].placa);
        printf("CPF do cliente: %s\n", alugueis[i].cpf_cliente);
        printf("Data de retirada: %s\n", alugueis[i].data_retirada);
        printf("Data de devolucao: %s\n", alugueis[i].data_devolucao);
        printf("Valor total: %.2lf\n", alugueis[i].valor_total);
    }
}

/*Além de possuir a listagem de todos alugueis, nessa função podemos procurar um aluguel especifico através do código
o que acaba facilitando na hora para o funcionário.*/
void buscarAluguel(Aluguel alugueis[], int qtdAlugueis) {
    int codigo;
    int encontrado = 0;

    printf("\n--- BUSCAR ALUGUEL ---\n");

    printf("Digite o codigo do aluguel: ");
    scanf("%d", &codigo);

    for (int i = 0; i < qtdAlugueis; i++) {

        if (alugueis[i].codigo_aluguel == codigo) {
            printf("\nAluguel encontrado:\n");
            printf("Codigo do aluguel: %d\n", alugueis[i].codigo_aluguel);
            printf("Placa do carro: %s\n", alugueis[i].placa);
            printf("CPF do cliente: %s\n", alugueis[i].cpf_cliente);
            printf("Data de retirada: %s\n", alugueis[i].data_retirada);
            printf("Data de devolucao: %s\n", alugueis[i].data_devolucao);
            printf("Valor total: %.2lf\n", alugueis[i].valor_total);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("Aluguel nao encontrado.\n");
    }
}

/*esta é a função para alterar o aluguel caso necessário, como por exemplo 
algum erro de digitação cometido em algum dos campos.*/
void alterarAluguel(Aluguel alugueis[], int qtdAlugueis) {
    int id;
    int encontrado = 0;

    printf("Digite o codigo do aluguel que deseja modificar: ");
    scanf("%d", &id);

    for(int i = 0; i < qtdAlugueis; i++){
        if(alugueis[i].codigo_aluguel == id){
            printf("Novo CPF do cliente: ");
            scanf("%11s", alugueis[i].cpf_cliente);

            printf("Novo codigo do aluguel: ");
            scanf("%d", &alugueis[i].codigo_aluguel);

            printf("Nova data de retirada: ");
            scanf("%s", alugueis[i].data_retirada);

            printf("Nova data de devolucao: ");
            scanf("%s", alugueis[i].data_devolucao);

            printf("Novo valor total: ");
            scanf("%lf", &alugueis[i].valor_total);

            encontrado = 1;

            break; //* para o loop porque encontrou o registro
        }
    }
    if(!encontrado){
        printf("Aluguel nao encontrado.\n");
    }
}

void excluirAluguel(Aluguel alugueis[], int *qtdAlugueis) {
    int id;
    int encontrado = 0;

    printf("Digite o codigo do aluguel que deseja excluir: ");
    scanf("%d", &id);
    for (int i = 0; i < *qtdAlugueis; i++) {

        if (alugueis[i].codigo_aluguel == id) {

            for (int j = i; j < *qtdAlugueis - 1; j++) {
                alugueis[j] = alugueis[j + 1];
            }
            (*qtdAlugueis)--;
            encontrado = 1;

            printf("Aluguel excluido com sucesso!\n");
            break;
        }
    }
    if (!encontrado) {
        printf("Aluguel nao encontrado.\n");
    }
}

void menuAluguel(Aluguel alugueis[], int *qtdAlugueis) {
    int opcao;

    do {
        printf("\n--- MENU ALUGUEL ---\n");
        printf("1 - Cadastrar aluguel\n");
        printf("2 - Listar alugueis\n");
        printf("3 - Alterar aluguel\n");
        printf("4 - Excluir aluguel\n");
        printf("5 - Buscar aluguel\n");
        printf("0 - Voltar ao menu principal\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                cadastrarAluguel(alugueis, qtdAlugueis);
                break;
            case 2:
                listarAlugueis(alugueis, *qtdAlugueis);
                break;
            case 3:
                alterarAluguel(alugueis, *qtdAlugueis);
                break;
            case 4:
                excluirAluguel(alugueis, qtdAlugueis);
                break;
            case 5:
                buscarAluguel(alugueis, *qtdAlugueis);
                break;
            case 0:
                break;
            default: 
                printf("[ERRO] Opcao invalida!\n");
        }

    } while(opcao != 0);
}


void submenuAlugueis(Aluguel alugueis[], int *qtdAlugueis) {
    menuAluguel(alugueis, qtdAlugueis);
}