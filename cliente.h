#ifndef CLIENTE_H
#define CLIENTE_H

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