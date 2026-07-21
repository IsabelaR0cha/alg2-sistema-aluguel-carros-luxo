#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cliente.h"
#include "util.h"

// 1. Funções de Busca (adaptadas para usar 'qtd' e retornar o índice correto)
int buscarPorCpf(Cliente *vetor, int qtd, char *cpfBusca) {
    for(int i = 0; i < qtd; i++)
        if(strcmp(vetor[i].cpf, cpfBusca) == 0) {
            return i;
        }
    return -1;
}

int buscarPorNome(Cliente *vetor, int qtd, char *nomeBusca) {
    for(int i = 0; i < qtd; i++) {
        if(strstr(vetor[i].nome, nomeBusca) != NULL) {
            return i;
        }
    }
    return -1;
}

int buscarPorCnh(Cliente *vetor, int qtd, char *cnhBusca) {
    for(int i = 0; i < qtd; i++) {
        if(strcmp(vetor[i].cnh, cnhBusca) == 0) {
            return i;
        }
    }
    return -1;
}

int buscarPorEmail(Cliente *vetor, int qtd, char *emailBusca) {
    for(int i = 0; i < qtd; i++)
        if(strcmp(vetor[i].email, emailBusca) == 0) {
            return i;
        }
    return -1;
}

int buscarPorTelefone(Cliente *vetor, int qtd, char *telefoneBusca) {
    for(int i = 0; i < qtd; i++)
        if(strcmp(vetor[i].telefone, telefoneBusca) == 0) {
            return i;
        }
    return -1;
}

// 2. Validações padrão mantidas e blindadas
int validarDDD(char *telefone) {
    // Lista fixa com todos os DDDs oficiais brasileiros.
    const char *dddsValidos[] = {
        "11", "12", "13", "14", "15", "16", "17", "18", "19",
        "21", "22", "24", "27", "28", "31", "32", "33", "34", 
        "35", "37", "38", "41", "42", "43", "44", "45", "46", 
        "47", "48", "49", "51", "53", "54", "55", "61", "62", 
        "63", "64", "65", "66", "67", "68", "69", "71", "73", 
        "74", "75", "77", "79", "81", "82", "83", "84", "85", 
        "86", "87", "88", "89", "91", "92", "93", "94", "95", 
        "96", "97", "98", "99"
    };

    char ddd[3];

    // Extrai apenas os dois primeiros caracteres do telefone (o DDD)
    // para compará-los isoladamente com a lista de DDDs válidos.
    strncpy(ddd, telefone, 2);
    ddd[2] = '\0';

    for(int i = 0; i < 67; i++) {
        if(strcmp(ddd, dddsValidos[i]) == 0) {
            return 1; // DDD válido
        }
    }

    return 0; // DDD inválido
}

int validarEmail(char *email) {
    char *arroba = strchr(email, '@');   // primeira ocorrência de '@'
    char *ponto = strrchr(email, '.');   // última ocorrência de '.'

    // A comparação de ponteiros (ponto > arroba) funciona porque ambos
    // apontam para posições dentro da mesma string: se o ponto está em
    // um endereço de memória "depois" do arroba, ele vem depois no texto.
    if(arroba != NULL && ponto != NULL && ponto > arroba) {
        return 1;
    }
    return 0;
}

int validarCnhCarro(char *cnh) {
    // Aceita qualquer combinação de categorias (ex: "AB", "BE") desde que
    // contenha pelo menos uma letra que habilite o cliente a dirigir carro.
    if(strchr(cnh, 'B') != NULL || strchr(cnh, 'b') != NULL ||
       strchr(cnh, 'C') != NULL || strchr(cnh, 'c') != NULL ||
       strchr(cnh, 'D') != NULL || strchr(cnh, 'd') != NULL ||
       strchr(cnh, 'E') != NULL || strchr(cnh, 'e') != NULL) {
        return 1;
    }
    return 0;
}

int ehCpfValido(char *cpf) {
    // Testa o tamanho antes de percorrer caractere a caractere,
    // evitando trabalho desnecessário caso o CPF já esteja claramente errado.
    if(strlen(cpf) != 11) {
        return 0;
    }

    for(int i = 0; i < 11; i++) {
        if(!isdigit(cpf[i])) {
            return 0; // Encontrou uma letra ou símbolo, é inválido!
        }
    }

    return 1; // Tudo certo, só tem números
}

int validarIdade(int idade) {
    if(idade >= 18 && idade <= 120) {
        return 1; // Idade válida
    }
    return 0; // Idade inválida
}

int validarCEP(char *cep) {
    if(strlen(cep) != 8) {
        return 0;
    }

    for(int i = 0; i < 8; i++) {
        if(!isdigit(cep[i])) {
            return 0;
        }
    }

    return 1;
}

int validarTelefoneTamanho(char *telefone) {
    if(strlen(telefone) != 11) {
        return 0;
    }

    for(int i = 0; i < 11; i++) {
        if(!isdigit(telefone[i])) {
            return 0;
        }
    }

    return 1;
}

int validarNome(char *nome) {
    if(strlen(nome) < 5) {
        return 0;
    }

    for(int i = 0; nome[i] != '\0'; i++) {
        if(isdigit(nome[i])) {
            return 0;
        }
    }

    return 1;
}

// 3. Cadastrar Cliente com Alocação Dinâmica e Realocação via realloc
void cadastrarCliente(Cliente **vetor, int *qtd, int *capacidade) {

    // Verifica se o vetor atingiu sua capacidade máxima.
    // Caso tenha atingido, aumenta a capacidade em mais 5 posições
    // utilizando realloc para realocar o bloco de memória.
    if (*qtd >= *capacidade) {
        *capacidade += 5; // Expande a capacidade em blocos de 5 clientes

        Cliente *temp = realloc(*vetor, (*capacidade) * sizeof(Cliente));

        // Verifica se a realocação foi realizada com sucesso.
        if (temp == NULL) {
            printf("\n[ERRO] Falha ao realocar memoria!\n");
            return;
        }

        // Atualiza o ponteiro do vetor para o novo endereço de memória.
        *vetor = temp;
    }

    // A próxima posição livre para inserção é a quantidade atual de clientes.
    int pos = *qtd;

    char cpfTemp[15];
    printf("\nCPF (apenas 11 numeros): ");
    limparBuffer();
    fgets(cpfTemp, 15, stdin);
    limparTexto(cpfTemp);

    // Valida o formato do CPF antes de verificar duplicidade,
    // pois não há motivo para buscar no vetor um CPF que já é inválido.
    if(!ehCpfValido(cpfTemp)) {
        printf("\n[ERRO] CPF invalido! Digite apenas 11 numeros.\n");
        return;
    }

    if(buscarPorCpf(*vetor, *qtd, cpfTemp) != -1) {
        printf("\n[ERRO] CPF duplicado!\n");
        return;
    }

    int idadeTemp;
    printf("Idade: ");
    scanf("%d", &idadeTemp);

    if(!validarIdade(idadeTemp)) {
        printf("\n[ERRO] Idade invalida!\n");
        return;
    }

    char cnhTemp[20];
    printf("CNH: ");
    // Limpa o '\n' deixado pelo scanf("%d", ...) anterior antes de usar
    // fgets(), evitando que ele leia uma linha vazia (ver limparBuffer()).
    limparBuffer();
    fgets(cnhTemp, 20, stdin);
    limparTexto(cnhTemp);

    if(!validarCnhCarro(cnhTemp)) {
        printf("\n[ERRO] CNH nao compativel com carro!\n");
        return;
    }

    char telefoneTemp[20];
    printf("Telefone: ");
    fgets(telefoneTemp, 20, stdin);
    limparTexto(telefoneTemp);

    // Todo celular brasileiro tem '9' como terceiro
    // caractere (posição 2), logo após o DDD de 2 dígitos. Verificamos
    // o tamanho mínimo antes de acessar o índice 2, para não ler memória
    // fora dos limites caso o usuário digite um telefone muito curto.
    if(strlen(telefoneTemp) < 3 || telefoneTemp[2] != '9') {
        printf("\n[ERRO] Telefone invalido! O numero deve iniciar com 9 apos o DDD.\n");
        return;
    }

    if(!validarTelefoneTamanho(telefoneTemp)) {
        printf("\n[ERRO] Telefone deve possuir 11 numeros.\n");
        return;
    }

    if(!validarDDD(telefoneTemp)) {
        printf("\n[ERRO] DDD invalido!\n");
        return;
    }

    if(buscarPorTelefone(*vetor, *qtd, telefoneTemp) != -1) {
        printf("\n[ERRO] Telefone duplicado!\n");
        return;
    }

    char emailTemp[100];
    printf("Email: ");
    fgets(emailTemp, 100, stdin);
    limparTexto(emailTemp);

    if(!validarEmail(emailTemp)) {
        printf("\n[ERRO] Email invalido!\n");
        return;
    }

    if(buscarPorEmail(*vetor, *qtd, emailTemp) != -1) {
        printf("\n[ERRO] E-mail duplicado!\n");
        return;
    }

    char nomeTemp[100];
    printf("Nome: ");
    fgets(nomeTemp, 100, stdin);
    limparTexto(nomeTemp);

    if(!validarNome(nomeTemp)) {
        printf("\n[ERRO] Nome invalido!\n");
        return;
    }

    char cepTemp[10];
    printf("CEP: ");
    fgets(cepTemp, 10, stdin);
    limparTexto(cepTemp);

    if(!validarCEP(cepTemp)) {
        printf("\n[ERRO] CEP invalido!\n");
        return;
    }

    strcpy((*vetor)[pos].cpf, cpfTemp);
    (*vetor)[pos].idade = idadeTemp;
    strcpy((*vetor)[pos].cnh, cnhTemp);
    strcpy((*vetor)[pos].telefone, telefoneTemp);
    strcpy((*vetor)[pos].email, emailTemp);
    strcpy((*vetor)[pos].nome, nomeTemp);
    strcpy((*vetor)[pos].cep, cepTemp);
    (*vetor)[pos].ativo = 1;
    (*vetor)[pos].alugado = 1; // 1 = Livre por padrão para exclusão futura
    (*qtd)++;

    printf("\n[OK] Cliente cadastrado com sucesso!\n");
}

// 4. Listar Clientes perfeitamente baseado na quantidade real 'qtd'
void listarClientes(Cliente *vetor, int qtd) {
    if(qtd == 0) {
        printf("\nNenhum cliente cadastrado.\n");
        return;
    }

    for(int i = 0; i < qtd; i++) {
        printf("\n--- Cliente %d ---\n", i + 1);
        
        printf("Nome: %s\nCPF: %s\nIdade: %d\nCNH: %s\nTelefone: %s\nEmail: %s\nCEP: %s\nStatus Aluguel: %s\n",
               vetor[i].nome, vetor[i].cpf, vetor[i].idade, vetor[i].cnh, 
               vetor[i].telefone, vetor[i].email, vetor[i].cep,
               vetor[i].alugado == 0 ? "Alugado (Bloqueado)" : "Livre");
    }
}

// 5. Consultar Cliente com suporte a CPF, Nome e CNH
void consultarCliente(Cliente *vetor, int qtd) {
    if(qtd == 0) {
        printf("\nNenhum cliente cadastrado para consultar.\n");
        return;
    }

    int opcao;
    
    printf("\nConsultar por:\n1. CPF\n2. Nome\n3. CNH\nEscolha: ");
    scanf("%d", &opcao);
    limparBuffer();

    int pos = -1;

    if(opcao == 1) {
        char cpfBusca[15];
        printf("\nCPF para buscar: ");
        fgets(cpfBusca, 15, stdin);
        limparTexto(cpfBusca);
        pos = buscarPorCpf(vetor, qtd, cpfBusca);
    } 
    else if(opcao == 2) {
        char nomeBusca[100];
        printf("\nNome para buscar: ");
        fgets(nomeBusca, 100, stdin);
        limparTexto(nomeBusca);
        pos = buscarPorNome(vetor, qtd, nomeBusca);
    } 
    else if(opcao == 3) {
        char cnhBusca[20];
        printf("\nCNH para buscar: ");
        fgets(cnhBusca, 20, stdin);
        limparTexto(cnhBusca);
        pos = buscarPorCnh(vetor, qtd, cnhBusca);
    } 
    else {
        printf("\n[ERRO] Opcao invalida!\n");
        return;
    }

    if(pos == -1) {
        printf("\n[ERRO] Cliente nao encontrado!\n");
    } else {
        printf("\n[OK] Encontrado!\nNome: %s\nCPF: %s\nCNH: %s\nEmail: %s\nTelefone: %s\n", 
               vetor[pos].nome, vetor[pos].cpf, vetor[pos].cnh, vetor[pos].email, vetor[pos].telefone);
    }
}

// 6. Alterar Cliente (menu de edição por campo sem alterar o CPF)
void alterarCliente(Cliente *vetor, int qtd) {
    char cpfBusca[15], novoTelefone[20], novoEmail[100], novaCnh[20], novoNome[100], novoCep[10];
    int novaIdade;

    printf("\nCPF para alterar: ");
    limparBuffer();
    fgets(cpfBusca, 15, stdin);
    limparTexto(cpfBusca);

    int pos = buscarPorCpf(vetor, qtd, cpfBusca);
    if(pos == -1) {
        printf("\n[ERRO] Nao encontrado!\n");
        return;
    }

    int opcaoAlterar = 0;
    printf("\nO que deseja alterar?\n1. Nome\n2. Idade\n3. CNH\n4. Telefone\n5. Email\n6. CEP\nEscolha: ");
    scanf("%d", &opcaoAlterar);

    if(opcaoAlterar == 1) {
        printf("Novo Nome: ");
        limparBuffer();
        fgets(novoNome, 100, stdin);
        limparTexto(novoNome);

        if(!validarNome(novoNome)) {
            printf("\n[ERRO] Nome invalido! Deve conter pelo menos 5 caracteres.\n");
            return;
        }

        strcpy(vetor[pos].nome, novoNome);
        printf("\n[OK] Nome atualizado!\n");

    } else if(opcaoAlterar == 2) {
        printf("Nova Idade: ");
        limparBuffer();
        scanf("%d", &novaIdade);
        limparBuffer();

        if(!validarIdade(novaIdade)) {
            printf("\n[ERRO] Idade invalida!\n");
            return;
        }

        vetor[pos].idade = novaIdade;
        printf("\n[OK] Idade atualizada!\n");

    } else if(opcaoAlterar == 3) {
        printf("Nova CNH: ");
        limparBuffer();
        fgets(novaCnh, 20, stdin);
        limparTexto(novaCnh);

        if(!validarCnhCarro(novaCnh)) {
            printf("\n[ERRO] CNH nao compativel com carro!\n");
            return;
        }

        strcpy(vetor[pos].cnh, novaCnh);
        printf("\n[OK] CNH atualizada!\n");

    } else if(opcaoAlterar == 4) {
        printf("Novo Telefone: ");
        limparBuffer();
        fgets(novoTelefone, 20, stdin);
        limparTexto(novoTelefone);

        if(!validarTelefoneTamanho(novoTelefone)) {
            printf("\n[ERRO] Telefone deve possuir 11 numeros.\n");
            return;
        }

        if(!validarDDD(novoTelefone)) {
            printf("\n[ERRO] DDD invalido!\n");
            return;
        }

        if(novoTelefone[2] != '9') {
            printf("\n[ERRO] O telefone deve iniciar com 9 apos o DDD.\n");
            return;
        }

        if(buscarPorTelefone(vetor, qtd, novoTelefone) != -1) {
            printf("\n[ERRO] Telefone ja em uso!\n");
            return;
        }

        strcpy(vetor[pos].telefone, novoTelefone);
        printf("\n[OK] Telefone atualizado!\n");

    } else if(opcaoAlterar == 5) {
        printf("Novo Email: ");
        limparBuffer();
        fgets(novoEmail, 100, stdin);
        limparTexto(novoEmail);

        if(!validarEmail(novoEmail)) {
            printf("\n[ERRO] Email invalido!\n");
            return;
        }

        if(buscarPorEmail(vetor, qtd, novoEmail) != -1) {
            printf("\n[ERRO] E-mail ja em uso!\n");
            return;
        }

        strcpy(vetor[pos].email, novoEmail);
        printf("\n[OK] Email atualizado!\n");

    } else if(opcaoAlterar == 6) {
        printf("Novo CEP: ");
        limparBuffer();
        fgets(novoCep, 10, stdin);
        limparTexto(novoCep);

        if(!validarCEP(novoCep)) {
            printf("\n[ERRO] CEP invalido! Deve possuir 8 numeros.\n");
            return;
        }

        strcpy(vetor[pos].cep, novoCep);
        printf("\n[OK] CEP atualizado!\n");

    } else {
        printf("\n[ERRO] Opcao invalida!\n");
    }
}

// 7. Exclusão com compactação real (Shift) e validação do campo alugado
void excluirCliente(Cliente **vetor, int *qtd, int *capacidade) {
    char cpfBusca[15];
    printf("\nCPF do cliente para excluir: ");
    limparBuffer();
    fgets(cpfBusca, 15, stdin);
    limparTexto(cpfBusca);

    int pos = buscarPorCpf(*vetor, *qtd, cpfBusca);
    if(pos == -1) {
        printf("\n[ERRO] Cliente nao encontrado!\n");
        return;
    }

    if((*vetor)[pos].alugado == 0) {
        printf("\n[BLOQUEIO] Cliente possui aluguel ativo (alugado == 0). Nao e permitido exclui-lo!\n");
        return;
    }

    // Compactação (Shift para a esquerda)
    for(int i = pos; i < *qtd - 1; i++) {
        (*vetor)[i] = (*vetor)[i + 1];
    }
    (*qtd)--;

    // Redução de memória opcional via realloc
    if (*capacidade > 5 && *qtd <= *capacidade - 5) {
        *capacidade -= 5;
        Cliente *temp = realloc(*vetor, (*capacidade) * sizeof(Cliente));
        if (temp != NULL) {
            *vetor = temp;
        }
    }

    printf("\n[OK] Cliente excluido com sucesso e vetor compactado!\n");
}

void submenuClientes(Cliente **vetor, int *qtd, int *capacidade) {
    int opcao = 0;
    while(opcao != 6) {
        printf("\n--- PAINEL DE CLIENTES ---\n");
        printf("1. Cadastrar cliente\n");
        printf("2. Listar clientes\n");
        printf("3. Consultar cliente\n");
        printf("4. Alterar cliente\n");
        printf("5. Excluir cliente\n");
        printf("6. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if(opcao == 1) {
            cadastrarCliente(vetor, qtd, capacidade);
        } else if(opcao == 2) {
            listarClientes(*vetor, *qtd);
        } else if(opcao == 3) {
            consultarCliente(*vetor, *qtd);
        } else if(opcao == 4) {
            alterarCliente(*vetor, *qtd);
        } else if(opcao == 5) {
            excluirCliente(vetor, qtd, capacidade);
        } else if(opcao == 6) {
            printf("Voltando...\n");
            return;
        } else {
            printf("\n[ERRO] Opcao invalida!\n");
        }
    }
}