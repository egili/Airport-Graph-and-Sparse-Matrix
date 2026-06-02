#include "matriz_esparsa.h"

MatrizEsparsa* criar_matriz_esparsa() {
    MatrizEsparsa *matriz = (MatrizEsparsa*)malloc(sizeof(MatrizEsparsa));
    if (!matriz) return NULL;
    matriz->linhas = NULL;
    matriz->quantidade_linhas = 0;
    matriz->quantidade_colunas = 0;
    return matriz;
}

void expandir_matriz_esparsa(MatrizEsparsa *matriz, int nova_dimensao) {
    if (nova_dimensao <= matriz->quantidade_linhas) return;

    No **novas_linhas = (No**)realloc(matriz->linhas, nova_dimensao * sizeof(No*));
    if (!novas_linhas) return;

    for (int i = matriz->quantidade_linhas; i < nova_dimensao; i++) {
        novas_linhas[i] = NULL;
    }

    matriz->linhas = novas_linhas;
    matriz->quantidade_linhas = nova_dimensao;
    matriz->quantidade_colunas = nova_dimensao;
}

static No* criar_no(int coluna, void *valor) {
    No *no = (No*)malloc(sizeof(No));
    if (!no) return NULL;
    no->coluna = coluna;
    no->valor = valor;
    no->esquerda = no->direita = NULL;
    return no;
}

static No* inserir_na_arvore(No *raiz, int coluna, void *valor) {
    if (!raiz) return criar_no(coluna, valor);

    if (coluna < raiz->coluna) {
        raiz->esquerda = inserir_na_arvore(raiz->esquerda, coluna, valor);
    } else if (coluna > raiz->coluna) {
        raiz->direita = inserir_na_arvore(raiz->direita, coluna, valor);
    } else {
        raiz->valor = valor;
    }
    return raiz;
}

static No* buscar_menor_indice_coluna(No *raiz) {
    No *atual = raiz;
    while (atual && atual->esquerda) {
        atual = atual->esquerda;
    }
    return atual;
}

static No* remover_da_arvore(No *raiz, int coluna) {
    if (!raiz) return NULL;

    if (coluna < raiz->coluna) {
        raiz->esquerda = remover_da_arvore(raiz->esquerda, coluna);
    } else if (coluna > raiz->coluna) {
        raiz->direita = remover_da_arvore(raiz->direita, coluna);
    } else {
        if (!raiz->esquerda) {
            No *temporario = raiz->direita;
            free(raiz);
            return temporario;
        } else if (!raiz->direita) {
            No *temporario = raiz->esquerda;
            free(raiz);
            return temporario;
        }

        No *temporario = buscar_menor_indice_coluna(raiz->direita);
        raiz->coluna = temporario->coluna;
        raiz->valor = temporario->valor;
        raiz->direita = remover_da_arvore(raiz->direita, temporario->coluna);
    }
    return raiz;
}

static void liberar_arvore_voos(No *raiz, void (*liberar_valor)(void*)) {
    if (!raiz) return;
    liberar_arvore_voos(raiz->esquerda, liberar_valor);
    liberar_arvore_voos(raiz->direita, liberar_valor);
    if (liberar_valor) liberar_valor(raiz->valor);
    free(raiz);
}

bool definir_valor_matriz(MatrizEsparsa *matriz, int linha, int coluna, void *valor) {
    if (linha < 0 || coluna < 0) return false;

    int maior_indice = (linha > coluna) ? linha : coluna;
    expandir_matriz_esparsa(matriz, maior_indice + 1);

    matriz->linhas[linha] = inserir_na_arvore(matriz->linhas[linha], coluna, valor);
    return true;
}

void* obter_valor_matriz(MatrizEsparsa *matriz, int linha, int coluna) {
    if (!matriz || linha < 0 || linha >= matriz->quantidade_linhas || coluna < 0 || coluna >= matriz->quantidade_colunas) {
        return NULL;
    }

    No *atual = matriz->linhas[linha];
    while (atual) {
        if (coluna < atual->coluna) atual = atual->esquerda;
        else if (coluna > atual->coluna) atual = atual->direita;
        else return atual->valor;
    }
    return NULL;
}

bool remover_valor_matriz(MatrizEsparsa *matriz, int linha, int coluna) {
    if (!matriz || linha < 0 || linha >= matriz->quantidade_linhas) return false;

    if (obter_valor_matriz(matriz, linha, coluna) == NULL) return false;

    matriz->linhas[linha] = remover_da_arvore(matriz->linhas[linha], coluna);
    return true;
}

void destruir_matriz_esparsa(MatrizEsparsa *matriz, void (*liberar_valor)(void*)) {
    if (!matriz) return;
    for (int i = 0; i < matriz->quantidade_linhas; i++) {
        liberar_arvore_voos(matriz->linhas[i], liberar_valor);
    }
    free(matriz->linhas);
    free(matriz);
}
