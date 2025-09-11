// ===================================================================
// JOGO DE ESTRATEGIA ESTILO WAR - VERSÃO CONSOLE
// Linguagem: C
// Conceitos aplicados: Structs, Ponteiros, Alocação Dinâmica, Funções.
// ===================================================================

// --- BIBLIOTECAS ---
// Inclui as ferramentas que vamos precisar no nosso código.
#include <stdio.h>      // Para funções de entrada e saída (printf, scanf)
#include <stdlib.h>     // Para alocação de memória (calloc, free) e números aleatórios (rand, srand)
#include <string.h>     // Para manipulação de strings (strcpy, strcmp)
#include <time.h>       // Para inicializar o gerador de números aleatórios (time)


// --- ESTRUTURA DE DADOS ---
// Define o "molde" para cada território, agrupando seus dados em um único tipo.
struct territorio {
    char nome[50];
    char cor[10];
    int tropas;
};


// --- FUNÇÕES AUXILIARES ---
// Funções pequenas que ajudam a manter o código principal (main) mais limpo.

/**
 * @brief Limpa o buffer de entrada do teclado.
 * Essencial para usar depois de um scanf, antes de ler uma string com fgets.
 */
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Exibe todos os territórios na tela.
 * Usar uma função para isso evita repetir o mesmo código várias vezes.
 * @param territorios Ponteiro para o vetor de territórios.
 * @param num_territorios O número total de territórios no vetor.
 */
void exibir_mapa(struct territorio* territorios, int num_territorios) {
    printf("\n================== MAPA DO MUNDO ==================\n");
    for (int i = 0; i < num_territorios; i++) {
        // Imprime o número do território (i + 1), e não o índice (i), para o usuário.
        printf("%d. %-20s (Exercito: %-10s, Tropas: %d)\n",
               i + 1,
               territorios[i].nome,
               territorios[i].cor,
               territorios[i].tropas);
    }
    printf("====================================================\n");
}

/**
 * @brief Verifica se um único exército conquistou todos os territórios.
 * @return 1 se o jogo acabou (temos um vencedor), 0 caso contrário.
 */
int verificar_vitoria(struct territorio* territorios, int num_territorios) {
    // Pega a cor do primeiro território como referência.
    char* cor_referencia = territorios[0].cor;

    // Compara a cor de todos os outros territórios com a cor de referência.
    for (int i = 1; i < num_territorios; i++) {
        // strcmp retorna 0 se as strings são iguais. Se for diferente de 0, as cores não batem.
        if (strcmp(territorios[i].cor, cor_referencia) != 0) {
            return 0; // Encontrou uma cor diferente, então o jogo continua.
        }
    }

    // Se o loop terminou, é porque todas as cores são iguais. Fim de jogo.
    printf("\n\n!!! FIM DE JOGO !!!\n");
    printf("O Exercito %s dominou o mundo!\n", cor_referencia);
    return 1; // Retorna 1 para sinalizar que o jogo acabou.
}

/**
 * @brief Simula uma rodada de ataque entre dois territórios.
 * @param atacante Ponteiro para o território que está atacando.
 * @param defensor Ponteiro para o território que está defendendo.
 */
void atacar(struct territorio* atacante, struct territorio* defensor) {
    printf("--------------------------------------------------------\n");
    printf(" Batalha: %s (%d tropas) ataca %s (%d tropas)\n", atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);

    // Verificação de segurança: um território precisa de mais de 1 tropa para poder atacar.
    if (atacante->tropas < 2) {
        printf("Ataque falhou: %s precisa de mais de 1 tropa para atacar.\n\n", atacante->nome);
        return; // 'return' encerra a função aqui mesmo.
    }

    // Rola um "dado" de 6 lados para cada um.
    int dado_ataque = (rand() % 6) + 1;
    int dado_defesa = (rand() % 6) + 1;
    printf("... rolando dados ...\n");
    printf(">> Ataque rolou: %d\n", dado_ataque);
    printf(">> Defesa rolou: %d\n", dado_defesa);

    // Compara os dados. O ataque só vence se tirar um número estritamente maior.
    if (dado_ataque > dado_defesa) {
        printf("VITORIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
        defensor->tropas--; // Decrementa (tira 1) das tropas do defensor.
    } else {
        printf("VITORIA DA DEFESA! O atacante perdeu 1 tropa.\n");
        atacante->tropas--; // Decrementa as tropas do atacante.
    }
    printf("Resultado: %s agora tem %d tropas | %s agora tem %d tropas.\n", atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);

    // Verifica se o defensor ficou sem tropas, resultando em uma conquista.
    if (defensor->tropas <= 0) {
        printf("\nCONQUISTA! %s dominou %s!\n", atacante->nome, defensor->nome);
        strcpy(defensor->cor, atacante->cor); // Copia a cor do atacante para o defensor.
        defensor->tropas = 1;                 // O território conquistado fica com 1 tropa.
        atacante->tropas--;                   // Essa tropa é movida do exército atacante.
        printf("%s moveu 1 tropa para %s.\n", atacante->nome, defensor->nome);
    }
    printf("--------------------------------------------------------\n");
}


// --- FUNÇÃO PRINCIPAL ---
// O ponto de entrada do nosso programa.
int main() {
    // Declara um ponteiro para 'struct territorio'. Ele vai apontar para o nosso vetor dinâmico.
    struct territorio* territorios;
    int num_Territorios;

    // Inicializa o gerador de números aleatórios usando a hora atual como semente.
    // Isso garante que os dados rolem números diferentes a cada execução do jogo.
    // Deve ser chamado APENAS UMA VEZ.
    srand(time(NULL));

    // --- SETUP INICIAL DO JOGO ---
    printf("Bem-vindo ao WAR de Console!\n");
    printf("Digite o numero total de territorios para o jogo: ");
    scanf("%d", &num_Territorios);
    limpar_buffer(); // Limpa o '\n' que o scanf deixa para trás.

    // Aloca memória dinamicamente para o vetor de territórios.
    // calloc é bom porque já inicializa a memória com zeros.
    territorios = (struct territorio*) calloc(num_Territorios, sizeof(struct territorio));

    // Verificação de segurança OBRIGATÓRIA: checa se a memória foi alocada com sucesso.
    if (territorios == NULL) {
        printf("Erro fatal: Falha ao alocar memoria!\n");
        return 1; // Encerra o programa com um código de erro.
    }

    // Loop para o usuário cadastrar os dados de cada território.
    printf("\n--- FASE DE CADASTRO DE TERRITORIOS ---\n");
    for (int i = 0; i < num_Territorios; i++) {
        printf("\n--- Cadastrando Territorio %d ---\n", i + 1);
        printf("Nome do Territorio: ");
        fgets(territorios[i].nome, 50, stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0'; // Remove o '\n' do fgets

        printf("Cor do Exercito: ");
        fgets(territorios[i].cor, 10, stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Numero de tropas inicial: ");
        scanf("%d", &territorios[i].tropas);
        limpar_buffer();
    }

    // --- LOOP PRINCIPAL DO JOGO (FASE DE ATAQUE) ---
    int id_atacante, id_defensor;
    while (1) { // 'while(1)' cria um loop infinito que só é parado por um 'break'.
        
        exibir_mapa(territorios, num_Territorios);

        printf("\n--- FASE DE ATAQUE ---\n");
        printf("Escolha o territorio atacante (1 a %d, ou 0 para encerrar o turno): ", num_Territorios);
        scanf("%d", &id_atacante);

        // Se o usuário digitar 0, o loop é interrompido.
        if (id_atacante == 0) {
            printf("Encerrando a fase de ataque...\n");
            break;
        }

        printf("Escolha o territorio defensor (1 a %d): ", num_Territorios);
        scanf("%d", &id_defensor);
        limpar_buffer();

        // Validação da entrada do usuário para evitar erros.
        if (id_atacante < 1 || id_atacante > num_Territorios || id_defensor < 1 || id_defensor > num_Territorios) {
            printf("\nOpcao invalida! Escolha numeros de territorios que existem.\n");
            continue; // 'continue' pula para a próxima volta do loop.
        }
        if (id_atacante == id_defensor) {
            printf("\nOpcao invalida! Um territorio nao pode atacar a si mesmo.\n");
            continue;
        }

        // Converte o ID que o usuário vê (1, 2, 3...) para o índice do vetor (0, 1, 2...).
        int indice_atacante = id_atacante - 1;
        int indice_defensor = id_defensor - 1;
        
        // Validação para não atacar um território do mesmo time.
        if (strcmp(territorios[indice_atacante].cor, territorios[indice_defensor].cor) == 0) {
            printf("\nAtaque invalido! Voce nao pode atacar um territorio que ja e seu.\n");
            continue;
        }

        // Se todas as validações passaram, executa o ataque.
        atacar(&territorios[indice_atacante], &territorios[indice_defensor]);

        // Após cada batalha, verifica se alguém já ganhou o jogo.
        if (verificar_vitoria(territorios, num_Territorios)) {
            break; // Se alguém venceu, sai do loop principal.
        }

        printf("\nPressione Enter para continuar...\n");
        getchar(); // Pausa a tela até o usuário pressionar Enter.
    }

    // --- FINALIZAÇÃO DO PROGRAMA ---
    // Libera a memória que foi alocada com 'calloc' no início.
    // Essencial para evitar "vazamento de memória" (memory leak).
    free(territorios);

    printf("\nJogo finalizado.\n");
    return 0; // Retorna 0 para indicar que o programa terminou com sucesso.
}