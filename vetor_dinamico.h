#ifndef VETOR_DINAMICO_H
#define VETOR_DINAMICO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    void **items;
    size_t size;
    size_t capacity;
} VetorDinamico;

VetorDinamico* criar_vetor();
bool inserir_vetor(VetorDinamico *v, void *item);
void* obter_vetor(VetorDinamico *v, size_t index);
bool remover_vetor(VetorDinamico *v, size_t index);
void destruir_vetor(VetorDinamico *v);

#endif // VETOR_DINAMICO_H
