#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


#include "aluguel.h"

//*Verifica se o funcionario não esta usando um código repetido para cadastrar o aluguel.
int codigoExiste(Aluguel *alugueis, int qtdAlugueis, int codigo) {
    for(int i = 0; i < qtdAlugueis; i++) {
        if(alugueis[i].codigo_aluguel == codigo) {
            return 1;
        }
    }

    return 0;
}
//Verifica se o cpf que esta sendo usado ja possui algum aluguel ligado a ele
int existeAluguelPorCpf(const Aluguel *alugueis, int qtdAlugueis, const char *cpf){

    for(int i = 0; i < qtdAlugueis; i++){

        if(strcmp(alugueis[i].cpf_cliente, cpf) == 0){
            return 1;
        }
    }
    return 0;
}
//Verifica se um carro espeficio ja esta alugado através de sua placa
int existeAluguelPorPlaca(const Aluguel *alugueis, int qtdAlugueis, const char *placa){

    for(int i = 0; i < qtdAlugueis; i++){

        if(strcmp(alugueis[i].placa, placa) == 0){
            return 1;
        }
    }

    return 0;
}
/*Além de possuir a listagem de todos alugueis, nessa função podemos procurar um aluguel especifico através do código
o que acaba facilitando na hora para o funcionário.*/
void buscarAluguelporcodigo(Aluguel *alugueis, int qtdAlugueis) {
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
            printf("Dias alugados: %d\n", alugueis[i].dias);
            printf("Valor total: %.2lf\n", alugueis[i].valor_total);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("Aluguel nao encontrado.\n");
    }
}
//essa função substitui o vetor fixo Maximocadastros.
int aumentarVetorAlugueis(Aluguel **alugueis, int qtdAlugueis){

    Aluguel *novo;

    novo = realloc(*alugueis, (qtdAlugueis + 1) * sizeof(Aluguel));

    if(novo == NULL){

        printf("\nErro de alocacao de memoria.\n");
        return 0;
    }

    *alugueis = novo;

    return 1;
}
int salvarAlugueis(const Aluguel *alugueis, int qtdAlugueis){

    FILE *arq = fopen("alugueis.bin", "wb");

    if(arq == NULL){
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    fwrite(&qtdAlugueis, sizeof(int), 1, arq);

    fwrite(alugueis,
           sizeof(Aluguel),
           qtdAlugueis,
           arq);

    fclose(arq);
    return 1;
}
int carregarAlugueis(Aluguel **alugueis, int *qtdAlugueis){

    FILE *arq = fopen("alugueis.bin", "rb");

    if(arq == NULL){
        *alugueis = NULL;
        *qtdAlugueis = 0;
        return 0;
    }

    fread(qtdAlugueis, sizeof(int), 1, arq);

    if(*qtdAlugueis > 0){

        *alugueis = malloc((*qtdAlugueis) * sizeof(Aluguel));

        if(*alugueis == NULL){
            fclose(arq);
            return 0;
        }

        fread(*alugueis,
              sizeof(Aluguel),
              *qtdAlugueis,
              arq);
    }
    else{
        *alugueis = NULL;
    }

    fclose(arq);
    return 1;
}
    
int cadastrarAluguel(Aluguel **alugueis, int *qtdAlugueis, Cliente *clientes, int qtdClientes, Carro *carros, int qtdCarros){

    if(!aumentarVetorAlugueis(alugueis, *qtdAlugueis)){
        return 0;
    }

    int pos = *qtdAlugueis;

    printf("\n--- CADASTRAR ALUGUEL ---\n");

    int codigo;

    do {
        printf("Codigo do aluguel: ");
        scanf("%d", &codigo);

        if(codigoExiste(*alugueis, *qtdAlugueis, codigo)) {
            printf("Codigo ja cadastrado!\n");
        }

 } while(codigoExiste(*alugueis, *qtdAlugueis, codigo));

    (*alugueis)[pos].codigo_aluguel = codigo;
    getchar();

    int validarPlaca = 0;

    while(!validarPlaca) {
        printf("Placa do carro: ");
        fgets((*alugueis)[pos].placa, 8, stdin);

        (*alugueis)[pos].placa[strcspn((*alugueis)[pos].placa, "\n")] = '\0';

        if(strlen((*alugueis)[pos].placa) != 7) {
            printf("Placa invalida!\n");
            continue;
        }

        int ok_placa = 1;

        for(int i = 0; i < 7; i++) {
            if(i < 3) {
                if(!isalpha((*alugueis)[pos].placa[i])) {
                    ok_placa = 0;
                }
            } else if(i == 3) {
                if(!isdigit((*alugueis)[pos].placa[i])) {
                    ok_placa = 0;
                }
            } else if(i == 4) {
                if(!isalpha((*alugueis)[pos].placa[i])) {
                    ok_placa = 0;
                }
            } else {
                if(!isdigit((*alugueis)[pos].placa[i])) {
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
    //Procura o carro correspondente a placa informada.
    int indiceCarro = -1;

    for(int i = 0; i < qtdCarros; i++){

    if(strcmp(carros[i].placa, (*alugueis)[pos].placa) == 0){

        indiceCarro = i;
        break;
    }
}

    if(indiceCarro == -1){

    printf("Carro nao encontrado.\n");
    return 0;
}

    getchar();

    getchar();

    printf("CPF do cliente: ");
    fgets((*alugueis)[pos].cpf_cliente,15,stdin);
    (*alugueis)[pos].cpf_cliente[strcspn((*alugueis)[pos].cpf_cliente,"\n")]='\0';

    do{

    printf("Quantidade de dias do aluguel: ");
    scanf("%d", &(*alugueis)[pos].dias);

    if((*alugueis)[pos].dias <= 0){
    printf("Quantidade de dias invalida!\n");
   }

     }while((*alugueis)[pos].dias <= 0);

     //Faz a multiplicação automatica do valor do aluguel;
     (*alugueis)[pos].valor_total = (*alugueis)[pos].dias * carros[indiceCarro].valor;

    printf("Valor total: R$ %.2lf\n ", (*alugueis)[pos].valor_total);
    
    (*qtdAlugueis)++;

    salvarAlugueis(*alugueis, *qtdAlugueis);

    printf("Aluguel cadastrado com sucesso!\n");
    return 1;
}

//a função lista todos alugueis que ja foram adicionados, mostrando todos os dados. 
void listarAlugueis(const Aluguel *alugueis, int qtdAlugueis) {
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
        printf("Dias alugados: %d\n", alugueis[i].dias);
        printf("Valor total: %.2lf\n", alugueis[i].valor_total);
    }
}

/*esta é a função para alterar o aluguel caso necessário, como por exemplo 
algum erro de digitação cometido em algum dos campos.*/
int alterarAluguel(Aluguel *alugueis, int qtdAlugueis) {
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

            printf("Nova quantidade de dias: ");
            scanf("%d", &alugueis[i].dias);

            printf("Novo valor total: ");
            scanf("%lf", &alugueis[i].valor_total);

            encontrado = 1;

            break; //* para o loop porque encontrou o registro
        }
    }
    if(!encontrado){
        printf("Aluguel nao encontrado.\n");
        return 0;
    }
    salvarAlugueis(alugueis, qtdAlugueis);
    return 1;
}

int excluirAluguel(Aluguel **alugueis, int *qtdAlugueis) {
    int id;
    int encontrado = 0;

    printf("Digite o codigo do aluguel que deseja excluir: ");
    scanf("%d", &id);
    for (int i = 0; i < *qtdAlugueis; i++) {

        if ((*alugueis)[i].codigo_aluguel == id) {

            for (int j = i; j < *qtdAlugueis - 1; j++) {
                (*alugueis)[j] = (*alugueis)[j + 1];
            }
            (*qtdAlugueis)--;
            encontrado = 1;

            printf("Aluguel excluido com sucesso!\n");
            break;
        }
    }
    if (!encontrado) {
        printf("Aluguel nao encontrado.\n");
        return 0;
    }
    salvarAlugueis(*alugueis, *qtdAlugueis);
    return 1;
}
//Sempre que um aluguel for criado ou removido esta função atualiza a variável alugado.
/*void atualizarAlugadoCliente(Cliente *clientes, int qtdClientes, const Aluguel *alugueis, int qtdAlugueis, const char *cpf){

    int possui = existeAluguelPorCpf(alugueis,
                                     qtdAlugueis,
                                     cpf);

    int pos = buscarPorCpf(clientes,
                           qtdClientes,
                           (char *)cpf);

    if(pos != -1){

        if(possui){
            clientes[pos].alugado = 1;
        }
        else{
            clientes[pos].alugado = 0;
        }
    }
}*/
//Atualiza a situação de um carro qualquer sempre que ele é alugado ou quando ele fica livre para aluguel

void atualizarAlugadoCarro(Carro *carros, int qtdCarros, const Aluguel *alugueis, int qtdAlugueis, const char *placa){

    int possui = existeAluguelPorPlaca(alugueis,
                                       qtdAlugueis,
                                       placa);

    for(int i = 0; i < qtdCarros; i++){

        if(strcmp(carros[i].placa, placa) == 0){

            if(possui){
                carros[i].alugado = 1;
            }
            else{
                carros[i].alugado = 0;
            }

            return;
        }
    }
}

void menuAluguel(Aluguel **alugueis, int *qtdAlugueis, Cliente *clientes, int qtdClientes, Carro *carros, int qtdCarros)
{
    int opcao;

    do{

        printf("\n--- MENU ALUGUEL ---\n");
        printf("1 - Cadastrar aluguel\n");
        printf("2 - Listar alugueis\n");
        printf("3 - Alterar aluguel\n");
        printf("4 - Excluir aluguel\n");
        printf("5 - Buscar aluguel\n");
        printf("0 - Voltar ao menu principal\n");
        printf("Opcao: ");
        scanf("%d",&opcao);

        switch(opcao){

            case 1:
                cadastrarAluguel(alugueis,
                                 qtdAlugueis,
                                 clientes,
                                 qtdClientes,
                                 carros,
                                 qtdCarros);
                break;
            case 2:
                listarAlugueis(*alugueis,
                               *qtdAlugueis);
                break;
            case 3:
                alterarAluguel(*alugueis,
                               *qtdAlugueis);
                break;
            case 4:
                excluirAluguel(alugueis,
                               qtdAlugueis);
                break;
            case 5:
                buscarAluguelporcodigo(*alugueis,
                                       *qtdAlugueis);
                break;
            case 0:
                break;

            default:
                printf("ERRO Opcao invalida!\n");
        }

    }while(opcao != 0);
}

void submenuAlugueis(Aluguel **alugueis,
                     int *qtdAlugueis,
                     Cliente *clientes,
                     int qtdClientes,
                     Carro *carros,
                     int qtdCarros)
{
    menuAluguel(alugueis,
                qtdAlugueis,
                clientes,
                qtdClientes,
                carros,
                qtdCarros);
}