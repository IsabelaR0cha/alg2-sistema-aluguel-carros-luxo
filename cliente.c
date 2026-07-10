#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "cliente.h"
#include "util.h"

int buscarPorCpf(Cliente *vetor, int qtd, char *cpfBusca) {
    // Fail-fast: ignoramos clientes com ativo == 0, pois representam
    // registros "excluídos" (soft delete) e não devem ser encontrados.
    for(int i = 0; i < MAX_CLIENTES; i++)
        if(vetor[i].ativo == 1 && strcmp(vetor[i].cpf, cpfBusca) == 0) {
            return i;
        }
    return -1;
}

int buscarPorEmail(Cliente *vetor, int qtd, char *emailBusca) {
    for(int i = 0; i < MAX_CLIENTES; i++)
        if(vetor[i].ativo == 1 && strcmp(vetor[i].email, emailBusca) == 0) {
            return i;
        }
    return -1;
}

int buscarPorTelefone(Cliente *vetor, int qtd, char *telefoneBusca) {
    for(int i = 0; i < MAX_CLIENTES; i++)
        if(vetor[i].ativo == 1 && strcmp(vetor[i].telefone, telefoneBusca) == 0) {
            return i;
        }
    return -1;
}

int validarDDD(char *telefone) {
    // Lista fixa com todos os DDDs oficiais brasileiros.
    const char *dddsValidos[] = {
        "11", "12", "13", "14", "15", "16", "17", "18", "19",
        "21", "22", "24", "27", "28",
        "31", "32", "33", "34", "35", "37", "38",
        "41", "42", "43", "44", "45", "46", "47", "48", "49",
        "51", "53", "54", "55",
        "61", "62", "63", "64", "65", "66", "67", "68", "69",
        "71", "73", "74", "75", "77", "79",
        "81", "82", "83", "84", "85", "86", "87", "88", "89",
        "91", "92", "93", "94", "95", "96", "97", "98", "99"
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
    // Fail-fast: testa o tamanho antes de percorrer caractere a caractere,
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

void cadastrarCliente(Cliente *vetor, int *qtd) {
    int pos = -1;

    // Procura a primeira posição livre (ativo == 0) no vetor para reutilizar
    // o espaço de um cliente excluído anteriormente, em vez de exigir que
    // o vetor cresça — já que seu tamanho é fixo (MAX_CLIENTES).
    for(int i = 0; i < MAX_CLIENTES; i++) {
        if(vetor[i].ativo == 0) {
            pos = i;
            break;
        }
    }

    if(pos == -1) {
        printf("\n[ERRO] Limite de clientes atingido!\n");
        return;
    }

    char cpfTemp[15];
    printf("\nCPF (apenas 11 numeros): ");
    limparBuffer();
    fgets(cpfTemp, 15, stdin);
    limparTexto(cpfTemp);

    // Fail-fast: valida o formato do CPF antes de verificar duplicidade,
    // pois não há motivo para buscar no vetor um CPF que já é inválido.
    if(!ehCpfValido(cpfTemp)) {
        printf("\n[ERRO] CPF invalido! Digite apenas 11 numeros.\n");
        return;
    }

    if(buscarPorCpf(vetor, *qtd, cpfTemp) != -1) {
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

    if(buscarPorTelefone(vetor, *qtd, telefoneTemp) != -1) {
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

    if(buscarPorEmail(vetor, *qtd, emailTemp) != -1) {
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

    strcpy(vetor[pos].cpf, cpfTemp);
    vetor[pos].idade = idadeTemp;
    strcpy(vetor[pos].cnh, cnhTemp);
    strcpy(vetor[pos].telefone, telefoneTemp);
    strcpy(vetor[pos].email, emailTemp);
    strcpy(vetor[pos].nome, nomeTemp);
    strcpy(vetor[pos].cep, cepTemp);
    vetor[pos].ativo = 1;
    (*qtd)++;

    printf("\n[OK] Cliente cadastrado com sucesso!\n");
}

void listarClientes(Cliente *vetor, int qtd) {
    int total = 0;

    for(int i = 0; i < MAX_CLIENTES; i++) {
        if(vetor[i].ativo == 1) {
            printf("\nNome: %s\nCPF: %s\nIdade: %d\nCNH: %s\nTelefone: %s\nEmail: %s\nCEP: %s\n",
                   vetor[i].nome, vetor[i].cpf, vetor[i].idade, vetor[i].cnh, vetor[i].telefone, vetor[i].email, vetor[i].cep);
            total++;
        }
    }
    if(total == 0) {
        printf("\nNenhum cliente cadastrado.\n");
    }
}

void consultarCliente(Cliente *vetor, int qtd) {
    char cpfBusca[15];
    printf("\nCPF para buscar: ");
    limparBuffer();
    fgets(cpfBusca, 15, stdin);
    limparTexto(cpfBusca);

    int pos = buscarPorCpf(vetor, qtd, cpfBusca);
    if(pos == -1) {
        printf("\n[ERRO] Nao encontrado!\n");
    } else {
        printf("\n[OK] Encontrado!\nNome: %s\nCPF: %s\nEmail: %s\n", vetor[pos].nome, vetor[pos].cpf, vetor[pos].email);
    }
}

void alterarCliente(Cliente *vetor, int qtd) {
    char cpfBusca[15], novoCpf[15], novoTelefone[20], novoEmail[100], novaCnh[20], novoNome[100], novoCep[10];
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
    printf("\nO que deseja alterar?\n1. CPF\n2. Nome\n3. Idade\n4. CNH\n5. Telefone\n6. Email\n7. CEP\nEscolha: ");
    scanf("%d", &opcaoAlterar);

    if(opcaoAlterar == 1) {
        printf("Novo CPF: ");
        limparBuffer();
        fgets(novoCpf, 15, stdin);
        limparTexto(novoCpf);

        if(!ehCpfValido(novoCpf)) {
            printf("\n[ERRO] CPF invalido!\n");
            return;
        }

        // Verifica duplicidade somente depois de confirmar que o formato
        // do novo CPF é válido (fail-fast), evitando busca desnecessária.
        if(buscarPorCpf(vetor, qtd, novoCpf) != -1) {
            printf("\n[ERRO] CPF ja cadastrado!\n");
            return;
        }

        strcpy(vetor[pos].cpf, novoCpf);
        printf("\n[OK] CPF atualizado!\n");

    } else if(opcaoAlterar == 2) {
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

    } else if(opcaoAlterar == 3) {
        printf("Nova Idade: ");
        limparBuffer();
        scanf("%d", &novaIdade);
        limparBuffer();

        // Reaproveita a mesma regra usada no cadastro (validarIdade),
        // em vez de repetir a comparação "novaIdade < 18" isoladamente,
        // para manter uma única fonte de verdade sobre o que é válido.
        if(!validarIdade(novaIdade)) {
            printf("\n[ERRO] Idade invalida!\n");
            return;
        }

        vetor[pos].idade = novaIdade;
        printf("\n[OK] Idade atualizada!\n");

    } else if(opcaoAlterar == 4) {
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

    } else if(opcaoAlterar == 5) {
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

    } else if(opcaoAlterar == 6) {
        printf("Novo Email: ");
        limparBuffer();
        fgets(novoEmail, 100, stdin);
        limparTexto(novoEmail);

        // Mesma validação de formato aplicada no cadastro original,
        // garantindo que a alteração não relaxe uma regra já existente.
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

    } else if(opcaoAlterar == 7) {
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

void excluirCliente(Cliente *vetor, int *qtd) {
    char cpfBusca[15];
    printf("\nCPF para excluir: ");
    limparBuffer();
    fgets(cpfBusca, 15, stdin);
    limparTexto(cpfBusca);

    int pos = buscarPorCpf(vetor, *qtd, cpfBusca);
    if(pos == -1) {
        printf("\n[ERRO] Nao encontrado!\n");
        return;
    }

    // Soft delete: apenas desativamos o registro em vez de deslocar todos
    // os elementos seguintes do vetor para "tapar o buraco". Isso é O(1)
    // em vez de O(n), e a posição liberada será reaproveitada no próximo
    // cadastro (ver busca por 'ativo == 0' em cadastrarCliente).
    vetor[pos].ativo = 0;
    (*qtd)--;

    printf("\n[OK] Removido com sucesso!\n");
}

void submenuClientes(Cliente *vetor, int *qtd) {
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
            cadastrarCliente(vetor, qtd);
        } else if(opcao == 2) {
            listarClientes(vetor, *qtd);
        } else if(opcao == 3) {
            consultarCliente(vetor, *qtd);
        } else if(opcao == 4) {
            alterarCliente(vetor, *qtd);
        } else if(opcao == 5) {
            excluirCliente(vetor, qtd);
        } else if(opcao == 6) {
            printf("Voltando...\n");
            return;
        } else {
            printf("\n[ERRO] Opcao invalida!\n");
        }
    }
}