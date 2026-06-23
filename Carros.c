#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_CAR 5

struct carro{
    char marca[50];
    char modelo[50];
    char cor[20];
    char placa[8];
    float km, valor;
};

int validarPlaca(char *placa){
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

int placaExiste(struct carro *carros, int cadastrados, char *placa){
    //recebe a lista de carros, a quantidade de carros cadastrados e uma placa, placa já cadastrada = 1, não cadastrada = 0.
    for(struct carro *p=carros; p<carros+cadastrados; p++){
        if(strcmp(placa, p->placa)==0){
            return 1;
        }
    }
    return 0;
}

int cadastrarCarro(struct carro *car, struct carro *carros, int cadastrados){
    //recebe uma posição de um vetor de carros, a lista de carros e a quantidade de carros.
    //placa invalida = 0, valida = 1;
    printf("Digite a placa no formato (ABC1D23): "); scanf("%7s", car->placa);
    
    if(validarPlaca(car->placa)){
        if(placaExiste(carros, cadastrados, car->placa)){
            printf("Placa ja cadastrada.\n\n");
            return 0;
        }
        else{
            printf("Placa aceita: %s\n", car->placa);
            printf("Digite a marca do carro: "); scanf(" %[^\n]", car->marca);
            printf("Digite o Modelo: "); scanf(" %[^\n]", car->modelo);
            printf("Digite a cor: "); scanf(" %[^\n]", car->cor);
            printf("Digite a Quilometragem: "); scanf("%f", &car->km);
            printf("Digite o Valor do aluguel: "); scanf("%f", &car->valor);
            printf("Carro cadastrado com sucesso!\n\n");
            return 1;
        }
    }
    else{
        printf("Placa Invalida!!\n\n");
        return cadastrarCarro(car, carros, cadastrados);
    }
}

void consultarCarro(struct carro *carros, int cadastrados, char *placa){
    //recebe uma lista de carros, a quantidade de cadastros e uma placa.
    if(!validarPlaca(placa)){
        printf("Placa Invalida!\n\n");
        return;
    }
    if(!placaExiste(carros, cadastrados, placa)){
        printf("Placa nao Cadastrada!\n\n");
        return;
    }
    for(struct carro *p=carros; p<carros+cadastrados; p++){
        if(strcmp(placa, p->placa)==0){
            printf("Placa: %s\n", p->placa);
            printf("Modelo: %s %s, %s\n", p->marca, p->modelo, p->cor);
            printf("Quilometragem: %.2f\n", p->km);
            printf("Valor Aluguel: %.2f\n\n", p->valor);
            break;
        }
    }
}

void listarCarros(struct carro *carros, int cadastrados){
    //recebe um vetor de carros e a quantidade de carros cadastrados.
    int i=1;
    if(cadastrados==0){
        printf("Nenhum carro cadastrado!\n\n");
        return;
    }
    for(struct carro *p = carros; p < carros+cadastrados; p++){
        printf("Carro %d:\n", i); i++;
        printf("Placa: %s\n", p->placa);
        printf("Modelo: %s %s, %s\n", p->marca, p->modelo, p->cor);
        printf("Quilometragem: %.2f\n", p->km);
        printf("Valor Aluguel: %.2f\n\n", p->valor);
    }
}

void alterarCarro(struct carro *carros, int cadastrados, char *placa){
    //recebe uma lista de carros, a quantidade de cadastros e uma placa.
    if(!validarPlaca(placa)){
        printf("Placa Invalida!\n\n");
        return;
    }
    if(!placaExiste(carros, cadastrados, placa)){
        printf("Placa nao Cadastrada!\n\n");
        return;
    }
    for(struct carro *p=carros; p<carros+cadastrados; p++){
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

void excluirCarroPlaca(struct carro *carros, int *cadastrados, char *placa){
    //recebe uma lista de carros, a quantidade de cadastros e uma placa.
    struct carro *encontrado = NULL;
    if(!validarPlaca(placa)){
        printf("Placa Invalida!\n");
        return;
    }
    for(struct carro *p=carros; p<carros+*cadastrados; p++){
        if(strcmp(p->placa, placa) == 0){
            encontrado = p;
            break;
        }
    }
    if(encontrado == NULL){
        printf("Placa nao cadastrada!\n\n");
        return;
    }
    for(struct carro *p=encontrado; p<carros+(*cadastrados-1); p++){
        *p = *(p + 1);
    }
    (*cadastrados)--;
    printf("Carro excluido com sucesso!\n\n");
}

int menu(){
    int op;

    printf("===== MENU =====\n");
    printf("1 - Cadastrar carro\n");
    printf("2 - listar carros\n");
    printf("3 - Excluir carro\n");
    printf("4 - Consultar carro pela placa\n");
    printf("5 - Alterar Carro pela placa\n");
    printf("0 - Sair\n");
    printf("Opcao: "); scanf("%d", &op);

    return op;
}

void main(){
    struct carro carros[MAX_CAR];
    int cadastrados=0;
    int op;
    char placa[8];
    do{
        op = menu();

        switch(op){
            case 1:
                if(cadastrados >= MAX_CAR){
                    printf("Limite maximo de carros atingido!\n\n");
                    break;
                }
                if(cadastrarCarro(carros + cadastrados, carros, cadastrados)){
                    cadastrados++;
                }
                break;
            case 2:
                listarCarros(carros, cadastrados);
                break;
            case 3:
                printf("Digite a placa a consultar: "); scanf("%7s", placa);
                excluirCarroPlaca(carros, &cadastrados, placa);
                break;
            case 4:
                printf("Digite a placa a consultar: "); scanf("%7s", placa);
                consultarCarro(carros, cadastrados, placa);
                break;
            case 5:
                printf("Digite a placa a consultar: "); scanf("%7s", placa);
                alterarCarro(carros, cadastrados, placa);
                break;
            case 0:
                printf("Encerrando...");
                break;
            default:
                printf("Opcao invalida.");
        }
    }while(op!=0);
}