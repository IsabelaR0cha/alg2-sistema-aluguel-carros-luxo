/*
 * Arquivo: aluguel.h
 * Finalidade: declarar as funcoes utilizadas no gerenciamento de alugueis.
 *
 * Este arquivo apresenta as funcoes de busca, alocacao dinamica, CRUD,
 * persistencia e atualizacao dos status de clientes e carros.
 * As implementacoes dessas funcoes encontram-se no arquivo aluguel.c.
 */

// Protege o header contra inclusoes repetidas durante a compilacao.
#ifndef ALUGUEL_H
#define ALUGUEL_H

// Inclui as definicoes das estruturas Aluguel, Cliente e Carro.
#include "modelos.h"
#include "cliente.h"
#include "carro.h"

// Funções de Verificação e Busca
int codigoExiste(
    Aluguel *alugueis,
    int qtdAlugueis,
    int codigo
);

int existeAluguelPorCpf(
    const Aluguel *alugueis,
    int qtdAlugueis,
    const char *cpf
);

int existeAluguelPorPlaca(
    const Aluguel *alugueis,
    int qtdAlugueis,
    const char *placa
);

void buscarAluguelporcodigo(
    Aluguel *alugueis,
    int qtdAlugueis
);

// Funções de Alocação Dinâmica
int aumentarVetorAlugueis(
    Aluguel **alugueis,
    int qtdAlugueis
);

// Funções de CRUD (ajustadas para suportar alocação dinâmica e realocação)
int cadastrarAluguel(
    Aluguel **alugueis,
    int *qtdAlugueis,
    Cliente *clientes,
    int qtdClientes,
    Carro *carros,
    int qtdCarros
);

void listarAlugueis(
    const Aluguel *alugueis,
    int qtdAlugueis
);

int alterarAluguel(
    Aluguel *alugueis,
    int qtdAlugueis,
    Cliente *clientes,
    int qtdClientes,
    Carro *carros,
    int qtdCarros
);

int excluirAluguel(
    Aluguel **alugueis,
    int *qtdAlugueis,
    Cliente *clientes,
    int qtdClientes,
    Carro *carros,
    int qtdCarros
);

// Funções de Atualização do Status de Cliente e Carro
void atualizarAlugadoCliente(
    Cliente *clientes,
    int qtdClientes,
    const Aluguel *alugueis,
    int qtdAlugueis,
    const char *cpf
);

void atualizarAlugadoCarro(
    Carro *carros,
    int qtdCarros,
    const Aluguel *alugueis,
    int qtdAlugueis,
    const char *placa
);

// Funções de Persistência em Arquivo Binário
int salvarAlugueis(
    Aluguel *alugueis,
    int qtdAlugueis
);

int carregarAlugueis(
    Aluguel **alugueis,
    int *qtdAlugueis
);

// Funções de Menu
void menuAluguel(
    Aluguel **alugueis,
    int *qtdAlugueis,
    Cliente *clientes,
    int qtdClientes,
    Carro *carros,
    int qtdCarros
);

void submenuAlugueis(
    Aluguel **alugueis,
    int *qtdAlugueis,
    Cliente *clientes,
    int qtdClientes,
    Carro *carros,
    int qtdCarros
);

#endif