#ifndef VOO_H
#define VOO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char numero_voo[10];
} Voo;

Voo* criar_voo(const char *numero);
void destruir_voo(Voo *v);
void imprimir_voo(Voo *v);

#endif // VOO_H
