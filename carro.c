#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "carro.h"
#include "util.h"

void pedirPlaca(char *placa){
    printf("\nDigite a placa no formato (ABC1D23): ");
    scanf("%7s", placa);
}

Carro* buscarCarroPorPlaca(Carro *carros, int cadastrados, char *placa){
    //Recebe a lista de carros cadastrados, a quantidade de carros cadastrados e uma placa;
    //Se a placa informada for igual a placa de algum carro cadastrado, retorna um ponteiro para o carro correspondente;
    //Se não encontrar nenhuma placa compativel retorna NULL.
    for(Carro *p = carros; p < (carros + cadastrados); p++){
        if(strcmp(p->placa, placa) == 0){
            return p;
        }
    }
    return NULL;
}

void exibirDadosCarro(Carro *carro){
    printf("\n\nPlaca: %s\n", carro->placa);
    printf("Modelo: %s %s, %s\n", carro->marca, carro->modelo, carro->cor);
    printf("Quilometragem: %.2f\n", carro->km);
    printf("Valor Aluguel: %.2f\n\n", carro->valor);
}

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

int cadastrarCarro(Carro *carros, int cadastrados) {
    //recebe uma lista de carros e a quantidade de carros cadastrados.
    //placa invalida = 0, valida = 1;
    Carro *car = carros + cadastrados;
    pedirPlaca(car->placa);
    
    if(validarPlaca(car->placa)){
        if(buscarCarroPorPlaca(carros, cadastrados, car->placa) != NULL){
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
        return cadastrarCarro(carros, cadastrados);
    }
}

void consultarCarro(Carro *carros, int cadastrados) {
    //recebe uma lista de carros, a quantidade de cadastros e uma placa;
    //usa as funções validarPlaca e buscarCarroPorPlaca para verificar a validade da placa e se já foi cadastrada;
    //Se a Placa já tiver sido cadastrada usa a função exibirDadosCarro e exibe as caracteristicas do carro.
    char placa[8];
    pedirPlaca(placa);

    if(!validarPlaca(placa)){
        printf("\n[ERRO] Placa Invalida!\n\n");
        return;
    }
    Carro *encontrado = buscarCarroPorPlaca(carros, cadastrados, placa);
    if(encontrado == NULL){
        printf("\n[ERRO] Placa nao Cadastrada!\n\n");
        return;
    }
    exibirDadosCarro(encontrado);
}

void listarCarros(Carro *carros, int cadastrados) {
    //recebe um vetor de carros e a quantidade de carros cadastrados.
    int i=1;
    if(cadastrados==0){
        printf("\n[ERRO] Nenhum carro cadastrado!\n\n");
        return;
    }
    for(Carro *p = carros; p < carros+cadastrados; p++){
        printf("\nCarro %d:", i++);
        exibirDadosCarro(p);
    }
}

void alterarCarro(Carro *carros, int cadastrados) {
    //recebe uma lista de carros, a quantidade de cadastros e uma placa.
    char placa[8];
    pedirPlaca(placa);

    if(!validarPlaca(placa)){
        printf("\n[ERRO] Placa Invalida!\n\n");
        return;
    }
    Carro *encontrado = buscarCarroPorPlaca(carros, cadastrados, placa);
    if(encontrado == NULL){
        printf("\n[ERRO] Placa nao Cadastrada!\n\n");
        return;
    }
    printf("\n\nO que voce deseja alterar ?\n");
    printf("1. Marca\n");
    printf("2. Modelo\n");
    printf("3. Quilometragem\n");
    printf("4. Valor do aluguel\n\n");
    int opcao;
    printf("Opcao: "); scanf("%d", &opcao);

    switch(opcao) {
        case 1:
            limparBuffer();
            printf("Digite a nova marca: "); scanf("%[^\n]", encontrado->marca);
            break;
        case 2:
            limparBuffer();
            printf("Digite o novo modelo: "); scanf("%[^\n]", encontrado->modelo);
            break;
        case 3:
            printf("Digite a nova Quilometragem: "); scanf("%f", &encontrado->km);
            break;
        case 4:
            printf("Digite o novo valor do aluguel: "); scanf("%f", &encontrado->valor);
            break;
        default:
            printf("Opcao invalida!\n");
            break;
    }
}

void excluirCarroPlaca(Carro *carros, int *cadastrados) {
    //recebe uma lista de carros, a quantidade de cadastros e uma placa.
    char placa[8];
    pedirPlaca(placa);

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

                if(cadastrarCarro(carros, *cadastrados)) {
                    (*cadastrados)++;
                }
                break;

            case 2:
                listarCarros(carros, *cadastrados);
                break;

            case 3: {
                consultarCarro(carros, *cadastrados);
                break;
            }

            case 4: {
                alterarCarro(carros, *cadastrados);
                break;
            }

            case 5: {
                excluirCarroPlaca(carros, cadastrados);
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
