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
    char cod1[10], cod2[10], num_voo[20], cidade[100];
    int duracao;
    double preco;

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
                scanf("%3s", cod1);
                limpar_buffer();
                printf("Nome da Cidade: ");
                fgets(cidade, 100, stdin);
                cidade[strcspn(cidade, "\n")] = 0;
                cadastrar_aeroporto(malha, cod1, cidade);
                break;
            case 2:
                printf("Codigo Origem: ");
                scanf("%3s", cod1);
                printf("Codigo Destino: ");
                scanf("%3s", cod2);
                printf("Numero do Voo: ");
                scanf("%19s", num_voo);
                printf("Duracao (min): ");
                scanf("%d", &duracao);
                printf("Preco: ");
                scanf("%lf", &preco);
                limpar_buffer();
                cadastrar_voo(malha, cod1, cod2, num_voo, duracao, preco);
                break;
            case 3:
                printf("Numero do Voo para remover: ");
                scanf("%19s", num_voo);
                limpar_buffer();
                remover_voo_por_numero(malha, num_voo);
                break;
            case 4:
                printf("Codigo do Aeroporto de Origem: ");
                scanf("%3s", cod1);
                limpar_buffer();
                listar_voos_origem(malha, cod1);
                break;
            case 5:
                printf("Codigo Aeroporto Inicio: ");
                scanf("%3s", cod1);
                printf("Codigo Aeroporto Fim: ");
                scanf("%3s", cod2);
                limpar_buffer();
                buscar_trajetos(malha, cod1, cod2);
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
