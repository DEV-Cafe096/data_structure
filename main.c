#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TERRITORIOS 5

// Estrutura para um território
struct territorio {
    char nome[50];
    char cor[10];
    int tropas;
};

// Função para limpar o buffer de entrada.
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    struct territorio territorios[MAX_TERRITORIOS];
    int i;

    
    printf("=========================================================\n");
    printf("Vamos cadastrar os 5 territorios iniciais do nosso mundo.\n");
    printf("=========================================================\n\n");


    // Loop para cadastrar os territórios
    for (i = 0; i < MAX_TERRITORIOS; i++) {
        // --- TEXTO DE CADASTRO ALTERADO ---
        printf("--- Cadastrando Territorio %d ---\n", i + 1);

        // --- PROMPTS DE ENTRADA ALTERADOS ---
        printf("Nome do Territorio: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Cor do Exercito (ex: Azul, Verde): ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Digite o numero de tropas no territorio: ");
        scanf("%d", &territorios[i].tropas);

        limpar_buffer();
        printf("\n"); // Adiciona uma linha em branco para separar os cadastros
    }

    // O código de exibição continua o mesmo
    printf("\n\n--- Territorios Cadastrados com Sucesso! ---\n");
    printf("================================================\n");
    for (i = 0; i < MAX_TERRITORIOS; i++) {
        printf("Territorio: %-20s | Cor: %-10s | Tropas: %d\n",
               territorios[i].nome,
               territorios[i].cor,
               territorios[i].tropas);
    }
    printf("================================================\n\n");

    return 0;
}