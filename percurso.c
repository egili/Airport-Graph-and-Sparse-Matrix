#include "percurso.h"

Percurso* criar_percurso() {
    Percurso *percurso = (Percurso*)malloc(sizeof(Percurso));
    if (!percurso) return NULL;
    percurso->capacidade = 10;
    percurso->tamanho = 0;
    percurso->aeroportos = (Aeroporto**)malloc(percurso->capacidade * sizeof(Aeroporto*));
    percurso->voos = (Voo**)malloc(percurso->capacidade * sizeof(Voo*));
    return percurso;
}

bool adicionar_ao_percurso(Percurso *percurso, Aeroporto *aeroporto, Voo *voo) {
    if (!percurso || !aeroporto) return false;

    if (percurso->tamanho >= percurso->capacidade) {
        int nova_capacidade = percurso->capacidade * 2;
        Aeroporto **novos_aeroportos = (Aeroporto**)realloc(percurso->aeroportos, nova_capacidade * sizeof(Aeroporto*));
        Voo **novos_voos = (Voo**)realloc(percurso->voos, nova_capacidade * sizeof(Voo*));
        if (!novos_aeroportos || !novos_voos) return false;
        percurso->aeroportos = novos_aeroportos;
        percurso->voos = novos_voos;
        percurso->capacidade = nova_capacidade;
    }

    percurso->aeroportos[percurso->tamanho] = aeroporto;
    percurso->voos[percurso->tamanho] = voo;
    percurso->tamanho++;
    return true;
}

void imprimir_percurso(Percurso *percurso) {
    if (!percurso || percurso->tamanho == 0) {
        printf("Percurso vazio.\n");
        return;
    }

    printf("Percurso: ");
    for (int i = 0; i < percurso->tamanho; i++) {
        printf("%s", percurso->aeroportos[i]->codigo);
        if (i < percurso->tamanho - 1 && percurso->voos[i]) {
            printf(" --[%s]--> ", percurso->voos[i]->numero_voo);
        }
    }
    printf("\n");
}

void destruir_percurso(Percurso *percurso) {
    if (!percurso) return;
    free(percurso->aeroportos);
    free(percurso->voos);
    free(percurso);
}
