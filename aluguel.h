#ifndef ALUGUEL_H
#define ALUGUEL_H

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