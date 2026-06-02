#include "voo.h"

Voo* criar_voo(const char *numero, int duracao, double preco) {
    if (!numero) return NULL;

    Voo *v = (Voo*)malloc(sizeof(Voo));
    if (!v) return NULL;

    strncpy(v->numero_voo, numero, 9);
    v->numero_voo[9] = '\0';
    v->duracao_minutos = duracao;
    v->preco = preco;

    return v;
}

void destruir_voo(Voo *v) {
    if (v) free(v);
}

void imprimir_voo(Voo *v) {
    if (!v) {
        printf("Voo inexistente");
        return;
    }
    printf("Voo %s (%d min, R$ %.2f)", v->numero_voo, v->duracao_minutos, v->preco);
}
