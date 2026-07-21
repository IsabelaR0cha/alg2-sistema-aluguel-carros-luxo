/*
 * Arquivo: cliente.h
 * Finalidade: declarar as funcoes utilizadas no gerenciamento de clientes.
 *
 * Este arquivo apresenta as funcoes de busca, validacao e CRUD de clientes.
 * As implementacoes dessas funcoes encontram-se no arquivo cliente.c.
 */

// Protege o header contra inclusoes repetidas durante a compilacao.
#ifndef CLIENTE_H
#define CLIENTE_H

// Inclui a definicao da estrutura Cliente.
#include "modelos.h"

// Funções de Busca (atualizadas para contemplar CPF, Nome, CNH, Email e Telefone)
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

// Funções de CRUD (ajustadas para suportar alocação dinâmica e realocação)
void cadastrarCliente(Cliente **vetor, int *qtd, int *capacidade);
void listarClientes(Cliente *vetor, int qtd);
void consultarCliente(Cliente *vetor, int qtd);
void alterarCliente(Cliente *vetor, int qtd);
void excluirCliente(Cliente **vetor, int *qtd, int *capacidade);
void submenuClientes(Cliente **vetor, int *qtd, int *capacidade);

#endif
