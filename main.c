#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura para um território
struct territorio {
    char nome[50];
    char cor[10];
    int tropas;
};

// Função para limpar o buffer de entrada
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para exibir o mapa atualizado
void exibir_mapa(struct territorio* territorios, int num_territorios) {
    printf("\n================== MAPA DO MUNDO ==================\n");
    for (int i = 0; i < num_territorios; i++) {
        printf("%d. %-20s (Exercito: %-10s, Tropas: %d)\n",
               i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
    printf("====================================================\n");
}

// Função que verifica se há um vencedor
int verificar_vitoria(struct territorio* territorios, int num_territorios) {
    char* cor_referencia = territorios[0].cor;
    for (int i = 1; i < num_territorios; i++) {
        if (strcmp(territorios[i].cor, cor_referencia) != 0) {
            return 0; // Jogo continua
        }
    }
    printf("\n\n!!! FIM DE JOGO !!!\nO Exercito %s dominou o mundo!\n", cor_referencia);
    return 1; // Temos um vencedor
}

// Função que simula o ataque
void atacar(struct territorio* atacante, struct territorio* defensor) {
    printf("--------------------------------------------------------\n");
    printf(" Batalha: %s (%d tropas) ataca %s (%d tropas)\n", atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);

    if (atacante->tropas < 2) {
        printf("Ataque falhou: %s precisa de mais de 1 tropa para atacar.\n\n", atacante->nome);
        return;
    }

    int dado_ataque = (rand() % 6) + 1;
    int dado_defesa = (rand() % 6) + 1;
    printf("... rolando dados ...\n");
    printf(">> Ataque rolou: %d\n", dado_ataque);
    printf(">> Defesa rolou: %d\n", dado_defesa);

    if (dado_ataque > dado_defesa) {
        printf("VITORIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
        defensor->tropas--;
    } else {
        printf("VITORIA DA DEFESA! O atacante perdeu 1 tropa.\n");
        atacante->tropas--;
    }
    printf("Resultado: %s agora tem %d tropas | %s agora tem %d tropas.\n", atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);

    if (defensor->tropas <= 0) {
        printf("\nCONQUISTA! %s dominou %s!\n", atacante->nome, defensor->nome);
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = 1;
        atacante->tropas--;
        printf("%s moveu 1 tropa para %s.\n", atacante->nome, defensor->nome);
    }
    printf("--------------------------------------------------------\n");
}

// --- FUNÇÃO PRINCIPAL ---
int main() {
    struct territorio* territorios;
    int num_Territorios;

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Setup inicial do jogo
    printf("Bem-vindo ao WAR de Console!\n");
    printf("Digite o numero total de territorios para o jogo: ");
    scanf("%d", &num_Territorios);
    limpar_buffer();

    // Aloca memória para o vetor de territórios
    territorios = (struct territorio*) calloc(num_Territorios, sizeof(struct territorio));
    if (territorios == NULL) {
        printf("Erro fatal: Falha ao alocar memoria!\n");
        return 1;
    }

    // Cadastro de territórios
    printf("\n--- FASE DE CADASTRO DE TERRITORIOS ---\n");
    for (int i = 0; i < num_Territorios; i++) {
        printf("\n--- Cadastrando Territorio %d ---\n", i + 1);
        printf("Nome do Territorio: ");
        fgets(territorios[i].nome, 50, stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Cor do Exercito: ");
        fgets(territorios[i].cor, 10, stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Numero de tropas inicial: ");
        scanf("%d", &territorios[i].tropas);
        limpar_buffer();
    }

    // Loop principal da fase de ataque
    int id_atacante, id_defensor;
    while (1) {
        exibir_mapa(territorios, num_Territorios);

        printf("\n--- FASE DE ATAQUE ---\n");
        printf("Escolha o territorio atacante (1 a %d, ou 0 para encerrar o turno): ", num_Territorios);
        scanf("%d", &id_atacante);

        if (id_atacante == 0) {
            printf("Encerrando a fase de ataque...\n");
            break;
        }

        printf("Escolha o territorio defensor (1 a %d): ", num_Territorios);
        scanf("%d", &id_defensor);
        limpar_buffer();

        if (id_atacante < 1 || id_atacante > num_Territorios || id_defensor < 1 || id_defensor > num_Territorios || id_atacante == id_defensor) {
            printf("\nOpcao invalida! Tente novamente.\n");
            continue;
        }

        int indice_atacante = id_atacante - 1;
        int indice_defensor = id_defensor - 1;
        
        if (strcmp(territorios[indice_atacante].cor, territorios[indice_defensor].cor) == 0) {
            printf("\nAtaque invalido! Voce nao pode atacar um territorio que ja e seu.\n");
            continue;
        }

        atacar(&territorios[indice_atacante], &territorios[indice_defensor]);

        if (verificar_vitoria(territorios, num_Territorios)) {
            break;
        }

        printf("\nPressione Enter para continuar...\n");
        getchar();
    }

    // Libera a memória alocada
    free(territorios);

    printf("\nJogo finalizado.\n");
    return 0;
}