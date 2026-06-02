#ifndef MALHA_H
#define MALHA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "aeroporto.h"
#include "vetor_dinamico.h"
#include "matriz_esparsa.h"
#include "voo.h"
#include "percurso.h"

typedef struct {
    VetorDinamico *aeroportos;
    MatrizEsparsa *matriz;
} MalhaAerea;

MalhaAerea* criar_malha_aerea();
void destruir_malha_aerea(MalhaAerea *malha);

bool cadastrar_aeroporto(MalhaAerea *malha, const char *codigo, const char *cidade);

bool cadastrar_voo(MalhaAerea *malha, const char *codigo_origem, const char *codigo_destino, const char *numero_voo, int duracao, double preco);

bool remover_voo_por_numero(MalhaAerea *malha, const char *numero_voo);

void listar_voos_origem(MalhaAerea *malha, const char *codigo_origem);

void buscar_trajetos(MalhaAerea *malha, const char *codigo_inicio, const char *codigo_fim);

void inicializar_malha_padrao(MalhaAerea *malha);

#endif // MALHA_H
