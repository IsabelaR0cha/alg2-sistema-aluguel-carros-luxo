#ifndef RELATORIOS_H
#define RELATORIOS_H

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
