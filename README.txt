SISTEMA DE ALUGUEL DE CARROS DE LUXO
=====================================

Trabalho Pratico de Algoritmos e Programacao II
Curso: Bacharelado em Sistemas de Informacao
Universidade Federal de Uberlandia - UFU
Disciplina: FACOM32201 - Algoritmos e Programacao II
Professor: Prof. Dr. Thiago Pirola Ribeiro
Turma: 2026-1


1. INTEGRANTES DO GRUPO
-----------------------

1. Nome completo: Isabela de Sousa Faria Pereira Rocha
   Matricula: 12521BSI238

2. Nome completo: Isabela Dantas de Moraes
   Matricula: 12521BSI221

3. Nome completo: Celina Vieira Silva
   Matricula: 12521BSI245

4. Nome completo: Gabriel Manzini Toledo
   Matricula: 12521BSI213

5. Nome completo: Kauã Magalhães dos Santos
   Matricula: 12521BSI215


2. DESCRICAO DO PROJETO
-----------------------

O programa implementa um sistema para gerenciamento de uma locadora de
carros de luxo. O sistema trabalha com tres tipos de registros:

- Cliente: armazena os dados dos clientes da locadora;
- Carro: armazena os dados dos veiculos disponiveis;
- Aluguel: relaciona um cliente e um carro, registrando a quantidade de
  dias e o valor total do aluguel.

Os registros sao mantidos em vetores de estruturas com alocacao dinamica
de memoria. Os dados sao carregados de arquivos binarios na inicializacao
e salvos novamente antes do encerramento do programa.


3. AMBIENTE DE COMPILACAO
-------------------------

O projeto foi desenvolvido em linguagem C para execucao no MS-Windows 11
por meio do Visual Studio Code.

Requisitos:

- Visual Studio Code 1.119.1 ou compativel;
- compilador GCC com suporte ao padrao C11;
- terminal PowerShell ou Prompt de Comando do Windows.


4. COMO COMPILAR
----------------

Abra o terminal na pasta que contem os arquivos do projeto e execute:

gcc -std=c11 -Wall -Wextra -Wpedantic main.c cliente.c carro.c aluguel.c relatorios.c util.c -o sistema.exe

Todos os arquivos .c e .h devem permanecer na mesma pasta durante a
compilacao.


5. COMO EXECUTAR
----------------

No PowerShell:

.\sistema.exe

No Prompt de Comando:

sistema.exe

O programa carregara automaticamente os dados existentes nos arquivos
binarios. Para encerrar corretamente, utilize a opcao "Sair do Sistema"
do menu principal. Ao sair, os dados mantidos em memoria serao salvos.


6. FUNCIONALIDADES
------------------

6.1. Clientes

- cadastrar cliente;
- listar todos os clientes;
- consultar cliente por CPF, nome ou CNH;
- alterar os dados de um cliente;
- excluir cliente;
- validar CPF, idade, CNH, telefone, e-mail, nome e CEP;
- impedir CPF, telefone e e-mail duplicados;
- impedir a exclusao de cliente vinculado a aluguel ativo.

6.2. Carros

- cadastrar carro;
- listar toda a frota;
- consultar carro pela placa;
- alterar os dados de um carro;
- excluir carro;
- validar placa, marca, modelo, cor, quilometragem e valor da diaria;
- impedir placas duplicadas;
- impedir a exclusao de carro vinculado a aluguel ativo.

6.3. Alugueis

- cadastrar aluguel;
- listar todos os alugueis;
- consultar por codigo, CPF, placa ou valor total minimo;
- exibir na consulta os dados relacionados do cliente e do carro;
- alterar CPF do cliente, codigo ou quantidade de dias;
- excluir aluguel;
- calcular automaticamente o valor total;
- impedir codigos duplicados;
- permitir aluguel somente com cliente e carro previamente cadastrados;
- impedir que um cliente ou carro tenha mais de um aluguel ativo.

6.4. Relatorios

O sistema gera arquivos de texto com os alugueis filtrados por:

- CPF do cliente;
- placa do carro;
- valor total minimo.

O nome do arquivo de saida e informado pelo usuario no momento da geracao.


7. ARQUIVOS DO PROJETO
----------------------

main.c
    Inicializa os vetores, carrega os arquivos, apresenta o menu principal,
    salva os registros e libera a memoria.

modelos.h
    Define as estruturas Cliente, Carro e Aluguel.

cliente.c / cliente.h
    Implementam o CRUD, as buscas, as validacoes e a persistencia de
    clientes.

carro.c / carro.h
    Implementam o CRUD, as buscas, as validacoes e a persistencia de
    carros.

aluguel.c / aluguel.h
    Implementam o CRUD, as consultas, o relacionamento entre as estruturas
    e a persistencia de alugueis.

relatorios.c / relatorios.h
    Implementam os tres tipos de relatorio em arquivo de texto.

util.c / util.h
    Implementam funcoes auxiliares para tratamento de texto e limpeza do
    buffer de entrada.

clientes.bin
    Armazena o vetor de clientes.

carros.bin
    Armazena o vetor de carros.

alugueis.bin
    Armazena o vetor de alugueis.


8. ARQUIVOS BINARIOS PRE-PREENCHIDOS
------------------------------------

A entrega acompanha os tres arquivos binarios criados pela execucao do
proprio programa. Cada arquivo contem 10 registros:

- clientes.bin: 10 clientes;
- carros.bin: 10 carros;
- alugueis.bin: 10 alugueis.

Os alugueis existentes possuem clientes e carros relacionados validos.

Os arquivos .bin devem permanecer no mesmo diretorio do executavel. Caso
um arquivo ainda nao exista, o respectivo vetor sera iniciado vazio e o
arquivo sera criado quando os dados forem salvos.


9. REGRAS IMPORTANTES DO SISTEMA
--------------------------------

- CPF de cliente, placa de carro e codigo de aluguel sao campos unicos;
- um aluguel so pode ser cadastrado com cliente e carro existentes;
- clientes e carros com aluguel ativo nao podem ser excluidos;
- o valor total e calculado multiplicando a quantidade de dias pelo valor
  da diaria do carro;
- entradas invalidas sao recusadas e solicitadas novamente;
- a memoria dos vetores e alocada e realocada dinamicamente.

