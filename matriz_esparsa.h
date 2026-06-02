#ifndef MATRIZ_ESPARSA_H
#define MATRIZ_ESPARSA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct No {
    int coluna;
    void *valor;
    struct No *esquerda;
    struct No *direita;
} No;

typedef struct {
    No **linhas;
    int quantidade_linhas;
    int quantidade_colunas;
} MatrizEsparsa;

MatrizEsparsa* criar_matriz_esparsa();
void expandir_matriz_esparsa(MatrizEsparsa *matriz, int nova_dimensao);
bool definir_valor_matriz(MatrizEsparsa *matriz, int linha, int coluna, void *valor);
void* obter_valor_matriz(MatrizEsparsa *matriz, int linha, int coluna);
bool remover_valor_matriz(MatrizEsparsa *matriz, int linha, int coluna);
void destruir_matriz_esparsa(MatrizEsparsa *matriz, void (*liberar_valor)(void*));

#endif // MATRIZ_ESPARSA_H
