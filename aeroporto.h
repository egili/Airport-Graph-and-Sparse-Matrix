#ifndef AEROPORTO_H
#define AEROPORTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char codigo[4];
    char nome_cidade[100];
} Aeroporto;

Aeroporto* criar_aeroporto(const char *codigo, const char *cidade);

#endif // AEROPORTO_H
