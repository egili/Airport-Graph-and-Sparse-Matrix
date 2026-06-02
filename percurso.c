#include "percurso.h"

Percurso* criar_percurso() {
    Percurso *p = (Percurso*)malloc(sizeof(Percurso));
    if (!p) return NULL;
    p->capacidade = 10;
    p->tamanho = 0;
    p->aeroportos = (Aeroporto**)malloc(p->capacidade * sizeof(Aeroporto*));
    p->voos = (Voo**)malloc(p->capacidade * sizeof(Voo*));
    return p;
}

bool adicionar_ao_percurso(Percurso *p, Aeroporto *a, Voo *v) {
    if (!p || !a) return false;

    if (p->tamanho >= p->capacidade) {
        int new_cap = p->capacidade * 2;
        Aeroporto **new_a = (Aeroporto**)realloc(p->aeroportos, new_cap * sizeof(Aeroporto*));
        Voo **new_v = (Voo**)realloc(p->voos, new_cap * sizeof(Voo*));
        if (!new_a || !new_v) return false;
        p->aeroportos = new_a;
        p->voos = new_v;
        p->capacidade = new_cap;
    }

    p->aeroportos[p->tamanho] = a;
    p->voos[p->tamanho] = v;  
    p->tamanho++;
    return true;
}

void imprimir_percurso(Percurso *p) {
    if (!p || p->tamanho == 0) {
        printf("Percurso vazio.\n");
        return;
    }

    printf("Percurso: ");
    for (int i = 0; i < p->tamanho; i++) {
        printf("%s", p->aeroportos[i]->codigo);
        if (i < p->tamanho - 1 && p->voos[i]) {
            printf(" --[%s]--> ", p->voos[i]->numero_voo);
        }
    }
    printf("\n");
}

void destruir_percurso(Percurso *p) {
    if (!p) return;
    free(p->aeroportos);
    free(p->voos);
    free(p);
}
