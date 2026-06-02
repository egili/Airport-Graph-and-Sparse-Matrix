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
bool adicionar_ao_percurso(Percurso *p, Aeroporto *a, Voo *v);
void imprimir_percurso(Percurso *p);
void destruir_percurso(Percurso *p);

#endif // PERCURSO_H
