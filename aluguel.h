/*
 * Arquivo: aluguel.h
 * Finalidade: declarar as funcoes utilizadas no gerenciamento de alugueis.
 *
 * Este arquivo informa quais operacoes do modulo de alugueis podem ser
 * utilizadas pelos outros arquivos do sistema. As implementacoes dessas
 * funcoes encontram-se no arquivo aluguel.c.
 */

// Protege o header contra inclusoes repetidas durante a compilacao.
#ifndef ALUGUEL_H
#define ALUGUEL_H

/*
 * Inclui modelos.h porque as declaracoes abaixo utilizam a estrutura
 * Aluguel e a constante Maximocadastros.
 */
#include "modelos.h"

int codigoExiste(
    Aluguel alugueis[],
    int qtdAlugueis,
    int codigo
);

void cadastrarAluguel(
    Aluguel alugueis[],
    int *qtdAlugueis
);

void listarAlugueis(
    Aluguel alugueis[],
    int qtdAlugueis
);

void buscarAluguel(
    Aluguel alugueis[],
    int qtdAlugueis
);

void alterarAluguel(
    Aluguel alugueis[],
    int qtdAlugueis
);

void excluirAluguel(
    Aluguel alugueis[],
    int *qtdAlugueis
);

void menuAluguel(
    Aluguel alugueis[],
    int *qtdAlugueis
);

void submenuAlugueis(
    Aluguel alugueis[],
    int *qtdAlugueis
);

#endif