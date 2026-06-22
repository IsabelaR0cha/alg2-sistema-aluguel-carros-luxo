#include<stdio.h>
#include<stdlib.h>

void submenuClientes(){
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
            printf("Chamando funcao de cadastrar cliente...\n");
            exit(1); //1 significa que a função ainda n está pronta
        }else if(opcao == 2){
            printf("Chamando funcao de listar clientes...\n");
            exit(1);
        }else if(opcao == 3){
            printf("Chamando funcao de consultar cliente...\n");
            exit(1);
        }else if(opcao == 4){
            printf("Chamando funcao de alterar cliente...\n");
            exit(1);
        }else if(opcao == 5){
            printf("Chamando funcao de excluir cliente...\n");
            exit(1);
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

void menuPrincipal(){
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
            submenuClientes();
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
