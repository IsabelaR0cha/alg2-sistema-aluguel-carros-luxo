#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "carro.h"
#include "util.h"



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

int validarMarca(char *marca){
    stringParaMinusculo(marca);
    const char *marcas_validas[] = {
        "porsche", "ferrari", "lamborghini", "bmw", 
        "mercedes", "audi", "land rover", "volvo"
    };
    int total_marcas = 8;

    for(int i = 0; i < total_marcas; i++){
        if(strcmp(marca, marcas_validas[i]) == 0){
            return 1;
        }
    }
    return 0;
}

int validarModelo(char *modelo){
    //Recebe uma string e verifica se possui mais de dois caracteres,  
    //se possui ao menos uma letra e se não é constituida apenas por espaços.
    //Se for valida retorna 1 se não retorna 0.
    stringParaMinusculo(modelo);
    if(strlen(modelo) < 2){
        return 0;
    }
    int tem_letra = 0;
    int so_espacos = 1;
    for(int i = 0; modelo[i] != '\0'; i++){
        if(isalpha((unsigned char)modelo[i])){
            tem_letra = 1;
        }
        if(!isspace((unsigned char)modelo[i])){
            so_espacos = 0;
        }
    }
    if(!tem_letra || so_espacos){
        return 0;
    }
    return 1;
}

int validarCor(char *cor){
    stringParaMinusculo(cor);
    if(strlen(cor) < 3){
        return 0;
    }
    int tem_numero = 0;
    int so_espacos = 1;
    for(int i = 0; cor[i] != '\0'; i++){
        if(isdigit((unsigned char)cor[i])){
            tem_numero = 1;
        }
        if(!isspace((unsigned char)cor[i])){
            so_espacos = 0;
        }
    }
    if(so_espacos || tem_numero){
        return 0;
    }
    return 1;
}


void pedirPlaca(char *placa){
    printf("\nDigite a placa no formato (ABC1D23): ");
    scanf("%7s", placa);
    limparBuffer();//O %7s deixa o \n no buffer, por isso é importante fazer a limpeza
}

void exibirDadosCarro(Carro *carro){
    printf("\nPlaca: %s\n", carro->placa);
    printf("Modelo: %s %s, %s\n", carro->marca, carro->modelo, carro->cor);
    printf("Quilometragem: %.2f\n", carro->km);
    printf("Valor Aluguel: %.2f\n\n", carro->valor);
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


int cadastrarCarro(Carro *carros, int cadastrados) {
    // Recebe o vetor de carros e o total de cadastros para registrar um novo veículo.
    // Valida a placa, marca, modelo, cor, quilometragem e aluguel, forçando redigitação em caso de erro.
    // Retorna 1 para sucesso ou 0 se a placa já existir no sistema.

    Carro *car = carros + cadastrados; //Calcula a posição do vetor em que o carro sera cadastrado.
    
    do {
        pedirPlaca(car->placa);
        if (!validarPlaca(car->placa)) {
            printf("[ERRO] Placa Invalida! Digite no formato correto (ABC1D23).\n\n");
            continue;
        }
        
        if (buscarCarroPorPlaca(carros, cadastrados, car->placa) != NULL) {
            printf("[ERRO] Placa ja cadastrada no sistema.\n\n");
            return 0;
        }
    } while (!validarPlaca(car->placa));
    
    printf("[OK] Placa aceita: %s\n", car->placa);

    do {
        printf("\n--- MARCAS DISPONIVEIS ---\n");
        printf("[ Porsche | Ferrari | Lamborghini | BMW | Mercedes | Audi | Land Rover | Volvo ]\n\n");
        printf("Digite a marca do carro: "); 
        scanf(" %[^\n]", car->marca);

        if (!validarMarca(car->marca)) {
            printf("[ERRO] Marca nao cadastrada ou erro de digitacao.\n\n");
        }
    } while (!validarMarca(car->marca));

    do {
        printf("Digite o Modelo: ");
        scanf(" %[^\n]", car->modelo);

        if (!validarModelo(car->modelo)) {
            printf("[ERRO] Modelo invalido! Deve conter letras e pelo menos 2 caracteres.\n\n");
        }
    } while (!validarModelo(car->modelo));

    do {
        printf("Digite a cor: "); 
        scanf(" %[^\n]", car->cor);

        if (!validarCor(car->cor)) {
            printf("[ERRO] Cor invalida! Use apenas letras e no minimo 3 caracteres.\n\n");
        }
    } while (!validarCor(car->cor));

    do {
        printf("Digite a Quilometragem: "); 
        if (scanf("%f", &car->km) != 1 || car->km < 0) {
            printf("[ERRO] Quilometragem invalida! Digite um valor maior ou igual a zero.\n\n");
            limparBuffer(); // Limpa o buffer caso o usuário digite letras por engano, previnindo um loop de erros;
            car->km = -1;   // Força a repetição do laço, ja que se uma letra dor digitada, o valor armazenado é imprevisivel.
        }
    } while (car->km < 0);

    do {
        printf("Digite o Valor do aluguel: "); 
        if (scanf("%f", &car->valor) != 1 || car->valor <= 0) {
            printf("[ERRO] Valor invalido! O aluguel deve ser maior do que zero.\n\n");
            limparBuffer();
            car->valor = -1;//mesma logica utilizada na validação de quilometragem
        }
    } while (car->valor <= 0);

    printf("[OK] Carro cadastrado com sucesso!\n");
    car->alugado = 1;
    return 1;
}

void consultarCarro(Carro *carros, int cadastrados) {
    //recebe uma lista de carros, a quantidade de cadastros e uma placa;
    //usa as funções validarPlaca e buscarCarroPorPlaca para verificar a validade da placa e se já foi cadastrada;
    //Se a Placa já tiver sido cadastrada usa a função exibirDadosCarro e exibe as caracteristicas do carro.
    char placa[8];
    Carro *encontrado = NULL;

    do{
        pedirPlaca(placa);
        if(!validarPlaca(placa)){
            printf("[ERRO] Placa Invalida! Digite no formato correto (ABC1D23).\n\n");
            continue;
        }
        encontrado = buscarCarroPorPlaca(carros, cadastrados, placa);
        if(encontrado == NULL){
            printf("[ERRO] Placa nao cadastrada no sistema.\n\n");
        }
    } while(!validarPlaca(placa) || encontrado == NULL);

    printf("[OK] Veiculo encontrado!\n");
    exibirDadosCarro(encontrado);
}

void listarCarros(Carro *carros, int cadastrados) {
    //recebe um vetor de carros e a quantidade de carros cadastrados.
    int i=1;
    if(cadastrados==0){
        printf("[ERRO] Nenhum carro cadastrado!\n\n");
        return;
    }
    for(Carro *p = carros; p < carros+cadastrados; p++){
        printf("\nCarro %d:", i++);
        exibirDadosCarro(p);
    }
}

void alterarCarro(Carro *carros, int cadastrados) {
    char placa[8];
    Carro *encontrado = NULL;

    do {
        pedirPlaca(placa);
        if (!validarPlaca(placa)) {
            printf("[ERRO] Placa Invalida! Digite no formato correto (ABC1D23).\n\n");
            continue; 
        }

        encontrado = buscarCarroPorPlaca(carros, cadastrados, placa);
        if (encontrado == NULL) {
            printf("[ERRO] Veiculo nao cadastrado! Tente outra placa.\n\n");
        }
    } while (!validarPlaca(placa) || encontrado == NULL);

    printf("[OK] Veiculo encontrado!\n");
    exibirDadosCarro(encontrado);

    printf("O que voce deseja alterar ?\n");
    printf("1. Marca\n");
    printf("2. Modelo\n");
    printf("3. Cor\n");
    printf("4. Quilometragem\n");
    printf("5. Valor do aluguel\n");
    printf("6. Cancelar operacao\n\n");
    
    int opcao;
    printf("Opcao: "); 
    scanf("%d", &opcao);
    limparBuffer();

    switch(opcao) {
        case 1:
            do {
                printf("\n--- MARCAS DISPONIVEIS ---\n");
                printf("[ Porsche | Ferrari | Lamborghini | BMW | Mercedes | Audi | Land Rover | Volvo ]\n\n");
                printf("Digite a nova marca: ");
                scanf(" %[^\n]", encontrado->marca);

                if (!validarMarca(encontrado->marca)) {
                    printf("[ERRO] Marca nao cadastrada ou erro de digitacao. Tente novamente.\n\n");
                }
            } while (!validarMarca(encontrado->marca));
            printf("[OK] Marca atualizada com sucesso!\n");
            break;

        case 2:
            do {
                printf("Digite o novo modelo: ");
                scanf(" %[^\n]", encontrado->modelo);

                if (!validarModelo(encontrado->modelo)) {
                    printf("[ERRO] Modelo invalido! Deve conter letras e pelo menos 2 caracteres.\n\n");
                }
            } while (!validarModelo(encontrado->modelo));
            printf("[OK] Modelo atualizado com sucesso!\n");
            break;

        case 3:
            do {
                printf("Digite a nova cor: ");
                scanf(" %[^\n]", encontrado->cor);

                if (!validarCor(encontrado->cor)) {
                    printf("[ERRO] Cor invalida! Use apenas letras e no minimo 3 caracteres.\n\n");
                }
            } while (!validarCor(encontrado->cor));
            printf("[OK] Cor atualizada com sucesso!\n\n");
            break;

        case 4:
            do {
                printf("Digite a nova Quilometragem: "); 
                if (scanf("%f", &encontrado->km) != 1 || encontrado->km < 0) {
                    printf("[ERRO] Quilometragem invalida! Digite um valor maior ou igual a zero.\n\n");
                    limparBuffer();
                    encontrado->km = -1; 
                }
            } while (encontrado->km < 0);
            printf("[OK] Quilometragem atualizada com sucesso!\n");
            break;

        case 5:
            do {
                printf("Digite o novo valor do aluguel: "); 
                if (scanf("%f", &encontrado->valor) != 1 || encontrado->valor <= 0) {
                    printf("[ERRO] Valor invalido! O aluguel deve ser maior do que zero.\n\n");
                    limparBuffer();
                    encontrado->valor = -1;
                }
            } while (encontrado->valor <= 0);
            printf("[OK] Valor do aluguel atualizado com sucesso!\n");
            break;

        case 6:
            printf("Operacao cancelada. Nenhum dado foi alterado.\n\n");
            break;

        default:
            printf("Opcao invalida! Retornando ao menu principal.\n\n");
            break;
    }
}

void excluirCarroPlaca(Carro *carros, int *cadastrados) {
    char placa[8];
    Carro *encontrado = NULL;

    do {
        pedirPlaca(placa);
        if(!validarPlaca(placa)){
            printf("[ERRO] Placa Invalida!\n\n");
            continue;
        }
        encontrado = buscarCarroPorPlaca(carros, *cadastrados, placa);
        if(encontrado == NULL){
            printf("[ERRO] Placa nao cadastrada!\n\n");
        }
    } while(!validarPlaca(placa) || encontrado == NULL);
    if(encontrado->alugado){
        for(Carro *p = encontrado; p < carros + (*cadastrados - 1); p++){
            *p = *(p + 1);
        }
        (*cadastrados)--;
        printf("[OK] Carro excluido com sucesso!\n\n");
        }
    else{
        printf("[ERRO] Veiculo alugado! Não é possivel concluir a exclusao.");
    }
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
        limparBuffer();

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
