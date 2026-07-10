#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "carro.h"

int validarPlaca(char *placa) {
    //recebe uma placa e verifica se esta no fomato Mercosul e padroniza os caracteres em Maiusculos, valida = 1, invalida = 0.
    char *p = placa;
    if(strlen(placa)!=7){
        return 0;
    }
    if(isalpha(*p)==0 || isalpha(*(p+1))==0 || isalpha(*(p+2))==0 ||isdigit(*(p+3))==0 ||
       isalpha(*(p+4))==0 || isdigit(*(p+5))==0 || isdigit(*(p+6))==0){
        return 0;
    }
    else{
        for(p; p < placa+7; p++){
            if(isalpha(*p)){
                *p = toupper(*p);
            }
        }
        return 1;
    }
}

int placaExiste(Carro *carros, int cadastrados, char *placa) {
    //recebe a lista de carros, a quantidade de carros cadastrados e uma placa, placa já cadastrada = 1, não cadastrada = 0.
    for(Carro *p=carros; p<carros+cadastrados; p++){
        if(strcmp(placa, p->placa)==0){
            return 1;
        }
    }
    return 0;
}

int cadastrarCarro(Carro *car, Carro *carros, int cadastrados) {
    //recebe uma posição de um vetor de carros, a lista de carros e a quantidade de carros.
    //placa invalida = 0, valida = 1;
    printf("\nDigite a placa no formato (ABC1D23): "); scanf("%7s", car->placa);
    
    if(validarPlaca(car->placa)){
        if(placaExiste(carros, cadastrados, car->placa)){
            printf("\n[ERRO] Placa ja cadastrada.\n\n");
            return 0;
        }
        else{
            printf("[OK] Placa aceita: %s\n", car->placa);
            printf("\nDigite a marca do carro: "); scanf(" %[^\n]", car->marca);
            printf("Digite o Modelo: "); scanf(" %[^\n]", car->modelo);
            printf("Digite a cor: "); scanf(" %[^\n]", car->cor);
            printf("Digite a Quilometragem: "); scanf("%f", &car->km);
            printf("Digite o Valor do aluguel: "); scanf("%f", &car->valor);
            printf("\n[OK] Carro cadastrado com sucesso!\n\n");
            return 1;
        }
    }
    else{
        printf("\n[ERRO] Placa Invalida!!\n\n");
        return cadastrarCarro(car, carros, cadastrados);
    }
}

void consultarCarro(Carro *carros, int cadastrados, char *placa) {
    //recebe uma lista de carros, a quantidade de cadastros e uma placa.
    if(!validarPlaca(placa)){
        printf("\n[ERRO] Placa Invalida!\n\n");
        return;
    }
    if(!placaExiste(carros, cadastrados, placa)){
        printf("\n[ERRO] Placa nao Cadastrada!\n\n");
        return;
    }
    for(Carro *p=carros; p<carros+cadastrados; p++){
        if(strcmp(placa, p->placa)==0){
            printf("Placa: %s\n", p->placa);
            printf("Modelo: %s %s, %s\n", p->marca, p->modelo, p->cor);
            printf("Quilometragem: %.2f\n", p->km);
            printf("Valor Aluguel: %.2f\n\n", p->valor);
            break;
        }
    }
}

void listarCarros(Carro *carros, int cadastrados) {
    //recebe um vetor de carros e a quantidade de carros cadastrados.
    int i=1;
    if(cadastrados==0){
        printf("\n[ERRO] Nenhum carro cadastrado!\n\n");
        return;
    }
    for(Carro *p = carros; p < carros+cadastrados; p++){
        printf("\nCarro %d:\n", i); i++;
        printf("Placa: %s\n", p->placa);
        printf("Modelo: %s %s, %s\n", p->marca, p->modelo, p->cor);
        printf("Quilometragem: %.2f\n", p->km);
        printf("Valor Aluguel: %.2f\n\n", p->valor);
    }
}

void alterarCarro(Carro *carros, int cadastrados, char *placa) {
    //recebe uma lista de carros, a quantidade de cadastros e uma placa.
    if(!validarPlaca(placa)){
        printf("\n[ERRO] Placa Invalida!\n\n");
        return;
    }
    if(!placaExiste(carros, cadastrados, placa)){
        printf("\n[ERRO] Placa nao Cadastrada!\n\n");
        return;
    }
    for(Carro *p=carros; p<carros+cadastrados; p++){
        if(strcmp(placa, p->placa)==0){
            printf("%s\n", p->placa);
            printf("Nova marca do carro: "); scanf(" %[^\n]", p->marca);
            printf("Novo Modelo: "); scanf(" %[^\n]", p->modelo);
            printf("Nova cor: "); scanf(" %[^\n]", p->cor);
            printf("Nova Quilometragem: "); scanf("%f", &p->km);
            printf("Novo Valor do Aluguel: "); scanf("%f", &p->valor);
            break;
        }
    }
}

void excluirCarroPlaca(Carro *carros, int *cadastrados, char *placa) {
    //recebe uma lista de carros, a quantidade de cadastros e uma placa.
    Carro *encontrado = NULL;
    if(!validarPlaca(placa)){
        printf("\n[ERRO] Placa Invalida!\n");
        return;
    }
    for(Carro *p=carros; p<carros+*cadastrados; p++){
        if(strcmp(p->placa, placa) == 0){
            encontrado = p;
            break;
        }
    }
    if(encontrado == NULL){
        printf("\n[ERRO] Placa nao cadastrada!\n\n");
        return;
    }
    for(Carro *p=encontrado; p<carros+(*cadastrados-1); p++){
        *p = *(p + 1);
    }
    (*cadastrados)--;
    printf("\n[OK] Carro excluido com sucesso!\n\n");
}

int menu() {
    int op;

    printf("\n--- PAINEL DE CARROS ---\n");
    printf("1 - Cadastrar carro\n");
    printf("2 - listar carros\n");
    printf("3 - Excluir carro\n");
    printf("4 - Consultar carro pela placa\n");
    printf("5 - Alterar Carro pela placa\n");
    printf("0 - Sair\n");
    printf("Opcao: "); scanf("%d", &op);

    return op;
}

void submenuCarros(Carro carros[], int *cadastrados) {
    int opcao;

    do {
        printf("\n--- CONTROLE DE FROTA ---\n");
        printf("1. Cadastrar veiculo\n");
        printf("2. Listar frota\n");
        printf("3. Consultar veiculo\n");
        printf("4. Alterar veiculo\n");
        printf("5. Remover veiculo\n");
        printf("6. Voltar ao menu principal\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                if(*cadastrados >= MAX_CAR) {
                    printf("Limite atingido\n");
                    break;
                }

                if(cadastrarCarro(carros + *cadastrados, carros, *cadastrados)) {
                    (*cadastrados)++;
                }
                break;

            case 2:
                listarCarros(carros, *cadastrados);
                break;

            case 3: {
                char placa[8];
                printf("Digite a placa: ");
                scanf("%7s", placa);
                consultarCarro(carros, *cadastrados, placa);
                break;
            }

            case 4: {
                char placa[8];
                printf("Digite a placa: ");
                scanf("%7s", placa);
                alterarCarro(carros, *cadastrados, placa);
                break;
            }

            case 5: {
                char placa[8];
                printf("Digite a placa: ");
                scanf("%7s", placa);
                excluirCarroPlaca(carros, cadastrados, placa);
                break;
            }

            case 6:
                printf("Voltando...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 6);
}
