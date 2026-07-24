/*
 * Arquivo: cliente.h
 * Finalidade: declarar as funções utilizadas no gerenciamento de clientes.
 * Implementações em: cliente.c
 */

// Protege contra inclusões repetidas durante a compilação
#ifndef CLIENTE_H
#define CLIENTE_H

#include "modelos.h"

// Funções de Busca
int buscarPorCpf(Cliente *vetor, int qtd, char *cpfBusca);
int buscarPorNome(Cliente *vetor, int qtd, char *nomeBusca);
int buscarPorCnh(Cliente *vetor, int qtd, char *cnhBusca);
int buscarPorEmail(Cliente *vetor, int qtd, char *emailBusca);
int buscarPorTelefone(Cliente *vetor, int qtd, char *telefoneBusca);

// Funções de Validação
int validarDDD(char *telefone);
int validarEmail(char *email);
int validarCnhCarro(char *cnh);
int ehCpfValido(char *cpf);
int validarIdade(int idade);
int validarCEP(char *cep);
int validarTelefoneTamanho(char *telefone);
int validarNome(char *nome);

// Funções de CRUD (vetor e capacidade por ponteiro para suportar realloc)
void cadastrarCliente(Cliente **vetor, int *qtd, int *capacidade);
void listarClientes(Cliente *vetor, int qtd);
void consultarCliente(Cliente *vetor, int qtd);
void alterarCliente(Cliente *vetor, int qtd);
void excluirCliente(Cliente **vetor, int *qtd, int *capacidade);
void submenuClientes(Cliente **vetor, int *qtd, int *capacidade);
void salvarClientesArquivo(Cliente *vetor, int qtd);
void carregarClientesArquivo(Cliente **vetor, int *qtd, int *capacidade);

#endif