#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo o tamanho solicitado
#define MAX_CLIENTES 5 

typedef struct{
    char nome[100], cnh[20], cpf[15], telefone[20], email[100], cep[10]; // CPF é char, pois pode conter zeros à esquerda
    int idade, ativo; // 'ativo' para marcar se o cliente está ativo (1) ou excluído (0)
} Cliente;

// Limpar o \n do final da string lida com fgets
void limparTexto(char *str){ 
    str[strcspn(str, "\n")] = '\0'; 
}

// Retorna a posição do cliente ou -1 se não encontrado pelo CPF
int buscarPorCpf(Cliente *vetor, int qtd, char *cpfBusca){
    for(int i = 0; i < qtd; i++)
        if(vetor[i].ativo == 1 && strcmp(vetor[i].cpf, cpfBusca) == 0){
            return i;
        }
    return -1; 
}

// Retorna a posição do cliente ou -1 se não encontrado pelo E-mail
int buscarPorEmail(Cliente *vetor, int qtd, char *emailBusca){
    for(int i = 0; i < qtd; i++)
        if(vetor[i].ativo == 1 && strcmp(vetor[i].email, emailBusca) == 0){
            return i;
        }
    return -1; 
}

// Retorna a posição do cliente ou -1 se não encontrado pelo Telefone
int buscarPorTelefone(Cliente *vetor, int qtd, char *telefoneBusca){
    for(int i = 0; i < qtd; i++)
        if(vetor[i].ativo == 1 && strcmp(vetor[i].telefone, telefoneBusca) == 0){
            return i;
        }
    return -1; 
}

// Função auxiliar para validar se a carteira permite dirigir carro (B, C, D, E)
int validarCnhCarro(char *cnh){
    if (strchr(cnh, 'B') != NULL || strchr(cnh, 'b') != NULL ||
        strchr(cnh, 'C') != NULL || strchr(cnh, 'c') != NULL ||
        strchr(cnh, 'D') != NULL || strchr(cnh, 'd') != NULL ||
        strchr(cnh, 'E') != NULL || strchr(cnh, 'e') != NULL) {
        return 1;
    }
    return 0;
}

// Função para cadastrar cliente
void cadastrarCliente(Cliente *vetor, int *qtd){
    if(*qtd >= MAX_CLIENTES){ 
        printf("\n[ERRO] Limite atingido!\n"); 
        return; 
    }

    char cpfVerifica[15];
    printf("\nCPF: "); 
    setbuf(stdin, NULL); 
    fgets(cpfVerifica, 15, stdin); 
    limparTexto(cpfVerifica); 

    if(buscarPorCpf(vetor, *qtd, cpfVerifica) != -1){ 
        printf("\n[ERRO] CPF duplicado!\n"); 
        return;
    }

    int idadeVerifica;
    printf("Idade: ");
    scanf("%d", &idadeVerifica);
    if(idadeVerifica < 18){
        printf("\n[ERRO] O cliente deve ser maior de 18 anos!\n");
        return;
    }

    char cnhVerifica[20];
    printf("CNH: ");
    setbuf(stdin, NULL);
    fgets(cnhVerifica, 20, stdin);
    limparTexto(cnhVerifica);
    if(!validarCnhCarro(cnhVerifica)){
        printf("\n[ERRO] CNH nao compativel com carro (exige B, C, D ou E)!\n");
        return;
    }

    char telefoneVerifica[20];
    printf("Telefone: "); 
    fgets(telefoneVerifica, 20, stdin); 
    limparTexto(telefoneVerifica);

    if(buscarPorTelefone(vetor, *qtd, telefoneVerifica) != -1){
        printf("\n[ERRO] Telefone duplicado!\n");
        return;
    }

    char emailVerifica[100];
    printf("Email: "); 
    fgets(emailVerifica, 100, stdin); 
    limparTexto(emailVerifica);

    if(buscarPorEmail(vetor, *qtd, emailVerifica) != -1){
        printf("\n[ERRO] E-mail duplicado!\n");
        return;
    }
    
    int pos = *qtd; 
    strcpy(vetor[pos].cpf, cpfVerifica); 
    vetor[pos].idade = idadeVerifica;
    strcpy(vetor[pos].cnh, cnhVerifica);
    strcpy(vetor[pos].telefone, telefoneVerifica);
    strcpy(vetor[pos].email, emailVerifica);

    printf("Nome: "); 
    fgets(vetor[pos].nome, 100, stdin); 
    limparTexto(vetor[pos].nome);

    printf("CEP: "); 
    fgets(vetor[pos].cep, 10, stdin); 
    limparTexto(vetor[pos].cep);

    vetor[pos].ativo = 1; (*qtd)++;             
    printf("\nCadastrado com sucesso!\n");
}

// Função para listar clientes
void listarClientes(Cliente *vetor, int qtd){
    int total = 0;
    printf("\nCLIENTES\n");
    for(int i = 0; i < qtd; i++) {
        if (vetor[i].ativo == 1) {
            printf("\nNome: %s\nCPF: %s\nIdade: %d\nCNH: %s\nTelefone: %s\nEmail: %s\nCEP: %s\n", 
                   vetor[i].nome, vetor[i].cpf, vetor[i].idade, vetor[i].cnh, vetor[i].telefone, vetor[i].email, vetor[i].cep);
            total++;
        } 
    }
    if(total == 0){
        printf("Nenhum cliente cadastrado.\n");
    } 
}

// Função para consultar cliente
void consultarCliente(Cliente *vetor, int qtd){
    char cpfBusca[15];
    printf("\nCPF para buscar: ");
    setbuf(stdin, NULL);
    fgets(cpfBusca, 15, stdin);
    limparTexto(cpfBusca);

    int pos = buscarPorCpf(vetor, qtd, cpfBusca);
    if(pos == -1){
        printf("\n[ERRO] Nao encontrado!\n");
    }else{
        printf("\nEncontrado!\nNome: %s\nCPF: %s\nEmail: %s\n", vetor[pos].nome, vetor[pos].cpf, vetor[pos].email);
    }
}

// Função para alterar cliente
void alterarCliente(Cliente *vetor, int qtd){
    char cpfBusca[15], novoCpf[15], novoTelefone[20], novoEmail[100], novaCnh[20];
    int novaIdade;
    printf("\nCPF para alterar: ");
    setbuf(stdin, NULL);
    fgets(cpfBusca, 15, stdin);
    limparTexto(cpfBusca);

    int pos = buscarPorCpf(vetor, qtd, cpfBusca);
    if(pos == -1){ 
        printf("\n[ERRO] Nao encontrado!\n");
        return; 
    }

    int opcaoAlterar = 0;
    printf("\nO que deseja alterar?\n1. CPF\n2. Nome\n3. Idade\n4. CNH\n5. Telefone\n6. Email\n7. CEP\nEscolha: ");
    scanf("%d", &opcaoAlterar);

    if(opcaoAlterar == 1){
        printf("Novo CPF: "); 
        setbuf(stdin, NULL); 
        fgets(novoCpf, 15, stdin); 
        limparTexto(novoCpf);

        if(buscarPorCpf(vetor, qtd, novoCpf) != -1){
            printf("\n[ERRO] CPF ja cadastrado!\n");
        }
        else{ 
            strcpy(vetor[pos].cpf, novoCpf);
            printf("\nCPF updated!\n"); 
        }
    }else if(opcaoAlterar == 2){
        printf("Novo Nome: "); 
        setbuf(stdin, NULL); 
        fgets(vetor[pos].nome, 100, stdin); 
        limparTexto(vetor[pos].nome);

        printf("\nNome atualizado!\n");

    }else if(opcaoAlterar == 3){
        printf("Nova Idade: "); 
        setbuf(stdin, NULL);
        scanf("%d", &novaIdade);
        setbuf(stdin, NULL);

        if(novaIdade < 18) {
            printf("\n[ERRO] Idade nao pode ser menor de 18 anos!\n");
        } else {
            vetor[pos].idade = novaIdade;
            printf("\nIdade atualizada!\n");
        }

    }else if(opcaoAlterar == 4){
        printf("Nova CNH: "); 
        setbuf(stdin, NULL); 
        fgets(novaCnh, 20, stdin); 
        limparTexto(novaCnh);

        if(!validarCnhCarro(novaCnh)) {
            printf("\n[ERRO] CNH nao compativel com carro!\n");
        } else {
            strcpy(vetor[pos].cnh, novaCnh);
            printf("\nCNH atualizada!\n");
        }

    }else if(opcaoAlterar == 5){
        printf("Novo Telefone: "); 
        setbuf(stdin, NULL); 
        fgets(novoTelefone, 20, stdin); 
        limparTexto(novoTelefone);

        if(buscarPorTelefone(vetor, qtd, novoTelefone) != -1){
            printf("\n[ERRO] Telefone ja em uso!\n");
        }
        else{ 
            strcpy(vetor[pos].telefone, novoTelefone); 
            printf("\nTelefone atualizado!\n");
        }
    }else if(opcaoAlterar == 6){
        printf("Novo Email: "); 
        setbuf(stdin, NULL); 
        fgets(novoEmail, 100, stdin); 
        limparTexto(novoEmail);

        if(buscarPorEmail(vetor, qtd, novoEmail) != -1){
            printf("\n[ERRO] E-mail ja em uso!\n");
        }
        else{ 
            strcpy(vetor[pos].email, novoEmail); 
            printf("\nEmail atualizado!\n"); 
        }
    }else if(opcaoAlterar == 7){
        printf("Novo CEP: "); 
        setbuf(stdin, NULL); 
        fgets(vetor[pos].cep, 10, stdin); 
        limparTexto(vetor[pos].cep);

        printf("\nCEP updated!\n");

    }else{
        printf("\nOpcao invalida!\n");
    }
}

// Função para excluir cliente
void excluirCliente(Cliente *vetor, int qtd){
    char cpfBusca[15];
    printf("\nCPF para excluir: ");
    setbuf(stdin, NULL); 
    fgets(cpfBusca, 15, stdin);
    limparTexto(cpfBusca);

    int pos = buscarPorCpf(vetor, qtd, cpfBusca);
    if(pos == -1){ 
        printf("\n[ERRO] Nao encontrado!\n"); 
        return; 
    }
    vetor[pos].ativo = 0; 
    printf("\nRemovido com sucesso!\n");
}

// Submenu de clientes, agora recebendo o vetor e a quantidade por referência
void submenuClientes(Cliente *vetor, int *qtd){
    int opcao = 0; 
    while(opcao != 6){
        printf("\n--- PAINEL DE CLIENTES ---\n");
        printf("1. Cadastrar cliente\n");
        printf("2. Listar clientes\n");
        printf("3. Consultar cliente\n");
        printf("4. Alterar cliente\n");
        printf("5. Excluir cliente\n");
        printf("6. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if(opcao == 1){
            cadastrarCliente(vetor, qtd); 
        }else if(opcao == 2){
            listarClientes(vetor, *qtd);
        }else if(opcao == 3){
            consultarCliente(vetor, *qtd);
        }else if(opcao == 4){
            alterarCliente(vetor, *qtd);
        }else if(opcao == 5){
            excluirCliente(vetor, *qtd);
        }else if(opcao == 6){
            printf("Voltando...\n");
            return; 
        }else{
            printf("Erro: Opcao invalida!\n");
        }
    }
}

void submenuCarros(){
    int opcao = 0;
    while(opcao != 6){
        printf("\n--- CONTROLE DE FROTA ---\n");
        printf("1. Cadastrar veiculo\n");
        printf("2. Listar frota\n");
        printf("3. Consultar veiculo\n");
        printf("4. Alterar dados do veiculo\n");
        printf("5. Remover veiculo\n");
        printf("6. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if(opcao == 1){
            printf("Chamando funcao de cadastrar veiculo...\n");
            exit(1);
        }else if(opcao == 6){
            printf("Voltando...\n");
            return;
        }else if(opcao > 1 && opcao < 6){
            printf("Chamando outra funcao de frota...\n");
            exit(1);
        }else{
            printf("Erro: Opcao invalida!\n");
        }
    }
}

void submenuAlugueis(){
    int opcao = 0;
    while(opcao != 2){
        printf("\n--- CENTRAL DE LOCACOES ---\n");
        printf("1. Listar locacoes ativas\n");
        printf("2. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if(opcao == 1){
            printf("Chamando funcao de listar locacoes...\n");
            exit(1);
        }else if(opcao == 2){
            printf("Voltando...\n");
            return;
        }else{
            printf("Erro: Opcao invalida!\n");
        }
    }
}

// Menu principal gerenciando o vetor compartilhado
void menuPrincipal(){
    Cliente vetorClientes[MAX_CLIENTES];
    int qtdClientes = 0;
    int opcao = 0;

    while(opcao != 4){
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Gerenciar clientes\n");
        printf("2. Gerenciar carros\n");
        printf("3. Gerenciar aluguéis\n");
        printf("4. Sair do Sistema\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if(opcao == 1){
            submenuClientes(vetorClientes, &qtdClientes); 
        }else if(opcao == 2){
            submenuCarros();
        }else if(opcao == 3){
            submenuAlugueis();
        }else if(opcao == 4){
            printf("Encerrando o sistema de locacao...\n");
            return;
        }else{
            printf("Erro: Opcao invalida! Tente novamente.\n");
        }
    }
}

int main(){
    menuPrincipal();
    return 0;
}