#include "malha.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    MalhaAerea *malha = criar_malha_aerea();
    inicializar_malha_padrao(malha);
    int opcao;
    char codigo1[10], codigo2[10], numero_voo[20], cidade[100];

    do {
        printf("\n--- SISTEMA DE GESTAO DE MALHA AEREA ---\n");
        printf("1. Cadastrar Aeroporto\n");
        printf("2. Cadastrar Voo\n");
        printf("3. Remover Voo por Numero\n");
        printf("4. Listar Voos de Origem\n");
        printf("5. Buscar Trajetos entre Aeroportos\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        switch (opcao) {
            case 1:
                printf("Codigo do Aeroporto (ex: GRU): ");
                scanf("%3s", codigo1);
                limpar_buffer();
                printf("Nome da Cidade: ");
                fgets(cidade, 100, stdin);
                cidade[strcspn(cidade, "\n")] = 0;
                cadastrar_aeroporto(malha, codigo1, cidade);
                break;
            case 2:
                printf("Codigo Origem: ");
                scanf("%3s", codigo1);
                printf("Codigo Destino: ");
                scanf("%3s", codigo2);
                printf("Numero do Voo (ex: G3-101, AD4050, Voo-XYZ): ");
                scanf("%19s", numero_voo);
                limpar_buffer();
                cadastrar_voo(malha, codigo1, codigo2, numero_voo);
                break;
            case 3:
                printf("Numero do Voo para remover: ");
                scanf("%19s", numero_voo);
                limpar_buffer();
                remover_voo_por_numero(malha, numero_voo);
                break;
            case 4:
                printf("Codigo do Aeroporto de Origem: ");
                scanf("%3s", codigo1);
                limpar_buffer();
                listar_voos_origem(malha, codigo1);
                break;
            case 5:
                printf("Codigo Aeroporto Inicio: ");
                scanf("%3s", codigo1);
                printf("Codigo Aeroporto Fim: ");
                scanf("%3s", codigo2);
                limpar_buffer();
                buscar_trajetos(malha, codigo1, codigo2);
                break;
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    destruir_malha_aerea(malha);
    return 0;
}
