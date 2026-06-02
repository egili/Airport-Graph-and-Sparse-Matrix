#ifndef VETOR_DINAMICO_H
#define VETOR_DINAMICO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    void **itens;
    size_t tamanho;
    size_t capacidade;
} VetorDinamico;

VetorDinamico* criar_vetor();
bool inserir_vetor(VetorDinamico *vetor, void *item);
void* obter_vetor(VetorDinamico *vetor, size_t indice);
bool remover_vetor(VetorDinamico *vetor, size_t indice);
void destruir_vetor(VetorDinamico *vetor);

#endif // VETOR_DINAMICO_H
