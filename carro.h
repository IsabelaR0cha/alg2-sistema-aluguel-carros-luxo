/*
 * Arquivo: carro.h
 * Finalidade: declarar as funcoes utilizadas no gerenciamento de carros.
 *
 * Este arquivo apresenta as operacoes disponiveis para cadastrar, listar,
 * consultar, alterar e excluir carros. A implementacao dessas funcoes
 * encontra-se no arquivo carro.c.
 */

// Protege o header contra inclusoes repetidas durante a compilacao.
#ifndef CARRO_H
#define CARRO_H

// Inclui a definicao da estrutura Carro e a constante MAX_CAR.
#include "modelos.h"

int validarPlaca(char *placa);
int validarMarca(char *marca);
int validarModelo(char *modelo);
int validarCor(char *cor);

int cadastrarCarro(Carro **carros, int *cadastrados);
void consultarCarro(Carro *carros, int cadastrados);
void listarCarros(Carro *carros, int cadastrados);
void alterarCarro(Carro *carros, int cadastrados);
void excluirCarroPlaca(Carro **carros, int *cadastrados);

void pedirPlaca(char *placa);
void exibirDadosCarro(Carro *carro);
Carro* buscarCarroPorPlaca(Carro *carros, int cadastrados, char *placa);
void submenuCarros(Carro **carros, int *cadastrados);

void salvarCarros(Carro *carros, int cadastrados);
Carro* carregarCarros(int *cadastrados);

#endif