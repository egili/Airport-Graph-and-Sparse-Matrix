#include "malha.h"

static int buscar_indice_aeroporto(MalhaAerea *malha, const char *codigo) {
    if (!malha || !codigo) return -1;
    VetorDinamico *v = malha->aeroportos;
    for (size_t i = 0; i < v->size; i++) {
        Aeroporto *a = (Aeroporto*)v->items[i];
        if (strcmp(a->codigo, codigo) == 0) return (int)i;
    }
    return -1;
}

static void imprimir_voos_da_linha(Node *root, MalhaAerea *malha) {
    if (!root) return;
    imprimir_voos_da_linha(root->left, malha);

    Voo *v = (Voo*)root->value;
    Aeroporto *dest = (Aeroporto*)obter_vetor(malha->aeroportos, root->col);
    if (v && dest) {
        printf("- Voo %s para %s (%s)\n", v->numero_voo, dest->nome_cidade, dest->codigo);
    }

    imprimir_voos_da_linha(root->right, malha);
}

static bool remover_voo_por_numero_recursivo(Node *root, const char *num, int row, MatrizEsparsa *m) {
    if (!root) return false;
    Voo *v = (Voo*)root->value;
    if (v && strcmp(v->numero_voo, num) == 0) {
        remover_valor(m, row, root->col);
        destruir_voo(v);
        return true;
    }
    if (remover_voo_por_numero_recursivo(root->left, num, row, m)) return true;
    if (remover_voo_por_numero_recursivo(root->right, num, row, m)) return true;
    return false;
}

static void dfs_trajetos(MalhaAerea *malha, int atual, int destino, bool *visitado, Percurso *caminho_atual) {
    visitado[atual] = true;
    adicionar_ao_percurso(caminho_atual, (Aeroporto*)obter_vetor(malha->aeroportos, atual), NULL);

    if (atual == destino) {
        for (int i = 0; i < caminho_atual->tamanho - 1; i++) {
            char cod_u[4], cod_v[4];
            strcpy(cod_u, caminho_atual->aeroportos[i]->codigo);
            strcpy(cod_v, caminho_atual->aeroportos[i+1]->codigo);
            int idx_u = buscar_indice_aeroporto(malha, cod_u);
            int idx_v = buscar_indice_aeroporto(malha, cod_v);
            Voo *voo = (Voo*)obter_valor(malha->matriz, idx_u, idx_v);

            printf("%s --[%s]--> ", cod_u, voo ? voo->numero_voo : "???");
        }
        printf("%s\n", caminho_atual->aeroportos[caminho_atual->tamanho-1]->codigo);
    } else {
        Node *current = malha->matriz->rows[atual];
        void explorar_vizinhos(Node *n) {
            if (!n) return;
            explorar_vizinhos(n->left);
            int vizinho = n->col;
            if (!visitado[vizinho]) {
                dfs_trajetos(malha, vizinho, destino, visitado, caminho_atual);
            }
            explorar_vizinhos(n->right);
        }
        explorar_vizinhos(current);
    }

    visitado[atual] = false;
    if (caminho_atual->tamanho > 0) {
        caminho_atual->tamanho--;
    }
}

MalhaAerea* criar_malha_aerea() {
    MalhaAerea *ma = (MalhaAerea*)malloc(sizeof(MalhaAerea));
    if (!ma) return NULL;
    ma->aeroportos = criar_vetor();
    ma->matriz = criar_matriz_esparsa();
    return ma;
}

void destruir_malha_aerea(MalhaAerea *ma) {
    if (!ma) return;
    VetorDinamico *v = ma->aeroportos;
    for (size_t i = 0; i < v->size; i++) {
        free(v->items[i]);
    }
    destruir_vetor(v);
    destruir_matriz_esparsa(ma->matriz, (void (*)(void*))destruir_voo);
    free(ma);
}

bool cadastrar_aeroporto(MalhaAerea *malha, const char *codigo, const char *cidade) {
    if (!malha || !codigo || !cidade) return false;
    if (buscar_indice_aeroporto(malha, codigo) != -1) {
        printf("Erro: Aeroporto com codigo %s ja cadastrado.\n", codigo);
        return false;
    }
    Aeroporto *novo = criar_aeroporto(codigo, cidade);
    if (!novo) return false;
    if (!inserir_vetor(malha->aeroportos, novo)) {
        free(novo);
        return false;
    }
    expandir_matriz_esparsa(malha->matriz, malha->aeroportos->size);
    printf("Sucesso: Aeroporto %s (%s) cadastrado.\n", codigo, cidade);
    return true;
}

bool cadastrar_voo(MalhaAerea *malha, const char *cod_origem, const char *cod_destino, const char *num_voo, int duracao, double preco) {
    int idx_origem = buscar_indice_aeroporto(malha, cod_origem);
    int idx_destino = buscar_indice_aeroporto(malha, cod_destino);

    if (idx_origem == -1 || idx_destino == -1) {
        printf("Erro: Um ou ambos os aeroportos nao foram encontrados.\n");
        return false;
    }

    Voo *novo_voo = criar_voo(num_voo, duracao, preco);
    if (!novo_voo) return false;

    if (!definir_valor(malha->matriz, idx_origem, idx_destino, novo_voo)) {
        destruir_voo(novo_voo);
        return false;
    }

    printf("Sucesso: Voo %s de %s para %s cadastrado.\n", num_voo, cod_origem, cod_destino);
    return true;
}

bool remover_voo_por_numero(MalhaAerea *malha, const char *num_voo) {
    if (!malha || !num_voo) return false;

    for (int i = 0; i < malha->matriz->rows_count; i++) {
        if (remover_voo_por_numero_recursivo(malha->matriz->rows[i], num_voo, i, malha->matriz)) {
            printf("Sucesso: Voo %s removido.\n", num_voo);
            return true;
        }
    }
    printf("Erro: Voo %s nao encontrado.\n", num_voo);
    return false;
}

void listar_voos_origem(MalhaAerea *malha, const char *codigo_origem) {
    int idx = buscar_indice_aeroporto(malha, codigo_origem);
    if (idx == -1) {
        printf("Erro: Aeroporto de origem %s nao encontrado.\n", codigo_origem);
        return;
    }

    printf("Voos saindo de %s:\n", codigo_origem);
    imprimir_voos_da_linha(malha->matriz->rows[idx], malha);
}

void buscar_trajetos(MalhaAerea *malha, const char *cod_inicio, const char *cod_fim) {
    int start = buscar_indice_aeroporto(malha, cod_inicio);
    int end = buscar_indice_aeroporto(malha, cod_fim);

    if (start == -1 || end == -1) {
        printf("Erro: Um ou ambos os aeroportos nao encontrados.\n");
        return;
    }

    if (start == end) {
        printf("Origem e destino sao o mesmo aeroporto.\n");
        return;
    }

    bool *visitado = (bool*)calloc(malha->matriz->rows_count, sizeof(bool));
    Percurso *caminho = criar_percurso();

    printf("Possiveis trajetos de %s para %s:\n", cod_inicio, cod_fim);
    dfs_trajetos(malha, start, end, visitado, caminho);

    destruir_percurso(caminho);
    free(visitado);
}

void inicializar_malha_padrao(MalhaAerea *malha) {
    printf("--- Inicializando Aeroportos Padrao ---\n");
    cadastrar_aeroporto(malha, "CNF", "Belo Horizonte");
    cadastrar_aeroporto(malha, "BSB", "Brasilia");
    cadastrar_aeroporto(malha, "GIG", "Rio de Janeiro");
    cadastrar_aeroporto(malha, "SSA", "Salvador");
    cadastrar_aeroporto(malha, "GRU", "Sao Paulo");
    printf("---------------------------------------\n");
}
