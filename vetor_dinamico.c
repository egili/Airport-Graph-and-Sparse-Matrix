#include "vetor_dinamico.h"

// cria o vetor dinâmico com tamanho inicial
VetorDinamico* criar_vetor() {
    VetorDinamico *vetor = (VetorDinamico*)malloc(sizeof(VetorDinamico));
    if (!vetor) return NULL;
    vetor->capacidade = 4;
    vetor->tamanho = 0;
    vetor->itens = (void**)malloc(vetor->capacidade * sizeof(void*));
    if (!vetor->itens) {
        free(vetor);
        return NULL;
    }
    return vetor;
}

// insere um item no vetor, aumenta a capacidade se estiver cheio
bool inserir_vetor(VetorDinamico *vetor, void *item) {
    if (vetor->tamanho == vetor->capacidade) {
        size_t nova_capacidade = vetor->capacidade * 2;
        void **novos_itens = (void**)realloc(vetor->itens, nova_capacidade * sizeof(void*));
        if (!novos_itens) return false;
        vetor->itens = novos_itens;
        vetor->capacidade = nova_capacidade;
    }
    vetor->itens[vetor->tamanho++] = item;
    return true;
}

// pega o item de um índice específico
void* obter_vetor(VetorDinamico *vetor, size_t indice) {
    if (indice >= vetor->tamanho) return NULL;
    return vetor->itens[indice];
}

// remove um item e puxa os outros pra frente
bool remover_vetor(VetorDinamico *vetor, size_t indice) {
    if (indice >= vetor->tamanho) return false;

    for (size_t i = indice; i < vetor->tamanho - 1; i++) {
        vetor->itens[i] = vetor->itens[i + 1];
    }
    vetor->tamanho--;
    return true;
}

// deleta o vetor e libera a memória
void destruir_vetor(VetorDinamico *vetor) {
    if (!vetor) return;
    free(vetor->itens);
    free(vetor);
}
