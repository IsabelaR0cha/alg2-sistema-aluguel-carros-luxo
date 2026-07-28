#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "carro.h"
#include "util.h"

// 1. Funções de Validação
int validarPlaca(char *placa)
{
    // recebe uma placa e verifica se esta no fomato Mercosul e padroniza os caracteres em Maiusculos, valida = 1, invalida = 0.
    char *p = placa;
    if (strlen(placa) != 7)
    {
        return 0;
    }
    if (isalpha(*p) == 0 || isalpha(*(p + 1)) == 0 || isalpha(*(p + 2)) == 0 || isdigit(*(p + 3)) == 0 ||
        isalpha(*(p + 4)) == 0 || isdigit(*(p + 5)) == 0 || isdigit(*(p + 6)) == 0)
    {
        return 0;
    }
    else
    {
        for (; p < placa + 7; p++)
        {
            if (isalpha(*p))
            {
                *p = toupper(*p);
            }
        }
        return 1;
    }
}

int validarMarca(char *marca)
{
    // recebe uma string, padroniza para minusculo e faz uma comparação item a item com o vetor constante de marcas pré definidas;
    // se a string for equivalente a alguma das marcas retorna 1 se não retona 0.
    stringParaMinusculo(marca);
    const char *marcas_validas[] = {
        "porsche", "ferrari", "lamborghini", "bmw",
        "mercedes", "audi", "land rover", "volvo"};
    int total_marcas = 8;

    for (int i = 0; i < total_marcas; i++)
    {
        if (strcmp(marca, marcas_validas[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int validarModelo(char *modelo)
{
    // Recebe uma string a padroniza em minusculo e verifica se possui mais de dois caracteres,
    // se possui ao menos uma letra e se não é constituida apenas por espaços.
    // Se for valida retorna 1 se não retorna 0.
    stringParaMinusculo(modelo);
    if (strlen(modelo) < 2)
    {
        return 0;
    }
    int tem_letra = 0;
    int so_espacos = 1;
    for (int i = 0; modelo[i] != '\0'; i++)
    {
        if (isalpha((unsigned char)modelo[i]))
        {
            tem_letra = 1;
        }
        if (!isspace((unsigned char)modelo[i]))
        {
            so_espacos = 0;
        }
    }
    if (!tem_letra || so_espacos)
    {
        return 0;
    }
    return 1;
}

int validarCor(char *cor)
{
    // Recebe uma string a padroniza em minusculo e verifica se possui mais de três caracteres,
    // se possui apenas letras e se não é constituida apenas por espaços.
    // Se for valida retorna 1 se não retorna 0.
    stringParaMinusculo(cor);
    if (strlen(cor) < 3)
    {
        return 0;
    }
    int tem_numero = 0;
    int so_espacos = 1;
    for (int i = 0; cor[i] != '\0'; i++)
    {
        if (isdigit((unsigned char)cor[i]))
        {
            tem_numero = 1;
        }
        if (!isspace((unsigned char)cor[i]))
        {
            so_espacos = 0;
        }
    }
    if (so_espacos || tem_numero)
    {
        return 0;
    }
    return 1;
}

// 2. Funções Auxiliares
void pedirPlaca(char *placa)
{
    // recebe uma string e a preeche com uma entrada de usuario;
    // limita a escrita de 7 caracteres;
    // Como %7s deixa o \n no buffer, é importante fazer a limpeza.
    printf("\nDigite a placa no formato (ABC1D23): ");
    scanf("%7s", placa);
    limparBuffer();
}

void exibirDadosCarro(Carro *carro)
{
    // Recebe os dados de um carro e imprime.
    printf("\nPlaca: %s\n", carro->placa);
    printf("Modelo: %s %s, %s\n", carro->marca, carro->modelo, carro->cor);
    printf("Quilometragem: %.2f\n", carro->km);
    printf("Valor Aluguel: %.2f\n\n", carro->valor);
}

Carro *buscarCarroPorPlaca(Carro *carros, int cadastrados, char *placa)
{
/*
  @brief [buscarCarroPorPlaca] Realiza a busca linear de um veículo no vetor dinâmico a partir da sua placa.
 
  Percorre a memória do vetor de carros utilizando aritmética de ponteiros e compara 
  a placa informada com a placa de cada registro cadastrado através de strcmp.
 
  @param carros Ponteiro simples para o início do vetor de estruturas Carro na memória.
  @param cadastrados Quantidade total de carros cadastrados atualmente no sistema.
  @param placa Ponteiro para a string contendo a placa a ser buscada.
  @return Carro* Retorna o ponteiro para o elemento Carro encontrado, ou NULL se não existir.
*/
    for (Carro *p = carros; p < (carros + cadastrados); p++)
    {
        if (strcmp(p->placa, placa) == 0)
        {
            return p;
        }
    }
    return NULL;
}

// 3. Cadastrar Carro com Alocação Dinâmica
int cadastrarCarro(Carro **carros, int *cadastrados)
{
/*
  @brief [cadastrarCarro] Cadastra um novo veículo expandindo dinamicamente a memória do vetor.
 
  Utiliza realloc() para incrementar o tamanho do vetor na memória para (cadastrados + 1).
  Lê e valida sequencialmente: placa (verificando duplicidade no sistema), marca, modelo, 
  cor, quilometragem e valor do aluguel usando laços do-while para forçar a redigitação 
  em caso de erro. Define a flag alugado = 1 (disponível/sem aluguel ativo) e incrementa 
  o ponteiro do contador de cadastrados (*cadastrados)++.
 
  @param carros Ponteiro duplo para o vetor dinâmico de Carro (permite atualizar a referência em caso de realloc).
  @param cadastrados Ponteiro para o número total de veículos cadastrados.
  @return int Retorna 1 após o cadastro ser concluído com sucesso ou 0 se a placa já estiver cadastrada.
*/
    Carro *temp = (Carro *)realloc(*carros, (*cadastrados + 1) * sizeof(Carro));
    if (temp == NULL)
    {
        printf("[ERRO] Falha ao alocar memoria para o novo carro!\n\n");
        exit(1);
    }
    *carros = temp;

    Carro *car = *carros + *cadastrados; // Calcula a posição do vetor em que o carro sera cadastrado.

    do
    {
        pedirPlaca(car->placa);
        if (!validarPlaca(car->placa))
        {
            printf("[ERRO] Placa Invalida! Digite no formato correto (ABC1D23).\n\n");
            continue;
        }

        if (buscarCarroPorPlaca(*carros, *cadastrados, car->placa) != NULL)
        {
            printf("[ERRO] Placa ja cadastrada no sistema.\n\n");
            return 0;
        }
    } while (!validarPlaca(car->placa));

    printf("[OK] Placa aceita: %s\n", car->placa);

    do
    {
        printf("\n--- MARCAS DISPONIVEIS ---\n");
        printf("[ Porsche | Ferrari | Lamborghini | BMW | Mercedes | Audi | Land Rover | Volvo ]\n\n");
        printf("Digite a marca do carro: ");
        scanf(" %[^\n]", car->marca);

        if (!validarMarca(car->marca))
        {
            printf("[ERRO] Marca nao cadastrada ou erro de digitacao.\n\n");
        }
    } while (!validarMarca(car->marca));

    do
    {
        printf("Digite o Modelo: ");
        scanf(" %[^\n]", car->modelo);

        if (!validarModelo(car->modelo))
        {
            printf("[ERRO] Modelo invalido! Deve conter letras e pelo menos 2 caracteres.\n\n");
        }
    } while (!validarModelo(car->modelo));

    do
    {
        printf("Digite a cor: ");
        scanf(" %[^\n]", car->cor);

        if (!validarCor(car->cor))
        {
            printf("[ERRO] Cor invalida! Use apenas letras e no minimo 3 caracteres.\n\n");
        }
    } while (!validarCor(car->cor));

    do
    {
        printf("Digite a Quilometragem: ");
        if (scanf("%f", &car->km) != 1 || car->km < 0)
        {
            printf("[ERRO] Quilometragem invalida! Digite um valor maior ou igual a zero.\n\n");
            limparBuffer(); // Limpa o buffer caso o usuário digite letras por engano, previnindo um loop de erros;
            car->km = -1;   // Força a repetição do laço, ja que se uma letra dor digitada, o valor armazenado é imprevisivel.
        }
    } while (car->km < 0);

    do
    {
        printf("Digite o Valor do aluguel: ");
        if (scanf("%f", &car->valor) != 1 || car->valor <= 0)
        {
            printf("[ERRO] Valor invalido! O aluguel deve ser maior do que zero.\n\n");
            limparBuffer();
            car->valor = -1; // mesma logica utilizada na validação de quilometragem
        }
    } while (car->valor <= 0);

    car->alugado = 1;
    (*cadastrados)++;
    printf("[OK] Carro cadastrado com sucesso!\n");
    return 1;
}

// 4. Consultar Carro pela Placa
void consultarCarro(Carro *carros, int cadastrados)
{
/*
  @brief [consultarCarro] Consulta e exibe as informações de um carro específico informando a placa.
 
  Verifica se existem veículos cadastrados na memória. Solicita e valida a placa via teclado, 
  realiza a busca com buscarCarroPorPlaca() e, se localizado, exibe os dados utilizando a 
  função exibirDadosCarro().
 
  @param carros Ponteiro simples para o vetor de estruturas Carro.
  @param cadastrados Quantidade total de veículos cadastrados na memória RAM.
*/
    char placa[8];
    Carro *encontrado = NULL;

    if (cadastrados == 0 || carros == NULL)
    {
        printf("[ERRO] Nenhum carro cadastrado!\n\n");
        return;
    }

    do
    {
        pedirPlaca(placa);
        if (!validarPlaca(placa))
        {
            printf("[ERRO] Placa Invalida! Digite no formato correto (ABC1D23).\n\n");
            continue;
        }
        encontrado = buscarCarroPorPlaca(carros, cadastrados, placa);
        if (encontrado == NULL)
        {
            printf("[ERRO] Placa nao cadastrada no sistema.\n\n");
        }
    } while (!validarPlaca(placa) || encontrado == NULL);

    printf("[OK] Veiculo encontrado!\n");
    exibirDadosCarro(encontrado);
}

// 5. Listar Carros Cadastrados
void listarCarros(Carro *carros, int cadastrados)
{
/*
  @brief [listarCarros] Lista na tela todos os carros gravados no vetor dinâmico.
 
  Verifica se a quantidade de cadastrados é zero. Caso existam registros, percorre o vetor 
  dinâmico através de ponteiros e exibe as informações de cada veículo enumeradamente.
 
  @param carros Ponteiro simples para o vetor contendo os dados dos carros.
  @param cadastrados Quantidade total de veículos cadastrados.
*/
    int i = 1;
    if (cadastrados == 0)
    {
        printf("[ERRO] Nenhum carro cadastrado!\n\n");
        return;
    }
    for (Carro *p = carros; p < carros + cadastrados; p++)
    {
        printf("\nCarro %d:", i++);
        exibirDadosCarro(p);
    }
}

// 6. Alterar Carro por Campo
void alterarCarro(Carro *carros, int cadastrados)
{
/*
  @brief [alterarCarro] Permite a alteração individual dos campos de um veículo existente.
 
  Valida a placa informada pelo usuário e localiza o veículo. Apresenta um menu de opções 
  contendo todos os atributos editáveis (Marca, Modelo, Cor, Quilometragem, Valor do Aluguel). 
  Ao escolher o campo, força a re-validação dos novos dados digitados antes de salvá-los no registro.
 
  @param carros Ponteiro simples para o vetor dinâmico de Carro.
  @param cadastrados Quantidade de veículos atualmente cadastrados.
*/
    char placa[8];
    Carro *encontrado = NULL;

    if (cadastrados == 0 || carros == NULL)
    {
        printf("[ERRO] Nenhum carro cadastrado!\n\n");
        return;
    }
    do
    {
        pedirPlaca(placa);
        if (!validarPlaca(placa))
        {
            printf("[ERRO] Placa Invalida! Digite no formato correto (ABC1D23).\n\n");
            continue;
        }

        encontrado = buscarCarroPorPlaca(carros, cadastrados, placa);
        if (encontrado == NULL)
        {
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

    switch (opcao)
    {
    case 1:
        do
        {
            printf("\n--- MARCAS DISPONIVEIS ---\n");
            printf("[ Porsche | Ferrari | Lamborghini | BMW | Mercedes | Audi | Land Rover | Volvo ]\n\n");
            printf("Digite a nova marca: ");
            scanf(" %[^\n]", encontrado->marca);

            if (!validarMarca(encontrado->marca))
            {
                printf("[ERRO] Marca nao cadastrada ou erro de digitacao. Tente novamente.\n\n");
            }
        } while (!validarMarca(encontrado->marca));
        printf("[OK] Marca atualizada com sucesso!\n");
        break;

    case 2:
        do
        {
            printf("Digite o novo modelo: ");
            scanf(" %[^\n]", encontrado->modelo);

            if (!validarModelo(encontrado->modelo))
            {
                printf("[ERRO] Modelo invalido! Deve conter letras e pelo menos 2 caracteres.\n\n");
            }
        } while (!validarModelo(encontrado->modelo));
        printf("[OK] Modelo atualizado com sucesso!\n");
        break;

    case 3:
        do
        {
            printf("Digite a nova cor: ");
            scanf(" %[^\n]", encontrado->cor);

            if (!validarCor(encontrado->cor))
            {
                printf("[ERRO] Cor invalida! Use apenas letras e no minimo 3 caracteres.\n\n");
            }
        } while (!validarCor(encontrado->cor));
        printf("[OK] Cor atualizada com sucesso!\n\n");
        break;

    case 4:
        do
        {
            printf("Digite a nova Quilometragem: ");
            if (scanf("%f", &encontrado->km) != 1 || encontrado->km < 0)
            {
                printf("[ERRO] Quilometragem invalida! Digite um valor maior ou igual a zero.\n\n");
                limparBuffer();
                encontrado->km = -1;
            }
            limparBuffer();
        } while (encontrado->km < 0);
        printf("[OK] Quilometragem atualizada com sucesso!\n");
        break;

    case 5:
        do
        {
            printf("Digite o novo valor do aluguel: ");
            if (scanf("%f", &encontrado->valor) != 1 || encontrado->valor <= 0)
            {
                printf("[ERRO] Valor invalido! O aluguel deve ser maior do que zero.\n\n");
                limparBuffer();
                encontrado->valor = -1;
            }
            limparBuffer();
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

// 7. Excluir Carro com Compactação do Vetor
void excluirCarroPlaca(Carro **carros, int *cadastrados)
{
/*
  @brief [excluirCarroPlaca] Remove um veículo do vetor dinâmico com compactação e redução da memória RAM.
 
  Localiza o veículo pela placa informada. Verifica a flag alugado: se for igual a 0 
  (veículo alugado), bloqueia a exclusão e exibe uma mensagem de erro. Se alugado == 1, 
  desloca todos os registros posteriores uma posição para a esquerda (compactação) e decrementa 
  o contador de cadastrados. Em seguida, redimensiona o vetor dinâmico usando realloc(). 
  Se for o último veículo excluído, faz o free() e atribui NULL ao ponteiro do vetor.
 
  @param carros Ponteiro duplo para o vetor de estruturas Carro.
  @param cadastrados Ponteiro para o número total de veículos cadastrados.
*/
    if (*cadastrados == 0 || *carros == NULL)
    {
        printf("[ERRO] Nenhum carro cadastrado para excluir!\n\n");
        return;
    }

    char placa[8];
    Carro *encontrado = NULL;

    do
    {
        pedirPlaca(placa);
        if (!validarPlaca(placa))
        {
            printf("[ERRO] Placa Invalida!\n\n");
            continue;
        }
        encontrado = buscarCarroPorPlaca(*carros, *cadastrados, placa);
        if (encontrado == NULL)
        {
            printf("[ERRO] Placa nao cadastrada!\n\n");
        }
    } while (!validarPlaca(placa) || encontrado == NULL);

    if (encontrado->alugado == 0)
    {
        printf("[ERRO] Veiculo possui aluguel ativo (alugado == 0)! Nao e possivel excluir.\n\n");
        return;
    }

    for (Carro *p = encontrado; p < *carros + (*cadastrados - 1); p++)
    {
        *p = *(p + 1);
    }
    (*cadastrados)--;

    if (*cadastrados > 0)
    {
        Carro *temp = realloc(
            *carros,
            (*cadastrados) * sizeof(Carro));

        if (temp != NULL)
        {
            *carros = temp;
        }
    }
    else
    {
        free(*carros);
        *carros = NULL;
    }

    printf("[OK] Carro excluido com sucesso!\n\n");
}

// 8. Salvar Carros no Arquivo Binário
void salvarCarros(Carro *carros, int cadastrados)
{
/*
  @brief [salvarCarros] Salva a quantidade de cadastros e o vetor de carros no arquivo binário "carros.bin".
 
  Abre o arquivo no modo escrita binária ("wb"). Escreve primeiramente a variável inteira com 
  a quantidade de registros (*cadastrados) e, em seguida, grava em um único bloco de dados 
  o conteúdo completo da estrutura na memória usando a função fwrite().
 
  @param carros Ponteiro simples contendo o vetor de dados armazenado na RAM.
  @param cadastrados Total de registros a serem salvos em disco.
*/
    FILE *arquivo = fopen("carros.bin", "wb");
    if (arquivo == NULL)
    {
        printf("[ERRO] Não foi possivel abrir o arquivo carros.bin.\n\n");
        exit(1);
    }

    fwrite(&cadastrados, sizeof(int), 1, arquivo);

    if (cadastrados > 0 && carros != NULL)
    {
        fwrite(carros, sizeof(Carro), cadastrados, arquivo);
    }

    fclose(arquivo);
    printf("[OK] Dados salvos com sucesso em carros.bin!\n");
}

// 9. Carregar Carros do Arquivo Binário
Carro *carregarCarros(int *cadastrados)
{
/*
  @brief [carregarCarros] Carrega os veículos gravados no arquivo binário "carros.bin" para a memória RAM.
 
  Abre o arquivo no modo leitura binária ("rb"). Lê o número total de registros gravados 
  com fread(), aloca a memória necessária dinamicamente com malloc() e efetua a leitura 
  dos dados diretamente para o vetor de estruturas.
 
  @param cadastrados Ponteiro para a variável que armazenará o total de registros lidos do arquivo.
  @return Carro* Retorna o ponteiro para o vetor alocado com os dados carregados ou NULL em caso de erro/arquivo inexistente.
*/
    FILE *arquivo = fopen("carros.bin", "rb");
    if (arquivo == NULL)
    {
        *cadastrados = 0;
        return NULL;
    }

    if (fread(cadastrados, sizeof(int), 1, arquivo) != 1 || *cadastrados <= 0)
    {
        *cadastrados = 0;
        fclose(arquivo);
        return NULL;
    }

    Carro *carros = (Carro *)malloc((*cadastrados) * sizeof(Carro));
    if (carros == NULL)
    {
        printf("[ERRO] Falha ao alocar memoria para carregar os carros.\n\n");
        *cadastrados = 0;
        fclose(arquivo);
        return NULL;
    }

    fread(carros, sizeof(Carro), *cadastrados, arquivo);

    fclose(arquivo);
    printf("[OK] %d carro(s) carregado(s) com sucesso de carros.bin!\n", *cadastrados);
    return carros;
}

// 10. Submenu de Gerenciamento de Carros
void submenuCarros(Carro **carros, int *cadastrados)
{
    int opcao;

    do
    {
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

        switch (opcao)
        {
        case 1:
            cadastrarCarro(carros, cadastrados);
            break;

        case 2:
            listarCarros(*carros, *cadastrados);
            break;

        case 3:
            consultarCarro(*carros, *cadastrados);
            break;

        case 4:
            alterarCarro(*carros, *cadastrados);
            break;

        case 5:
            excluirCarroPlaca(carros, cadastrados);
            break;

        case 6:
            printf("Voltando...\n");
            break;

        default:
            printf("Opcao invalida!\n");
        }

    } while (opcao != 6);
}