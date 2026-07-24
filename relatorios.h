/*
 * Arquivo: relatorios.h
 * Finalidade: declarar as funcoes utilizadas na geracao de relatorios.
 *
 * Este arquivo apresenta o menu de relatorios e as funcoes responsáveis
 * por gerar consultas de alugueis por CPF, placa e valor total minimo.
 * As implementacoes dessas funcoes encontram-se no arquivo relatorios.c.
 */

 // Protege o header contra inclusoes repetidas durante a compilacao.
#ifndef RELATORIOS_H
#define RELATORIOS_H

// Inclui as definicoes das estruturas Cliente, Carro e Aluguel.
#include "modelos.h"

// 1. Menu principal dos relatorios
    void menuRelatorios(Cliente *clientes, int qtdClientes, Carro *carros, int qtdCarros, Aluguel *alugueis, int qtdAlugueis);
    
// 2. Relatorio de alugueis por CPF do cliente
    void relatorioPorCPF(Cliente *clientes, int qtdClientes, Carro *carros, int qtdCarros, Aluguel *alugueis, int qtdAlugueis);
    
// 3. Relatorio de alugueis por Placa do carro
    void relatorioPorPlaca(Cliente *clientes, int qtdClientes, Carro *carros, int qtdCarros, Aluguel *alugueis, int qtdAlugueis);
    
// 4. Relatorio por Valor Total Minimo
    void relatorioPorValorMinimo(Cliente *clientes, int qtdClientes, Carro *carros, int qtdCarros, Aluguel *alugueis, int qtdAlugueis);
    

#endif
