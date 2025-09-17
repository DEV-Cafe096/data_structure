#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura que define o "molde" de um território, agrupando seus dados.
struct territorio {
    char nome[50];
    char cor[10];
    int tropas;
};

// Define uma constante de compilação para o número total de missões.
#define TOTAL_MISSOES 5

// Vetor global (acessível por todo o código) que armazena as missões do jogo.
const char* missoes[TOTAL_MISSOES] = {
    "Conquistar o territorio America",
    "Eliminar o exercito de cor Verde",
    "Acumular 8 tropas em um unico territorio",
    "Conquistar 2 territorios em uma unica rodada",
    "Manter o controle de 3 territorios"
};

// Sorteia uma missão do vetor e a copia para a variável de destino do jogador.
void atribuir_missao(char* destino, const char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    // Usa strcpy para copiar a string sorteada para o ponteiro 'destino'.
    strcpy(destino, missoes[indice]);
}

// Limpa o buffer de entrada para evitar erros de leitura com scanf e fgets.
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Percorre o vetor de territórios e exibe o estado atual do mapa.
void exibir_mapa(struct territorio* territorios, int num_territorios) {
    printf("\n================== MAPA DO MUNDO ==================\n");
    for (int i = 0; i < num_territorios; i++) {
        // Usa (i + 1) para uma exibição mais amigável ao usuário (1, 2, 3...).
        printf("%d. %-20s (Exercito: %-10s, Tropas: %d)\n",
               i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
    printf("====================================================\n");
}

// Verifica a condição de vitória por dominação total (um exército controla tudo).
int verificar_vitoria(struct territorio* territorios, int num_territorios) {
    char* cor_referencia = territorios[0].cor;
    for (int i = 1; i < num_territorios; i++) {
        // Se encontrar qualquer território com uma cor diferente, o jogo continua.
        if (strcmp(territorios[i].cor, cor_referencia) != 0) {
            return 0; // Retorna 0 (falso)
        }
    }
    printf("\n\n!!! FIM DE JOGO !!!\nO Exercito %s dominou o mundo!\n", cor_referencia);
    return 1; // Retorna 1 (verdadeiro)
}

// Simula uma rodada de combate entre dois territórios.
void atacar(struct territorio* atacante, struct territorio* defensor) {
    printf("--------------------------------------------------------\n");
    printf(" Batalha: %s (%d tropas) ataca %s (%d tropas)\n", atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);

    // Validação: um território precisa ter mais de 1 tropa para atacar.
    if (atacante->tropas < 2) {
        printf("Ataque falhou: %s precisa de mais de 1 tropa para atacar.\n\n", atacante->nome);
        return; // Encerra a função prematuramente.
    }

    // Usa rand() para simular a rolagem de dados de 6 lados.
    int dado_ataque = (rand() % 6) + 1;
    int dado_defesa = (rand() % 6) + 1;
    printf("... rolando dados ...\n");
    printf(">> Ataque rolou: %d\n", dado_ataque);
    printf(">> Defesa rolou: %d\n", dado_defesa);

    // Lógica do combate: o atacante vence se tirar um número maior.
    if (dado_ataque > dado_defesa) {
        printf("VITORIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
        defensor->tropas--;
    } else {
        printf("VITORIA DA DEFESA! O atacante perdeu 1 tropa.\n");
        atacante->tropas--;
    }
    printf("Resultado: %s agora tem %d tropas | %s agora tem %d tropas.\n", atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);

    // Lógica de conquista: se o defensor ficar sem tropas.
    if (defensor->tropas <= 0) {
        printf("\nCONQUISTA! %s dominou %s!\n", atacante->nome, defensor->nome);
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = 1;
        atacante->tropas--;
        printf("%s moveu 1 tropa para %s.\n", atacante->nome, defensor->nome);
    }
    printf("--------------------------------------------------------\n");
}

// Verifica se a missão secreta foi cumprida.
int verificarMissao(const char* missao_jogador, const char* cor_do_jogador, struct territorio* territorios, int num_territorios) {
    // Lógica para a missão "Conquistar o territorio America"
    if (strcmp(missao_jogador, "Conquistar o territorio America") == 0) {
        for (int i = 0; i < num_territorios; i++) {
            // Se encontrarmos o território "America"...
            if (strcmp(territorios[i].nome, "America") == 0) {
                // ...verificamos se a cor dele é a mesma do jogador.
                if (strcmp(territorios[i].cor, cor_do_jogador) == 0) {
                    return 0; // Missão CUMPRIDA!
                } else {
                    return 1; // America existe, mas não é do jogador. Missão falhou por agora.
                }
            }
        }
        // Se o loop terminar sem encontrar "America" no mapa, não é possível conquistá-lo.
        return 0; 
    }

    // Adicione a lógica para outras missões com 'else if' aqui.

    return 0; // Nenhuma missão conhecida foi cumprida
}

// --- FUNÇÃO PRINCIPAL ---
int main() {
    struct territorio* territorios;
    int num_Territorios;
    char* missao_jogador;

    // Inicializa o gerador de números aleatórios para garantir dados diferentes a cada jogo.
    srand(time(NULL));

    // --- SETUP INICIAL ---
    printf("Bem-vindo ao WAR de Console!\n");
    printf("Digite o numero total de territorios para o jogo: ");
    scanf("%d", &num_Territorios);
    limpar_buffer();

    // Alocação dinâmica de memória para o vetor de territórios.
    territorios = (struct territorio*)calloc(num_Territorios, sizeof(struct territorio));
    if (territorios == NULL) {
        printf("Erro fatal: Falha ao alocar memoria!\n");
        return 1;
    }
    // Alocação dinâmica de memória para a string da missão.
    missao_jogador = (char*)malloc(100 * sizeof(char));
    if (missao_jogador == NULL) {
        printf("Erro fatal: Falha ao alocar memoria!\n");
        free(territorios);
        return 1;
    }
    
    // Atribui e exibe a missão do jogador.
    atribuir_missao(missao_jogador, missoes, TOTAL_MISSOES);
    printf("\n================== MISSAO SECRETA ==================\n");
    printf("Seu objetivo: \"%s\"\n", missao_jogador);
    printf("====================================================\n");

    // --- CADASTRO DE TERRITÓRIOS ---
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

    // --- LOOP PRINCIPAL DO JOGO ---
    int escolha;
    while (1) {
        system("cls || clear"); // Limpa a tela do console.
        exibir_mapa(territorios, num_Territorios);

        printf("\n--- Sua Missao ---\n");
        printf("\"%s\"\n", missao_jogador);

        // Menu de Ações para o jogador.
        printf("\n--- Menu ---\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar se cumpriu a missao\n");
        printf("0 - Sair do Jogo\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);
        limpar_buffer();

        // Lógica que trata a escolha do jogador.
        if (escolha == 1) {
            int id_atacante, id_defensor;
            printf("\n--- FASE DE ATAQUE ---\n");
            printf("\nEscolha o territorio atacante (1 a %d ou 0 para voltar): \n", num_Territorios);
            scanf("%d", &id_atacante);

            if (id_atacante == 0) {
                continue; // Volta para o início do loop (menu).
            }

            printf("Escolha o territorio defensor (1 a %d): ", num_Territorios);
            scanf("%d", &id_defensor);
            limpar_buffer();

            if (id_atacante < 1 || id_atacante > num_Territorios || id_defensor < 1 || id_defensor > num_Territorios || id_atacante == id_defensor) {
                printf("\nOpcao invalida! Tente novamente.\n");
            } else {
                int indice_atacante = id_atacante - 1;
                int indice_defensor = id_defensor - 1;
                
                if (strcmp(territorios[indice_atacante].cor, territorios[indice_defensor].cor) == 0) {
                    printf("\nAtaque invalido! Voce nao pode atacar um territorio que ja e seu.\n");
                } else {
                    atacar(&territorios[indice_atacante], &territorios[indice_defensor]);
                }
            }
        } else if (escolha == 2) {
            // Verifica a missão, assumindo que a cor do jogador é a do primeiro território cadastrado.
            int missao_cumprida = verificarMissao(missao_jogador, territorios[0].cor, territorios, num_Territorios);
            if (missao_cumprida) {
                printf("\nPARABENS! Voce cumpriu sua missao e venceu o jogo!\n");
                break; // Encerra o jogo.
            } else {
                printf("\nAVISO: Voce ainda nao cumpriu sua missao.\n");
            }
        } else if (escolha == 0) {
            printf("Saindo do jogo...\n");
            break; // Encerra o jogo.
        } else {
            printf("\nOpcao invalida! Pressione Enter para tentar novamente.\n");
        }

        // Ao final de cada ação, verifica se houve vitória por dominação total.
        if (verificar_vitoria(territorios, num_Territorios)) {
            break; 
        }
        
        printf("\nPressione Enter para continuar...\n");
        getchar();
    }

    // --- FINALIZAÇÃO ---
    // Libera toda a memória que foi alocada dinamicamente para evitar vazamentos.
    printf("\nLiberando memoria...\n");
    free(territorios);
    free(missao_jogador);

    printf("\nJogo finalizado.\n");
    return 0;
}