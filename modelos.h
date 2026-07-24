/*
 * Arquivo: modelos.h
 * Finalidade: reunir as constantes e as estruturas de dados compartilhadas
 * por todos os módulos do sistema de locacao de carros.
 *
 * Este arquivo nao implementa funcoes. Ele apenas define os tipos Cliente,
 * Carro e Aluguel para serem utilizados nos demais arquivos do projeto.
 */

// Protege o header contra inclusoes repetidas durante a compilacao.
#ifndef MODELOS_H
#define MODELOS_H

typedef struct {
    char nome[100];
    char cnh[20];
    char cpf[15];
    char telefone[20]; 
    char email[100];
    char cep[10];
    int alugado;
    int idade;
    int ativo;
} Cliente;

typedef struct {
    char marca[50];
    char modelo[50];
    char cor[20];
    char placa[8];
    float km, valor;
    int alugado;
} Carro;

typedef struct {
    int codigo_aluguel;
    char cpf_cliente[15];
    char placa[8];
    int dias;
    double valor_total;
} Aluguel;

// Encerra a protecao iniciada pelo #ifndef.
#endif