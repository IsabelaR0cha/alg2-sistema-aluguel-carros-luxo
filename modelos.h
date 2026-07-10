#ifndef MODELOS_H
#define MODELOS_H

#define MAX_CAR 5
#define Maximocadastros 5

// Máximo de clientes que podem ser cadastrados.
// Usado tanto para dimensionar a alocação dinâmica em main() quanto como
// Limite de iteração em todas as funções que percorrem o vetor de clientes.
#define MAX_CLIENTES 5

typedef struct {
    char nome[100], 
    cnh[20], 
    cpf[15], 
    telefone[20], 
    email[100], 
    cep[10];
    int idade, ativo;
} Cliente;

typedef struct {
    char marca[50];
    char modelo[50];
    char cor[20];
    char placa[8];
    float km, valor;
} Carro;

typedef struct {
    int codigo_aluguel;
    char cpf_cliente[15];
    char placa[8];
    char data_retirada[11];
    char data_devolucao[11];
    double valor_total;
} Aluguel;

#endif