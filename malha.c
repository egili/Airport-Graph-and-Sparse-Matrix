#include "malha.h"

static int buscar_indice_aeroporto(MalhaAerea *malha, const char *codigo) {
    if (!malha || !codigo) return -1;
    VetorDinamico *vetor = malha->aeroportos;
    for (size_t i = 0; i < vetor->tamanho; i++) {
        Aeroporto *aeroporto = (Aeroporto*)vetor->itens[i];
        if (strcmp(aeroporto->codigo, codigo) == 0) return (int)i;
    }
    return -1;
}

static void imprimir_voos_da_linha(No *raiz, MalhaAerea *malha) {
    if (!raiz) return;
    imprimir_voos_da_linha(raiz->esquerda, malha);

    Voo *voo = (Voo*)raiz->valor;
    Aeroporto *destino = (Aeroporto*)obter_vetor(malha->aeroportos, raiz->coluna);
    if (voo && destino) {
        printf("- Voo %s para %s (%s)\n", voo->numero_voo, destino->nome_cidade, destino->codigo);
    }

    imprimir_voos_da_linha(raiz->direita, malha);
}

static bool remover_voo_por_numero_recursivo(No *raiz, const char *numero, int linha, MatrizEsparsa *matriz) {
    if (!raiz) return false;
    Voo *voo = (Voo*)raiz->valor;
    if (voo && strcmp(voo->numero_voo, numero) == 0) {
        remover_valor_matriz(matriz, linha, raiz->coluna);
        destruir_voo(voo);
        return true;
    }
    if (remover_voo_por_numero_recursivo(raiz->esquerda, numero, linha, matriz)) return true;
    if (remover_voo_por_numero_recursivo(raiz->direita, numero, linha, matriz)) return true;
    return false;
}

static void buscar_trajetos_profundidade(MalhaAerea *malha, int atual, int destino, bool *visitado, Percurso *caminho_atual, bool *encontrado) {
    visitado[atual] = true;
    adicionar_ao_percurso(caminho_atual, (Aeroporto*)obter_vetor(malha->aeroportos, atual), NULL);

    if (atual == destino) {
        *encontrado = true;
        for (int i = 0; i < caminho_atual->tamanho - 1; i++) {
            char codigo_u[4], codigo_v[4];
            strcpy(codigo_u, caminho_atual->aeroportos[i]->codigo);
            strcpy(codigo_v, caminho_atual->aeroportos[i+1]->codigo);
            int indice_u = buscar_indice_aeroporto(malha, codigo_u);
            int indice_v = buscar_indice_aeroporto(malha, codigo_v);
            Voo *voo = (Voo*)obter_valor_matriz(malha->matriz, indice_u, indice_v);

            printf("%s --[%s]--> ", codigo_u, voo ? voo->numero_voo : "???");
        }
        printf("%s\n", caminho_atual->aeroportos[caminho_atual->tamanho-1]->codigo);
    } else {
        No *atual_no = malha->matriz->linhas[atual];
        void explorar_vizinhos(No *no) {
            if (!no) return;
            explorar_vizinhos(no->esquerda);
            int vizinho = no->coluna;
            if (!visitado[vizinho]) {
                buscar_trajetos_profundidade(malha, vizinho, destino, visitado, caminho_atual, encontrado);
            }
            explorar_vizinhos(no->direita);
        }
        explorar_vizinhos(atual_no);
    }

    visitado[atual] = false;
    if (caminho_atual->tamanho > 0) {
        caminho_atual->tamanho--;
    }
}

MalhaAerea* criar_malha_aerea() {
    MalhaAerea *malha = (MalhaAerea*)malloc(sizeof(MalhaAerea));
    if (!malha) return NULL;
    malha->aeroportos = criar_vetor();
    malha->matriz = criar_matriz_esparsa();
    return malha;
}

void destruir_malha_aerea(MalhaAerea *malha) {
    if (!malha) return;
    VetorDinamico *vetor = malha->aeroportos;
    for (size_t i = 0; i < vetor->tamanho; i++) {
        free(vetor->itens[i]);
    }
    destruir_vetor(vetor);
    destruir_matriz_esparsa(malha->matriz, (void (*)(void*))destruir_voo);
    free(malha);
}

bool cadastrar_aeroporto(MalhaAerea *malha, const char *codigo, const char *cidade) {
    if (!malha || !codigo || !cidade) return false;
    if (buscar_indice_aeroporto(malha, codigo) != -1) {
        printf("Erro: Aeroporto com codigo %s ja cadastrado.\n", codigo);
        return false;
    }
    Aeroporto *novo_aeroporto = criar_aeroporto(codigo, cidade);
    if (!novo_aeroporto) return false;
    if (!inserir_vetor(malha->aeroportos, novo_aeroporto)) {
        free(novo_aeroporto);
        return false;
    }
    expandir_matriz_esparsa(malha->matriz, malha->aeroportos->tamanho);
    printf("Sucesso: Aeroporto %s (%s) cadastrado.\n", codigo, cidade);
    return true;
}

bool cadastrar_voo(MalhaAerea *malha, const char *codigo_origem, const char *codigo_destino, const char *numero_voo) {
    int indice_origem = buscar_indice_aeroporto(malha, codigo_origem);
    int indice_destino = buscar_indice_aeroporto(malha, codigo_destino);

    if (indice_origem == -1 || indice_destino == -1) {
        printf("Erro: Um ou ambos os aeroportos nao foram encontrados.\n");
        return false;
    }

    if (indice_origem == indice_destino) {
        printf("Erro: Origem e destino nao podem ser o mesmo aeroporto.\n");
        return false;
    }

    Voo *novo_voo = criar_voo(numero_voo);
    if (!novo_voo) return false;

    if (!definir_valor_matriz(malha->matriz, indice_origem, indice_destino, novo_voo)) {
        destruir_voo(novo_voo);
        return false;
    }

    printf("Sucesso: Voo %s de %s para %s cadastrado.\n", numero_voo, codigo_origem, codigo_destino);
    return true;
}

bool remover_voo_por_numero(MalhaAerea *malha, const char *numero_voo) {
    if (!malha || !numero_voo) return false;

    for (int i = 0; i < malha->matriz->quantidade_linhas; i++) {
        if (remover_voo_por_numero_recursivo(malha->matriz->linhas[i], numero_voo, i, malha->matriz)) {
            printf("Sucesso: Voo %s removido.\n", numero_voo);
            return true;
        }
    }
    printf("Erro: Voo %s nao encontrado.\n", numero_voo);
    return false;
}

void listar_voos_origem(MalhaAerea *malha, const char *codigo_origem) {
    int indice = buscar_indice_aeroporto(malha, codigo_origem);
    if (indice == -1) {
        printf("Erro: Aeroporto de origem %s nao encontrado.\n", codigo_origem);
        return;
    }

    if (malha->matriz->linhas[indice] == NULL) {
        printf("Nao ha voos cadastrados saindo de %s.\n", codigo_origem);
        return;
    }

    printf("Voos saindo de %s:\n", codigo_origem);
    imprimir_voos_da_linha(malha->matriz->linhas[indice], malha);
}

void buscar_trajetos(MalhaAerea *malha, const char *codigo_inicio, const char *codigo_fim) {
    int inicio = buscar_indice_aeroporto(malha, codigo_inicio);
    int fim = buscar_indice_aeroporto(malha, codigo_fim);

    if (inicio == -1 || fim == -1) {
        printf("Erro: Um ou ambos os aeroportos nao encontrados.\n");
        return;
    }

    if (inicio == fim) {
        printf("Origem e destino sao o mesmo aeroporto.\n");
        return;
    }

    bool *visitado = (bool*)calloc(malha->matriz->quantidade_linhas, sizeof(bool));
    Percurso *percurso = criar_percurso();
    bool encontrado = false;

    printf("Possiveis trajetos de %s para %s:\n", codigo_inicio, codigo_fim);
    buscar_trajetos_profundidade(malha, inicio, fim, visitado, percurso, &encontrado);

    if (!encontrado) {
        printf("Nenhum trajeto encontrado entre %s e %s.\n", codigo_inicio, codigo_fim);
    }

    destruir_percurso(percurso);
    free(visitado);
}

void inicializar_malha_padrao(MalhaAerea *malha) {
    printf("--- Inicializando Dados Padrao ---\n");
    cadastrar_aeroporto(malha, "CNF", "Belo Horizonte");
    cadastrar_aeroporto(malha, "BSB", "Brasilia");
    cadastrar_aeroporto(malha, "GIG", "Rio de Janeiro");
    cadastrar_aeroporto(malha, "SSA", "Salvador");
    cadastrar_aeroporto(malha, "GRU", "Sao Paulo");

    printf("Cadastrando voos de exemplo...\n");
    cadastrar_voo(malha, "GRU", "CNF", "G3-101");
    cadastrar_voo(malha, "CNF", "BSB", "G3-202");
    cadastrar_voo(malha, "BSB", "GIG", "G3-303");
    cadastrar_voo(malha, "GIG", "SSA", "G3-404");
    cadastrar_voo(malha, "SSA", "GRU", "G3-505");
    cadastrar_voo(malha, "GRU", "GIG", "G3-606");
    cadastrar_voo(malha, "CNF", "GRU", "G3-707");
    cadastrar_voo(malha, "BSB", "GRU", "G3-808");
    printf("---------------------------------------\n");
}
