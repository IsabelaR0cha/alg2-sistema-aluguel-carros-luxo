#ifndef ALUGUEL_H
#define ALUGUEL_H

#include "modelos.h"
#include "cliente.h"
#include "carro.h"

// BUSCAS

void buscarAluguelporcodigo(
    Aluguel *alugueis,
    int qtdAlugueis
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
//  ALOCAÇÃO
int aumentarVetorAlugueis(
    Aluguel **alugueis,
    int qtdAlugueis
);
//  CRUD
int cadastrarAluguel(
    Aluguel **alugueis,
    int *qtdAlugueis,
    Cliente *clientes,
    int qtdClientes,
    Carro *carros,
    int qtdCarros
);
void listarAlugueis(
    const Aluguel *alugueis, int qtdAlugueis
);
int alterarAluguel(
    Aluguel *alugueis,
    int qtdAlugueis,
    Carro *carros,
    int qtdCarros
);
int excluirAluguel(
    Aluguel **alugueis,
    int *qtdAlugueis
);
/* ===========================
   PERSISTÊNCIA
   =========================== */
int salvarAlugueis(
    Aluguel *alugueis,
    int qtdAlugueis
);
int carregarAlugueis(
    Aluguel **alugueis,
    int *qtdAlugueis
);
/* ===========================
   MENU
   =========================== */
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