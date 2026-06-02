#ifndef MATRIZ_ESPARSA_H
#define MATRIZ_ESPARSA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int col;
    void *value;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    Node **rows;
    int rows_count;
    int cols_count;
} MatrizEsparsa;

MatrizEsparsa* criar_matriz_esparsa();
void expandir_matriz_esparsa(MatrizEsparsa *m, int new_dim);
bool definir_valor(MatrizEsparsa *m, int row, int col, void *value);
void* obter_valor(MatrizEsparsa *m, int row, int col);
bool remover_valor(MatrizEsparsa *m, int row, int col);
void destruir_matriz_esparsa(MatrizEsparsa *m, void (*free_value)(void*));

#endif // MATRIZ_ESPARSA_H
