#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// --- DEFINIÇÕES GLOBAIS E ESTRUTURAS ---

// Define a capacidade máxima da Fila de peças e da Pilha de reserva.
#define MAX_PECAS 10
#define MAX_RESERVA 3

// Estrutura que representa uma única peça do jogo.
typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', etc.
    int id;    // Identificador único sequencial.
} Peca;

// Estrutura da Fila Circular que armazena as próximas peças.
typedef struct {
    Peca pecas[MAX_PECAS];
    int inicio, fim, total;
} Fila;

// Estrutura da Pilha que armazena as peças reservadas.
typedef struct {
    Peca pecas[MAX_RESERVA];
    int topo;
} Pilha;


// --- FUNÇÕES DA FILA ---

// Configura os valores iniciais de uma Fila.
void initFila(Fila* f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

// Gera uma nova peça com um tipo aleatório e um ID único.
Peca gerarPeca() {
    static int id_counter = 0; // 'static' faz o contador preservar seu valor entre chamadas.
    Peca novaPeca;
    char tiposDePecas[] = "IOTLSZJ";
    novaPeca.nome = tiposDePecas[rand() % 7];
    novaPeca.id = id_counter++;
    return novaPeca;
}

// Adiciona uma peça ao final da Fila (enqueue).
void inserirPeca(Fila* f, Peca peca) {
    if (f->total == MAX_PECAS) {
        printf(">> Fila Cheia! Nao e possivel inserir mais pecas.\n");
        return;
    }
    f->pecas[f->fim] = peca;
    f->fim = (f->fim + 1) % MAX_PECAS; // Lógica circular para o índice 'fim'.
    f->total++;
}

// Remove e retorna a peça do início da Fila (dequeue).
Peca jogarPeca(Fila* f) {
    Peca pecaJogada;
    if (f->total == 0) {
        printf(">> Fila Vazia! Nao ha pecas para jogar.\n");
        pecaJogada.nome = ' ';
        pecaJogada.id = -1; // Retorna uma peça "inválida" para indicar erro.
        return pecaJogada;
    }
    pecaJogada = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_PECAS; // Lógica circular para o índice 'inicio'.
    f->total--;
    printf(">> Peca jogada: [%c %d]\n", pecaJogada.nome, pecaJogada.id);
    return pecaJogada;
}


// --- FUNÇÕES DA PILHA ---

// Configura os valores iniciais de uma Pilha.
void initPilha(Pilha* p) {
    p->topo = -1; // Topo -1 indica que a pilha está vazia.
}

// Verifica se a Pilha atingiu sua capacidade máxima.
int pilhaCheia(Pilha* p) {
    return (p->topo == MAX_RESERVA - 1);
}

// Verifica se a Pilha não contém nenhum elemento.
int pilhaVazia(Pilha* p) {
    return (p->topo == -1);
}

// Adiciona uma peça no topo da Pilha (push).
void push(Pilha* p, Peca peca) {
    if (pilhaCheia(p)) {
        printf(">> Pilha de reserva cheia!\n");
        return;
    }
    p->topo++;
    p->pecas[p->topo] = peca;
}

// Remove e retorna a peça do topo da Pilha (pop).
Peca pop(Pilha* p) {
    Peca pecaRemovida;
    if (pilhaVazia(p)) {
        printf(">> Pilha de reserva vazia!\n");
        pecaRemovida.nome = ' ';
        pecaRemovida.id = -1; // Retorna uma peça "inválida" para indicar erro.
        return pecaRemovida;
    }
    pecaRemovida = p->pecas[p->topo];
    p->topo--;
    return pecaRemovida;
}

// Troca a peca na pilha com a peca na fila.
void trocarPeca(Pilha* p, Fila* f) {
    printf(">> Trocando a peca na pilha com a peca na fila...\n");
    if(pilhaVazia(p) || f->total == 0) {
        printf(">> Pilha vazia ou fila vazia!\n");
        return;
        
    
} 
    // Troca a peça na pilha com a peça na fila.
    Peca pecaPilha = pop(p);
    Peca pecaFila = jogarPeca(f);
    // Insere a peça da fila na pilha.
    push(p, pecaFila);
    // Insere a peça da pilha na fila.
    inserirPeca(f, pecaPilha);
    

    printf(">> Peca trocada com sucesso!\n");
}

// Trocar Multiplas Peças
void trocarMultiplasPecas(Pilha* p, Fila* f) {
    printf(">> Trocando as 3 primeiras pecas da fila com as 3 primeiras pecas da pilha...\n");

    if(p->topo < 2 || f->total < 3) {
        printf(">> A pilha nao possui 3 pecas ou a fila nao possui 3 pecas!\n");
        return;
    }
    // Cria vetor para armazenar as peças da pilha.
    Peca tempFila[3];
    Peca tempPilha[3];
    // Remove as peças da pilha e da fila e armazena em vetores temporarios.
    for (int i = 0; i < 3; i++) {
        tempFila[i] = jogarPeca(f);
        tempPilha[i] = pop(p);
    
    }

    // Insere as peças da pilha e da fila nos respectivos vetores.
    for (int i = 0; i < 3; i++) {
        push(p, tempFila[i]);
        inserirPeca(f, tempPilha[i]);
    }
    printf(">> Pecas trocadas com sucesso!\n");
}


// --- FUNÇÕES DE INTERFACE E EXIBIÇÃO ---

// Exibe o conteúdo da Fila.
void mostrarFila(Fila* f) {
    printf("Fila de pecas: ");
    if (f->total == 0) printf("[VAZIA]");
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX_PECAS) {
        printf("[%c %d] ", f->pecas[idx].nome, f->pecas[idx].id);
    }
    printf("\n");
}

// Exibe o conteúdo da Pilha.
void mostrarPilha(Pilha* p) {
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) printf("[VAZIA]");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
    }
    printf("\n");
}

// Função de tela de boas-vindas.
void telaDeBoasVindas() {
    system("cls || clear");
    printf("+----------------------------------------------------+\n");
    printf("|                                                    |\n");
    printf("|           BEM-VINDO AO TETRIS STACK!               |\n");
    printf("|                                                    |\n");
    printf("+----------------------------------------------------+\n");
    printf("\nPressione Enter para comecar...");
    getchar();
}

// Agrupa a exibição da Fila e da Pilha.
void exibirEstadoAtual(Fila* f, Pilha* p) {
    system("cls || clear"); // Limpa a tela para uma nova rodada.
    printf("\n--- ESTADO ATUAL ---\n");
    mostrarFila(f);
    mostrarPilha(p);
    printf("--------------------\n");
}

// Exibe o menu de ações do jogador.
void menu() {
    printf("\nOpcoes de Acao:\n");
    printf("1. Jogar peca\n");
    printf("2. Reservar peca\n");
    printf("3. Usar peca reservada\n");
    printf("4. Trocar peca atual (Fila <-> Pilha)\n");
    printf("5. Trocar 3 pecas da fila com 3 pecas da pilha\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
}

// Limpa caracteres extras do buffer de entrada do teclado.
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// --- FUNÇÃO PRINCIPAL ---
int main() {
    // Declaração das estruturas principais do jogo.
    Fila filaDePecas;
    Pilha pilhaDeReserva;
    int escolha;

    telaDeBoasVindas();

    // Inicializa o gerador de números aleatórios.
    srand(time(NULL));

    // Prepara a Fila e a Pilha para o início do jogo.
    initFila(&filaDePecas);
    initPilha(&pilhaDeReserva);

    // Popula a fila com as 5 peças iniciais, conforme requisito.
    printf("Iniciando o jogo com 5 pecas...\n");
    for (int i = 0; i < 5; i++) {
        inserirPeca(&filaDePecas, gerarPeca());
    }

    // Loop principal do jogo.
    while (1) {
        exibirEstadoAtual(&filaDePecas, &pilhaDeReserva);
        menu();
        scanf("%d", &escolha);
        limpar_buffer();

        // Estrutura de decisão para cada ação do jogador.
        switch (escolha) {
            case 1: { // Jogar Peça
                jogarPeca(&filaDePecas);
                inserirPeca(&filaDePecas, gerarPeca()); // Regra: reabastece a fila.
                break;
            }
            case 2: { // Reservar Peça
                if (!pilhaCheia(&pilhaDeReserva)) {
                    Peca pecaMovida = jogarPeca(&filaDePecas);
                    if (pecaMovida.id != -1) { // Verifica se a peça é válida
                        push(&pilhaDeReserva, pecaMovida);
                        printf(">> Peca [%c %d] movida para a reserva.\n", pecaMovida.nome, pecaMovida.id);
                        inserirPeca(&filaDePecas, gerarPeca()); // Reabastece a fila.
                    }
                } else {
                    printf(">> A pilha de reserva esta cheia! Nao e possivel reservar.\n");
                }
                break;
            }
            case 3: { // Usar Peça Reservada
                Peca pecaUsada = pop(&pilhaDeReserva);
                if (pecaUsada.id != -1) { // Verifica se a peça é válida
                    printf(">> Peca reservada [%c %d] foi usada.\n", pecaUsada.nome, pecaUsada.id);
                    inserirPeca(&filaDePecas, gerarPeca()); // Reabastece a fila.
                }
                break;
            }
            case 4: { // Trocar Peça
                trocarPeca(&pilhaDeReserva, &filaDePecas);
                break;
            }
            case 5: { // Trocar 3 peças da fila com 3 peças da pilha
                trocarMultiplasPecas(&pilhaDeReserva, &filaDePecas);
                break;
            }
            case 0: // Sair do jogo
                printf("Fim de jogo!\n");
                return 0;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
        printf("\nPressione Enter para continuar...");
        getchar();
    }

    return 0;
}