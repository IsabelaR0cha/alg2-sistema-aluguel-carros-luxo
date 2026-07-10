#ifndef CARRO_H
#define CARRO_H

#include "modelos.h"

int validarPlaca(char *placa);
int placaExiste(Carro *carros, int cadastrados, char *placa);
int cadastrarCarro(Carro *car, Carro *carros, int cadastrados);

void consultarCarro(Carro *carros, int cadastrados, char *placa);
void listarCarros(Carro *carros, int cadastrados);
void alterarCarro(Carro *carros, int cadastrados, char *placa);
void excluirCarroPlaca(
    Carro *carros,
    int *cadastrados,
    char *placa
);

int menu(void);
void submenuCarros(Carro carros[], int *cadastrados);

#endif