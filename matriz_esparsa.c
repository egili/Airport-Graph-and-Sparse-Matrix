#include "matriz_esparsa.h"

MatrizEsparsa* criar_matriz_esparsa() {
    MatrizEsparsa *m = (MatrizEsparsa*)malloc(sizeof(MatrizEsparsa));
    if (!m) return NULL;
    m->rows = NULL;
    m->rows_count = 0;
    m->cols_count = 0;
    return m;
}

void expandir_matriz_esparsa(MatrizEsparsa *m, int new_dim) {
    if (new_dim <= m->rows_count) return;

    Node **new_rows = (Node**)realloc(m->rows, new_dim * sizeof(Node*));
    if (!new_rows) return;

    for (int i = m->rows_count; i < new_dim; i++) {
        new_rows[i] = NULL;
    }

    m->rows = new_rows;
    m->rows_count = new_dim;
    m->cols_count = new_dim;
}

static Node* criar_node(int col, void *value) {
    Node *n = (Node*)malloc(sizeof(Node));
    if (!n) return NULL;
    n->col = col;
    n->value = value;
    n->left = n->right = NULL;
    return n;
}

static Node* inserir_na_arvore(Node *root, int col, void *value) {
    if (!root) return criar_node(col, value);

    if (col < root->col) {
        root->left = inserir_na_arvore(root->left, col, value);
    } else if (col > root->col) {
        root->right = inserir_na_arvore(root->right, col, value);
    } else {
        root->value = value; 
    }
    return root;
}

static Node* buscar_menor_indice_coluna(Node *root) {
    Node *current = root;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}

static Node* remover_da_arvore(Node *root, int col) {
    if (!root) return NULL;

    if (col < root->col) {
        root->left = remover_da_arvore(root->left, col);
    } else if (col > root->col) {
        root->right = remover_da_arvore(root->right, col);
    } else {
        if (!root->left) {
            Node *temp = root->right;
            free(root);
            return temp;
        } else if (!root->right) {
            Node *temp = root->left;
            free(root);
            return temp;
        }

        Node *temp = buscar_menor_indice_coluna(root->right);
        root->col = temp->col;
        root->value = temp->value;
        root->right = remover_da_arvore(root->right, temp->col);
    }
    return root;
}

static void liberar_arvore_voos(Node *root, void (*free_value)(void*)) {
    if (!root) return;
    liberar_arvore_voos(root->left, free_value);
    liberar_arvore_voos(root->right, free_value);
    if (free_value) free_value(root->value);
    free(root);
}

bool definir_valor(MatrizEsparsa *m, int row, int col, void *value) {
    if (row < 0 || col < 0) return false;

    int max_idx = (row > col) ? row : col;
    expandir_matriz_esparsa(m, max_idx + 1);

    m->rows[row] = inserir_na_arvore(m->rows[row], col, value);
    return true;
}

void* obter_valor(MatrizEsparsa *m, int row, int col) {
    if (!m || row < 0 || row >= m->rows_count || col < 0 || col >= m->cols_count) {
        return NULL;
    }

    Node *current = m->rows[row];
    while (current) {
        if (col < current->col) current = current->left;
        else if (col > current->col) current = current->right;
        else return current->value;
    }
    return NULL;
}

bool remover_valor(MatrizEsparsa *m, int row, int col) {
    if (!m || row < 0 || row >= m->rows_count) return false;

    if (obter_valor(m, row, col) == NULL) return false;

    m->rows[row] = remover_da_arvore(m->rows[row], col);
    return true;
}

void destruir_matriz_esparsa(MatrizEsparsa *m, void (*free_value)(void*)) {
    if (!m) return;
    for (int i = 0; i < m->rows_count; i++) {
        liberar_arvore_voos(m->rows[i], free_value);
    }
    free(m->rows);
    free(m);
}
