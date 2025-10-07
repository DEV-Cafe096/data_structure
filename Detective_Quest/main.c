#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar uma sala da mansão (um nó da árvore).
typedef struct Sala {
    char value[50];
    struct Sala* left;  // Ponteiro para a sala à esquerda
    struct Sala* right; // Ponteiro para a sala à direita
} Sala;

// Aloca memória e cria uma nova sala com o nome especificado.
Sala* createRoom(char* value) {
    Sala* newSala = malloc(sizeof(Sala));
    if (newSala == NULL) {
        printf("Erro ao alocar memoria\n");
        exit(1);
    }
    strcpy(newSala->value, value);
    newSala->left = NULL;
    newSala->right = NULL;
    return newSala;
}

// Libera recursivamente toda a memória alocada para a árvore.
void freeTree(Sala* root) {
    if (root != NULL) {
        freeTree(root->left);  // Libera a sub-árvore esquerda
        freeTree(root->right); // Libera a sub-árvore direita
        free(root);            // Libera o nó atual
    }
}

// --- FUNÇÕES DE INTERFACE ---

// Exibe o menu principal do jogo.
// Versão com caracteres simples, garantida para funcionar
void exibirMenuPrincipal() {
    printf("+------------------------------------+\n");
    printf("|          DETECTIVE QUEST           |\n");
    printf("+------------------------------------+\n");
    printf("| 1. Iniciar Exploracao              |\n");
    printf("| 0. Sair do Jogo                    |\n");
    printf("+------------------------------------+\n");
    printf(" Escolha uma opcao: ");
}
// Limpa caracteres extras do buffer de entrada do teclado.
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// --- FUNÇÃO PRINCIPAL ---
int main() {
    int escolha; // Variável para a escolha do menu principal

    // Loop do menu principal. Só termina quando o jogador escolhe sair (0).
    while (1) {
        system("cls || clear");
        exibirMenuPrincipal();
        
        // Lê a escolha do jogador para o menu principal
        scanf("%d", &escolha);
        limpar_buffer(); // Limpa o buffer após ler o número

        switch (escolha) {
            case 1: { // Caso o jogador escolha "Iniciar Exploração"
                
                // --- A LÓGICA DO JOGO INICIA AQUI ---

                // 1. Montagem do Mapa da Mansão
                printf("\nConstruindo o mapa da mansao...\n");
                Sala* raiz = createRoom("Hall de Entrada");
                raiz->left = createRoom("Biblioteca");
                raiz->left->left = createRoom("Estudio");
                raiz->left->left->right = createRoom("Passagem Secreta");
                raiz->right = createRoom("Sala de Jantar");
                raiz->right->left = createRoom("Cozinha");
                
                // 2. Início da Exploração
                Sala* salaAtual = raiz;
                char escolhaCaminho;
                printf("\nBem-vindo ao Detective Quest!\n");
                printf("Voce esta na sala: %s\n", salaAtual->value);

                // Loop de exploração: continua enquanto a sala atual não for uma "folha"
                while (salaAtual->left != NULL || salaAtual->right != NULL) {
                    printf("\nCaminhos disponiveis:\n");
                    if (salaAtual->left) printf(" (e)squerda -> %s\n", salaAtual->left->value);
                    if (salaAtual->right) printf(" (d)ireita -> %s\n", salaAtual->right->value);
                    printf("Para onde voce quer ir? ");
                    
                    scanf(" %c", &escolhaCaminho); // Espaço antes de %c para ignorar quebras de linha
                    limpar_buffer();

                    Sala* proximaSala = NULL;
                    if (escolhaCaminho == 'e' || escolhaCaminho == 'E') {
                        proximaSala = salaAtual->left;
                    } else if (escolhaCaminho == 'd' || escolhaCaminho == 'D') {
                        proximaSala = salaAtual->right;
                    }

                    if (proximaSala != NULL) {
                        salaAtual = proximaSala;
                        printf("\n>> Voce entrou em: %s\n", salaAtual->value);
                    } else {
                        printf("\n>> Nao ha um caminho nessa direcao! Tente novamente.\n");
                    }
                }

                // 3. Fim da exploração e Limpeza da Memória
                printf("\nVoce chegou ao fim do caminho. Nao ha mais saidas.\n");
                printf("Sala Final: %s\n", salaAtual->value);
                freeTree(raiz); // Libera a memória alocada para a árvore
                
                printf("\n--- Fim da exploracao ---\n");
                printf("Pressione Enter para voltar ao menu principal...");
                getchar(); // Pausa para o jogador ler a mensagem
                break; // Volta para o loop do menu principal
            }
            case 0: { // Caso o jogador escolha "Sair"
                printf("\nObrigado por jogar Detective Quest!\n");
                return 0; // Encerra o programa
            }
            default: { // Caso a opção seja inválida
                printf("\nOpcao invalida! Pressione Enter para tentar novamente.");
                getchar(); // Pausa
                break;
            }
        } // Fim do switch
    } // Fim do while

    return 0;
}