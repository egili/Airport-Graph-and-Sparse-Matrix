#include "voo.h"

// cria o voo com o número informado
Voo* criar_voo(const char *numero) {
    if (!numero) return NULL;

    Voo *v = (Voo*)malloc(sizeof(Voo));
    if (!v) return NULL;

    strncpy(v->numero_voo, numero, 19);
    v->numero_voo[19] = '\0';

    return v;
}

// libera a memória do voo
void destruir_voo(Voo *v) {
    if (v) free(v);
}

// mostra os dados do voo na tela
void imprimir_voo(Voo *v) {
    if (!v) {
        printf("Voo inexistente");
        return;
    }
    printf("Voo %s", v->numero_voo);
}
