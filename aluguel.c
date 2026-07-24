#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "aluguel.h"
#include "carro.h"
#include "cliente.h"
#include "util.h"

// 1. Funcoes de Verificacao
int codigoExiste(Aluguel *alugueis, int qtdAlugueis, int codigo)
{
    // Retorna 1 quando o codigo ja pertence a outro aluguel e 0 quando esta livre.
    for (int i = 0; i < qtdAlugueis; i++)
    {
        if (alugueis[i].codigo_aluguel == codigo)
        {
            return 1;
        }
    }

    return 0;
}

int existeAluguelPorCpf(const Aluguel *alugueis, int qtdAlugueis, const char *cpf)
{
    // Verifica se o cliente ja possui um aluguel ativo.
    // Verifica se o CPF informado ja possui algum aluguel em aberto vinculado a ele.
    // Usada para nao deixar o mesmo cliente com mais de um aluguel ao mesmo tempo.
    for (int i = 0; i < qtdAlugueis; i++)
    {
        if (strcmp(alugueis[i].cpf_cliente, cpf) == 0)
        {
            return 1;
        }
    }

    return 0;
}

int existeAluguelPorPlaca(const Aluguel *alugueis, int qtdAlugueis, const char *placa)
{
    // Verifica se o carro ja esta vinculado a um aluguel ativo.
    for (int i = 0; i < qtdAlugueis; i++)
    {
        if (strcmp(alugueis[i].placa, placa) == 0)
        {
            return 1;
        }
    }

    return 0;
}

// 2. Buscar Aluguel pelo Codigo
void buscarAluguelporcodigo(Aluguel *alugueis, int qtdAlugueis)
{
    if (qtdAlugueis == 0 || alugueis == NULL)
    {
        printf("[ERRO] Nenhum aluguel cadastrado!\n");
        return;
    }

    int codigo;
    int encontrado = 0;

    printf("\n--- BUSCAR ALUGUEL ---\n");
    printf("Digite o codigo do aluguel: ");

    if (scanf("%d", &codigo) != 1)
    {
        limparBuffer();
        printf("[ERRO] Codigo invalido!\n");
        return;
    }
    limparBuffer();

    for (int i = 0; i < qtdAlugueis; i++)
    {
        if (alugueis[i].codigo_aluguel == codigo)
        {
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

    if (!encontrado)
    {
        printf("Aluguel nao encontrado.\n");
    }
}

// 3. Aumentar o Vetor Dinamico de Alugueis
// Aumenta em 1 posicao o vetor dinamico de alugueis usando realloc.
// Isso substitui a ideia de um vetor de tamanho fixo, deixando o cadastro sem limite maximo.
int aumentarVetorAlugueis(Aluguel **alugueis, int qtdAlugueis)
{
    // Usa um ponteiro temporario para nao perder o vetor original se o realloc falhar.
    Aluguel *novo = realloc(
        *alugueis,
        (qtdAlugueis + 1) * sizeof(Aluguel)
    );

    if (novo == NULL)
    {
        printf("\n[ERRO] Falha na alocacao de memoria.\n");
        return 0;
    }

    *alugueis = novo;
    return 1;
}

// 4. Salvar Alugueis no Arquivo Binario
// Salva todo o vetor de alugueis no arquivo binario "alugueis.bin".
// Retorna 0 se der erro ao abrir o arquivo e 1 se salvar com sucesso.
int salvarAlugueis(Aluguel *alugueis, int qtdAlugueis)
{
    FILE *arq = fopen("alugueis.bin", "wb");

    if (arq == NULL)
    {
        printf("[ERRO] Nao foi possivel abrir alugueis.bin.\n");
        return 0;
    }

    if (qtdAlugueis > 0 && alugueis != NULL)
    {
        fwrite(alugueis, sizeof(Aluguel), qtdAlugueis, arq);
    }

    fclose(arq);
    return 1;
}

// 5. Carregar Alugueis do Arquivo Binario
// Le o arquivo "alugueis.bin" e carrega os alugueis salvos para dentro do programa.
// Calcula quantos registros existem a partir do tamanho do arquivo e aloca a memoria certa para eles.
int carregarAlugueis(Aluguel **alugueis, int *qtdAlugueis)
{
    FILE *arq = fopen("alugueis.bin", "rb");

    if (arq == NULL)
    {
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

// 6. Cadastrar Aluguel com Alocacao Dinamica
// Cadastra um novo aluguel: pede codigo, placa do carro, CPF do cliente e quantidade de dias,
// calcula o valor total (dias x diaria do carro) e marca o carro como alugado.
int cadastrarAluguel(
    Aluguel **alugueis,
    int *qtdAlugueis,
    Cliente *clientes,
    int qtdClientes,
    Carro *carros,
    int qtdCarros
)
{
    if (qtdClientes == 0 || clientes == NULL)
    {
        printf("[ERRO] Cadastre um cliente antes de criar um aluguel!\n");
        return 0;
    }

    if (qtdCarros == 0 || carros == NULL)
    {
        printf("[ERRO] Cadastre um carro antes de criar um aluguel!\n");
        return 0;
    }

    if (!aumentarVetorAlugueis(alugueis, *qtdAlugueis))
    {
        return 0;
    }

    int pos = *qtdAlugueis;
    int codigo;

    printf("\n--- CADASTRAR ALUGUEL ---\n");

    do
    {
        printf("Codigo do aluguel: ");

        if (scanf("%d", &codigo) != 1)
        {
            limparBuffer();
            printf("[ERRO] Codigo invalido!\n");
            codigo = -1;
            continue;
        }
        limparBuffer();

        if (codigoExiste(*alugueis, *qtdAlugueis, codigo))
        {
            printf("[ERRO] Codigo ja cadastrado!\n");
        }
    } while (codigo < 0 || codigoExiste(*alugueis, *qtdAlugueis, codigo));

    (*alugueis)[pos].codigo_aluguel = codigo;

    char placa[8];
    Carro *indiceCarro = NULL;

    do
    {
        pedirPlaca(placa);

        if (!validarPlaca(placa))
        {
            printf("[ERRO] Placa invalida! Digite no formato correto (ABC1D23).\n\n");
            continue;
        }

        // Busca o carro no array passado pelo menus
        indiceCarro = buscarCarroPorPlaca(carros, qtdCarros, placa);

        if (indiceCarro == NULL)
        {
            printf("[ERRO] Carro nao encontrado!\n\n");
            continue;
        }

        // Regra do projeto: 1 significa livre e 0 significa que possui aluguel ativo.
        if (indiceCarro->alugado == 0)
        {
            printf("[ERRO] Este carro ja esta alugado no momento!\n\n");
            indiceCarro = NULL;
        }
    } while (indiceCarro == NULL);

    // Copia a placa validada para a estrutura do aluguel
    strcpy((*alugueis)[pos].placa, placa);

    char cpf[15];
    int indiceCliente = -1;

    do
    {
        printf("CPF do cliente, com 11 digitos e sem formatacao: ");
        scanf("%14s", cpf);
        limparBuffer();

        indiceCliente = buscarPorCpf(clientes, qtdClientes, cpf);

        if (indiceCliente == -1)
        {
            printf("[ERRO] Cliente nao encontrado!\n");
            continue;
        }

        // Impede que o mesmo cliente tenha mais de um aluguel ativo.
        if (clientes[indiceCliente].alugado == 0 ||
            existeAluguelPorCpf(*alugueis, *qtdAlugueis, cpf))
        {
            printf("[ERRO] Este cliente ja possui um aluguel ativo!\n");
            indiceCliente = -1;
        }
    } while (indiceCliente == -1);

    strcpy((*alugueis)[pos].cpf_cliente, cpf);

    do
    {
        printf("Quantidade de dias do aluguel: ");

        if (scanf("%d", &(*alugueis)[pos].dias) != 1)
        {
            limparBuffer();
            (*alugueis)[pos].dias = 0;
            printf("[ERRO] Quantidade de dias invalida!\n");
            continue;
        }
        limparBuffer();

        if ((*alugueis)[pos].dias <= 0)
        {
            printf("[ERRO] Quantidade de dias invalida!\n");
        }
    } while ((*alugueis)[pos].dias <= 0);

    // O valor total e calculado automaticamente pela diaria do carro.
    (*alugueis)[pos].valor_total =
        (*alugueis)[pos].dias * indiceCarro->valor;

    printf("Valor total: R$ %.2lf\n", (*alugueis)[pos].valor_total);

    (*qtdAlugueis)++;

    // Regra padronizada: 0 significa que cliente e carro possuem aluguel ativo.
    atualizarAlugadoCliente(
        clientes,
        qtdClientes,
        *alugueis,
        *qtdAlugueis,
        cpf
    );

    atualizarAlugadoCarro(
        carros,
        qtdCarros,
        *alugueis,
        *qtdAlugueis,
        placa
    );

    salvarAlugueis(*alugueis, *qtdAlugueis);
    salvarCarros(carros, qtdCarros);

    printf("[OK] Aluguel cadastrado com sucesso!\n");
    return 1;
}

// 7. Listar Alugueis Cadastrados
// Lista todos os alugueis ja cadastrados, mostrando codigo, placa, CPF, dias e valor total de cada um.
void listarAlugueis(const Aluguel *alugueis, int qtdAlugueis)
{
    if (qtdAlugueis == 0 || alugueis == NULL)
    {
        printf("Nenhum aluguel cadastrado.\n");
        return;
    }

    printf("\n--- LISTA DE ALUGUEIS ---\n");

    for (int i = 0; i < qtdAlugueis; i++)
    {
        printf("\nAluguel %d\n", i + 1);
        printf("Codigo do aluguel: %d\n", alugueis[i].codigo_aluguel);
        printf("Placa do carro: %s\n", alugueis[i].placa);
        printf("CPF do cliente: %s\n", alugueis[i].cpf_cliente);
        printf("Dias alugados: %d\n", alugueis[i].dias);
        printf("Valor total: %.2lf\n", alugueis[i].valor_total);
    }
}

// 8. Alterar Aluguel por Campo
// Altera os dados de um aluguel ja existente (CPF, codigo e dias), buscando-o pelo codigo antigo.
// Recalcula o valor total apos a alteracao, com base na diaria do carro correspondente.
int alterarAluguel(
    Aluguel *alugueis,
    int qtdAlugueis,
    Cliente *clientes,
    int qtdClientes,
    Carro *carros,
    int qtdCarros
)
{
    if (qtdAlugueis == 0 || alugueis == NULL)
    {
        printf("[ERRO] Nenhum aluguel cadastrado para alterar!\n");
        return 0;
    }

    int codigoBusca;
    int pos = -1;

    printf("Digite o codigo do aluguel que deseja modificar: ");

    if (scanf("%d", &codigoBusca) != 1)
    {
        limparBuffer();
        printf("[ERRO] Codigo invalido!\n");
        return 0;
    }
    limparBuffer();

    for (int i = 0; i < qtdAlugueis; i++)
    {
        if (alugueis[i].codigo_aluguel == codigoBusca)
        {
            pos = i;
            break; //* para o loop porque encontrou o registro
        }
    }

    if (pos == -1)
    {
        printf("Aluguel nao encontrado.\n");
        return 0;
    }

    printf("\nO que voce deseja alterar?\n");
    printf("1. CPF do cliente\n");
    printf("2. Codigo do aluguel\n");
    printf("3. Quantidade de dias\n");
    printf("4. Cancelar operacao\n");
    printf("Opcao: ");

    int opcao;

    if (scanf("%d", &opcao) != 1)
    {
        limparBuffer();
        printf("[ERRO] Opcao invalida!\n");
        return 0;
    }
    limparBuffer();

    switch (opcao)
    {
        case 1:
        {
            char cpfAntigo[15];
            char novoCpf[15];
            int indiceCliente;

            strcpy(cpfAntigo, alugueis[pos].cpf_cliente);

            printf("Novo CPF do cliente: ");
            scanf("%14s", novoCpf);
            limparBuffer();

            indiceCliente = buscarPorCpf(clientes, qtdClientes, novoCpf);

            if (indiceCliente == -1)
            {
                printf("[ERRO] Cliente nao encontrado!\n");
                return 0;
            }

            if (strcmp(novoCpf, cpfAntigo) != 0 &&
                (clientes[indiceCliente].alugado == 0 ||
                 existeAluguelPorCpf(alugueis, qtdAlugueis, novoCpf)))
            {
                printf("[ERRO] O novo cliente ja possui um aluguel ativo!\n");
                return 0;
            }

            strcpy(alugueis[pos].cpf_cliente, novoCpf);

            // Libera o cliente antigo e bloqueia o novo conforme os alugueis restantes.
            atualizarAlugadoCliente(
                clientes,
                qtdClientes,
                alugueis,
                qtdAlugueis,
                cpfAntigo
            );

            atualizarAlugadoCliente(
                clientes,
                qtdClientes,
                alugueis,
                qtdAlugueis,
                novoCpf
            );

            printf("[OK] CPF atualizado com sucesso!\n");
            break;
        }

        case 2:
        {
            int novoCodigo;

            do
            {
                printf("Novo codigo do aluguel: ");

                if (scanf("%d", &novoCodigo) != 1)
                {
                    limparBuffer();
                    novoCodigo = -1;
                    printf("[ERRO] Codigo invalido!\n");
                    continue;
                }
                limparBuffer();

                if (novoCodigo != alugueis[pos].codigo_aluguel &&
                    codigoExiste(alugueis, qtdAlugueis, novoCodigo))
                {
                    printf("[ERRO] Codigo ja cadastrado!\n");
                    novoCodigo = -1;
                }
            } while (novoCodigo < 0);

            alugueis[pos].codigo_aluguel = novoCodigo;
            printf("[OK] Codigo atualizado com sucesso!\n");
            break;
        }

        case 3:
        {
            do
            {
                printf("Nova quantidade de dias: ");

                if (scanf("%d", &alugueis[pos].dias) != 1)
                {
                    limparBuffer();
                    alugueis[pos].dias = 0;
                    printf("[ERRO] Quantidade de dias invalida!\n");
                    continue;
                }
                limparBuffer();

                if (alugueis[pos].dias <= 0)
                {
                    printf("[ERRO] Quantidade de dias invalida!\n");
                }
            } while (alugueis[pos].dias <= 0);

            Carro *indiceCarro = buscarCarroPorPlaca(
                carros,
                qtdCarros,
                alugueis[pos].placa
            );

            if (indiceCarro == NULL)
            {
                printf("[ERRO] Carro vinculado ao aluguel nao foi encontrado!\n");
                return 0;
            }

            alugueis[pos].valor_total =
                alugueis[pos].dias * indiceCarro->valor;

            printf("Novo valor total: R$ %.2lf\n", alugueis[pos].valor_total);
            printf("[OK] Quantidade de dias atualizada com sucesso!\n");
            break;
        }

        case 4:
            printf("Operacao cancelada. Nenhum dado foi alterado.\n");
            return 0;

        default:
            printf("[ERRO] Opcao invalida!\n");
            return 0;
    }

    salvarAlugueis(alugueis, qtdAlugueis);
    return 1;
}

// 9. Excluir Aluguel com Compactacao do Vetor
// Exclui um aluguel pelo codigo, deslocando os registros seguintes uma posicao para tras
// (ja que nao existe delete direto em vetor) e diminuindo a quantidade de alugueis.
int excluirAluguel(
    Aluguel **alugueis,
    int *qtdAlugueis,
    Cliente *clientes,
    int qtdClientes,
    Carro *carros,
    int qtdCarros
)
{
    if (*qtdAlugueis == 0 || *alugueis == NULL)
    {
        printf("[ERRO] Nenhum aluguel cadastrado para excluir!\n");
        return 0;
    }

    int codigo;
    int pos = -1;

    printf("Digite o codigo do aluguel que deseja excluir: ");

    if (scanf("%d", &codigo) != 1)
    {
        limparBuffer();
        printf("[ERRO] Codigo invalido!\n");
        return 0;
    }
    limparBuffer();

    for (int i = 0; i < *qtdAlugueis; i++)
    {
        if ((*alugueis)[i].codigo_aluguel == codigo)
        {
            pos = i;
            break;
        }
    }

    if (pos == -1)
    {
        printf("Aluguel nao encontrado.\n");
        return 0;
    }

    // Guarda os relacionamentos antes de remover o registro do vetor.
    char cpf[15];
    char placa[8];
    strcpy(cpf, (*alugueis)[pos].cpf_cliente);
    strcpy(placa, (*alugueis)[pos].placa);

    for (int i = pos; i < *qtdAlugueis - 1; i++)
    {
        (*alugueis)[i] = (*alugueis)[i + 1];
    }

    (*qtdAlugueis)--;

    if (*qtdAlugueis == 0)
    {
        free(*alugueis);
        *alugueis = NULL;
    }
    else
    {
        // Usa um temporario para preservar o vetor original se o realloc falhar.
        Aluguel *temp = realloc(
            *alugueis,
            (*qtdAlugueis) * sizeof(Aluguel)
        );

        if (temp != NULL)
        {
            *alugueis = temp;
        }
    }

// Sem aluguel ativo, Cliente e Carro voltam para 1, que significa livre.
// Atualiza o campo "alugado" do cliente sempre que um aluguel dele e criado ou removido.
// Confere se o CPF ainda possui algum aluguel ativo e marca 1 (alugado) ou 0 (livre).
    atualizarAlugadoCliente(
        clientes,
        qtdClientes,
        *alugueis,
        *qtdAlugueis,
        cpf
    );

    atualizarAlugadoCarro(
        carros,
        qtdCarros,
        *alugueis,
        *qtdAlugueis,
        placa
    );

    salvarAlugueis(*alugueis, *qtdAlugueis);
    salvarCarros(carros, qtdCarros);

    printf("[OK] Aluguel excluido com sucesso!\n");
    return 1;
}

// 10. Atualizar Status de Aluguel do Cliente
// Atualiza o campo "alugado" do carro (pela placa) sempre que ele e alugado ou devolvido.
// Funciona igual a funcao acima, mas voltada para o carro em vez do cliente.
void atualizarAlugadoCliente(
    Cliente *clientes,
    int qtdClientes,
    const Aluguel *alugueis,
    int qtdAlugueis,
    const char *cpf
)
{
    int possui = existeAluguelPorCpf(alugueis, qtdAlugueis, cpf);
    int pos = buscarPorCpf(clientes, qtdClientes, (char *)cpf);

    if (pos != -1)
    {
        // Regra do projeto: 0 = possui aluguel; 1 = livre.
        clientes[pos].alugado = possui ? 0 : 1;
    }
}

// 11. Atualizar Status de Aluguel do Carro
void atualizarAlugadoCarro(
    Carro *carros,
    int qtdCarros,
    const Aluguel *alugueis,
    int qtdAlugueis,
    const char *placa
)
{
    int possui = existeAluguelPorPlaca(alugueis, qtdAlugueis, placa);

    for (int i = 0; i < qtdCarros; i++)
    {
        if (strcmp(carros[i].placa, placa) == 0)
        {
            // Regra do projeto: 0 = possui aluguel; 1 = livre.
            carros[i].alugado = possui ? 0 : 1;
            return;
        }
    }
}

// 12. Menu de Gerenciamento de Alugueis
void menuAluguel(
    Aluguel **alugueis,
    int *qtdAlugueis,
    Cliente *clientes,
    int qtdClientes,
    Carro *carros,
    int qtdCarros
)
{
    int opcao;

    do
    {
        printf("\n--- MENU ALUGUEL ---\n");
        printf("1 - Cadastrar aluguel\n");
        printf("2 - Listar alugueis\n");
        printf("3 - Alterar aluguel\n");
        printf("4 - Excluir aluguel\n");
        printf("5 - Buscar aluguel\n");
        printf("0 - Voltar ao menu principal\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1)
        {
            limparBuffer();
            opcao = -1;
        }
        else
        {
            limparBuffer();
        }

        switch (opcao)
        {
            case 1:
                cadastrarAluguel(
                    alugueis,
                    qtdAlugueis,
                    clientes,
                    qtdClientes,
                    carros,
                    qtdCarros
                );
                break;

            case 2:
                listarAlugueis(*alugueis, *qtdAlugueis);
                break;

            case 3:
                alterarAluguel(
                    *alugueis,
                    *qtdAlugueis,
                    clientes,
                    qtdClientes,
                    carros,
                    qtdCarros
                );
                break;

            case 4:
                excluirAluguel(
                    alugueis,
                    qtdAlugueis,
                    clientes,
                    qtdClientes,
                    carros,
                    qtdCarros
                );
                break;

            case 5:
                buscarAluguelporcodigo(*alugueis, *qtdAlugueis);
                break;

            case 0:
                printf("Voltando...\n");
                break;

            default:
                printf("[ERRO] Opcao invalida!\n");
        }
    } while (opcao != 0);
}

// 13. Submenu de Alugueis Chamado pelo Menu Principal
// Apenas repassa a chamada para menuAluguel. Serve como ponto de entrada do submenu
// para ser chamado a partir do menu principal do sistema.
void submenuAlugueis(
    Aluguel **alugueis,
    int *qtdAlugueis,
    Cliente *clientes,
    int qtdClientes,
    Carro *carros,
    int qtdCarros
)
{
    menuAluguel(
        alugueis,
        qtdAlugueis,
        clientes,
        qtdClientes,
        carros,
        qtdCarros
    );
}