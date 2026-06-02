#include "vetor_dinamico.h"

VetorDinamico* criar_vetor() {
    VetorDinamico *v = (VetorDinamico*)malloc(sizeof(VetorDinamico));
    if (!v) return NULL;
    v->capacity = 4;
    v->size = 0;
    v->items = (void**)malloc(v->capacity * sizeof(void*));
    if (!v->items) {
        free(v);
        return NULL;
    }
    return v;
}

bool inserir_vetor(VetorDinamico *v, void *item) {
    if (v->size == v->capacity) {
        size_t new_capacity = v->capacity * 2;
        void **new_items = (void**)realloc(v->items, new_capacity * sizeof(void*));
        if (!new_items) return false;
        v->items = new_items;
        v->capacity = new_capacity;
    }
    v->items[v->size++] = item;
    return true;
}

void* obter_vetor(VetorDinamico *v, size_t index) {
    if (index >= v->size) return NULL;
    return v->items[index];
}

bool remover_vetor(VetorDinamico *v, size_t index) {
    if (index >= v->size) return false;

    for (size_t i = index; i < v->size - 1; i++) {
        v->items[i] = v->items[i + 1];
    }
    v->size--;
    return true;
}

void destruir_vetor(VetorDinamico *v) {
    if (!v) return;
    free(v->items);
    free(v);
}
