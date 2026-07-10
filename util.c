#include <stdio.h>
#include <string.h>

/*
 * limparTexto
 * Remove o caractere de nova linha ('\n') deixado no final de uma string
 * lida com fgets(), substituindo-o por '\0' (fim de string).
 *
 * Parâmetros:
 * - str: string a ser tratada (modificada diretamente, por referência).
 *
 * Retorno: void (a função não retorna valor; ela altera 'str' no local).
 */
void limparTexto(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

/*
 * limparBuffer
 * Descarta todos os caracteres restantes no buffer de entrada (stdin) até
 * encontrar uma nova linha ou o fim do arquivo.
 *
 * Por que é necessário: scanf("%d", ...) não consome o '\n' deixado no
 * buffer após o Enter. Se um fgets() for chamado em seguida sem essa
 * limpeza, ele lê apenas esse '\n' residual e retorna uma string vazia,
 * "pulando" a pergunta seguinte. Por isso limparBuffer() é chamada sempre
 * entre um scanf() e um fgets() subsequente.
 *
 * Parâmetros: nenhum.
 * Retorno: void.
 */
void limparBuffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}