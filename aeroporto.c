#include "aeroporto.h"

Aeroporto* criar_aeroporto(const char *codigo, const char *cidade) {
    if (!codigo || !cidade) return NULL;

    Aeroporto *novo = (Aeroporto*)malloc(sizeof(Aeroporto));
    if (!novo) return NULL;

    strncpy(novo->codigo, codigo, 3);
    novo->codigo[3] = '\0';
    strncpy(novo->nome_cidade, cidade, 99);
    novo->nome_cidade[99] = '\0';

    return novo;
}
