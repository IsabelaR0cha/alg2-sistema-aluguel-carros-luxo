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

// Inclui a definicao da estrutura Cliente e a constante MAX_CLIENTES.
#include "modelos.h"

int buscarPorCpf(Cliente *vetor, int qtd, char *cpfBusca);
int buscarPorEmail(Cliente *vetor, int qtd, char *emailBusca);
int buscarPorTelefone(Cliente *vetor, int qtd, char *telefoneBusca);

int validarDDD(char *telefone);
int validarEmail(char *email);
int validarCnhCarro(char *cnh);
int ehCpfValido(char *cpf);
int validarIdade(int idade);
int validarCEP(char *cep);
int validarTelefoneTamanho(char *telefone);
int validarNome(char *nome);

void cadastrarCliente(Cliente *vetor, int *qtd);
void listarClientes(Cliente *vetor, int qtd);
void consultarCliente(Cliente *vetor, int qtd);
void alterarCliente(Cliente *vetor, int qtd);
void excluirCliente(Cliente *vetor, int *qtd);
void submenuClientes(Cliente *vetor, int *qtd);

#endif