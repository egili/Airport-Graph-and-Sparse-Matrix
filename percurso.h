#ifndef PERCURSO_H
#define PERCURSO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "aeroporto.h"
#include "voo.h"

typedef struct {
    Aeroporto **aeroportos;
    Voo **voos;
    int tamanho;
    int capacidade;
} Percurso;

Percurso* criar_percurso();
bool adicionar_ao_percurso(Percurso *percurso, Aeroporto *aeroporto, Voo *voo);
void imprimir_percurso(Percurso *percurso);
void destruir_percurso(Percurso *percurso);

#endif // PERCURSO_H
