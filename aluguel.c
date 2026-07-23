#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


#include "aluguel.h"
#include "carro.h"

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
int salvarAlugueis(Aluguel *alugueis, int qtdAlugueis){

    FILE *arq = fopen("alugueis.bin", "wb");

    if(arq == NULL){
        return 0;
    }

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
     // Descobre o tamanho do arquivo
    fseek(arq, 0, SEEK_END);
    long tamanho = ftell(arq);
    rewind(arq);
     // Calcula a quantidade de registros
    *qtdAlugueis = tamanho / sizeof(Aluguel);

    if(*qtdAlugueis == 0){

        *alugueis = NULL;
        fclose(arq);
        return 1;
    }
    // Aloca exatamente a quantidade necessária
    *alugueis = malloc((*qtdAlugueis) * sizeof(Aluguel));
    if(*alugueis == NULL){

        fclose(arq);
        return 0;
    }
    // Aqui faz a leitura dos registros
    size_t lidos = fread(*alugueis,
                         sizeof(Aluguel),
                         *qtdAlugueis,
                         arq);
    fclose(arq);
    // Aqui é para conferir se leu tudo 
    if(lidos != *qtdAlugueis){

        free(*alugueis);
        *alugueis = NULL;
        *qtdAlugueis = 0;
        return 0;
    }
    return 1;
}
    
int cadastrarAluguel(Aluguel **alugueis, int *qtdAlugueis, Cliente *clientes, int qtdClientes, Carro *carros, int qtdCarros) {
    if (!aumentarVetorAlugueis(alugueis, *qtdAlugueis)) {
        return 0;
    }

    int pos = *qtdAlugueis;

    printf("\n--- CADASTRAR ALUGUEL ---\n");

    int codigo;
    do {
        printf("Codigo do aluguel: ");
        scanf("%d", &codigo);

        if (codigoExiste(*alugueis, *qtdAlugueis, codigo)) {
            printf("Codigo ja cadastrado!\n");
        }
    } while (codigoExiste(*alugueis, *qtdAlugueis, codigo));

    (*alugueis)[pos].codigo_aluguel = codigo;
    getchar(); // Limpa o buffer após o scanf

    char placa[8];
    Carro *indiceCarro = NULL;

    do {
        pedirPlaca(placa);
        if (!validarPlaca(placa)) {
            printf("[ERRO] Placa Invalida! Digite no formato correto (ABC1D23).\n\n");
            continue;
        }

        // Busca o carro no array passado pelo menu

        if (indiceCarro->alugado) {
            printf("[ERRO] Este carro ja esta alugado no momento!\n\n");
            return 0;
        }

    } while (!validarPlaca(placa));

    // Copia a placa validada para a estrutura do aluguel
    strcpy((*alugueis)[pos].placa, placa);

    printf("CPF do cliente: ");
    fgets((*alugueis)[pos].cpf_cliente, 15, stdin);
    (*alugueis)[pos].cpf_cliente[strcspn((*alugueis)[pos].cpf_cliente, "\n")] = '\0';

    do {
        printf("Quantidade de dias do aluguel: ");
        scanf("%d", &(*alugueis)[pos].dias);

        if ((*alugueis)[pos].dias <= 0) {
            printf("Quantidade de dias invalida!\n");
        }
    } while ((*alugueis)[pos].dias <= 0);

    // Cálculo do valor total usando a diária do carro encontrado
    (*alugueis)[pos].valor_total = (*alugueis)[pos].dias * indiceCarro->valor;

    printf("Valor total: R$ %.2lf\n", (*alugueis)[pos].valor_total);

    // Atualiza status do carro e salva
    indiceCarro->alugado = 1;
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
int alterarAluguel(Aluguel *alugueis, int qtdAlugueis, Carro *carros, int qtdCarros) {
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
            int cadastrados = 0;
            Carro *car = carregarCarros(&cadastrados);

            Carro *indiceCarro = buscarCarroPorPlaca(carros, cadastrados, car->placa);

    if(indiceCarro == NULL){
        printf("Carro nao encontrado.\n");
        return 0;
    }
            alugueis[i].valor_total = alugueis[i].dias * indiceCarro->valor;

            printf("Novo valor total: R$ %.2lf\n", alugueis[i].valor_total);
            
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
void atualizarAlugadoCliente(Cliente *clientes, int qtdClientes, const Aluguel *alugueis, int qtdAlugueis, const char *cpf){

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
}
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
                alterarAluguel(*alugueis, *qtdAlugueis, carros, qtdCarros);
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