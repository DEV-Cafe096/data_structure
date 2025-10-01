#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // Adicionado para strcpy

#define MAX_PECAS 10
#define MAX_RESERVA 3

// Estrutura que representa uma única peça
typedef struct
{
    char nome;
    int id;
} Peca;

// Estrutura da Fila Circular de Peças
typedef struct
{
    Peca pecas[MAX_PECAS];
    int inicio;
    int fim;
    int total;
} Fila;

// Estrutura da Pilha de Peças
typedef struct
{
    Peca pecas[MAX_RESERVA];
    int topo;
} Pilha;

// --- FUNÇÕES DA FILA ---

void initFila(Fila *f)
{
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

Peca gerarPeca()
{
    static int id_counter = 0;
    Peca novaPeca;
    char tiposDePecas[] = "IOTLSZJ";
    novaPeca.nome = tiposDePecas[rand() % 7];
    novaPeca.id = id_counter++;
    return novaPeca;
}

void inserirPeca(Fila *f, Peca peca)
{
    if (f->total == MAX_PECAS)
    {
        printf(">> Fila Cheia! Nao e possivel inserir mais pecas.\n");
        return;
    }
    f->pecas[f->fim] = peca;
    f->fim = (f->fim + 1) % MAX_PECAS;
    f->total++;
}

Peca jogarPeca(Fila *f)
{
    Peca pecaJogada;
    if (f->total == 0)
    {
        printf(">> Fila Vazia! Nao ha pecas para jogar.\n");
        pecaJogada.nome = ' ';
        pecaJogada.id = -1;
        return pecaJogada;
    }
    pecaJogada = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_PECAS;
    f->total--;
    printf(">> Peca jogada: [%c %d]\n", pecaJogada.nome, pecaJogada.id);
    return pecaJogada;
}

void mostrarFila(Fila *f)
{
    printf("Fila de pecas: ");
    if (f->total == 0)
        printf("[VAZIA]");
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX_PECAS)
    {
        printf("[%c %d] ", f->pecas[idx].nome, f->pecas[idx].id);
    }
    printf("\n");
}

// --- FUNÇÕES DA PILHA ---

void initPilha(Pilha *p)
{
    p->topo = -1;
}

// CORRIGIDO: Retorno de int
int pilhaCheia(Pilha *p)
{
    return (p->topo == MAX_RESERVA - 1);
}

// CORRIGIDO: Retorno de int
int pilhaVazia(Pilha *p)
{
    return (p->topo == -1);
}

void push(Pilha *p, Peca peca)
{
    if (pilhaCheia(p))
    {
        printf(">> Pilha de reserva cheia!\n");
        return;
    }
    p->topo++;
    p->pecas[p->topo] = peca;
}

Peca pop(Pilha *p)
{
    Peca pecaRemovida;
    if (pilhaVazia(p))
    {
        printf(">> Pilha de reserva vazia!\n");
        pecaRemovida.nome = ' ';
        pecaRemovida.id = -1;
        return pecaRemovida;
    }
    pecaRemovida = p->pecas[p->topo];
    p->topo--;
    return pecaRemovida;
}

void mostrarPilha(Pilha *p)
{
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p))
    {
        printf("[VAZIA]");
    }
    for (int i = p->topo; i >= 0; i--)
    {
        printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
    }
    printf("\n");
}

// --- FUNÇÕES DE INTERFACE ---

void telaDeBoasVindas()
{
    system("cls || clear"); // Limpa a tela
    printf("+----------------------------------------------------+\n");
    printf("|                                                    |\n");
    printf("|           BEM-VINDO AO TETRIS STACK!               |\n");
    printf("|                                                    |\n");
    printf("|   Seu objetivo e gerenciar a fila de pecas.        |\n");
    printf("|   Use as pecas da frente e adicione novas no fim.  |\n");
    printf("|                                                    |\n");
    printf("+----------------------------------------------------+\n");
    printf("\nPressione Enter para comecar...");
    getchar(); // Pausa o programa e espera o usuário pressionar Enter
}

void exibirEstadoAtual(Fila *f, Pilha *p)
{
    system("cls || clear");
    printf("\n--- ESTADO ATUAL ---\n");
    mostrarFila(f);
    mostrarPilha(p);
    printf("--------------------\n");
}

void menu()
{
    printf("\nOpcoes de Acao:\n");
    printf("1. Jogar peca\n");
    printf("2. Reservar peca\n");
    printf("3. Usar peca reservada\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
}

void limpar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// --- FUNÇÃO PRINCIPAL ---
int main()
{
    Fila filaDePecas;
    Pilha pilhaDeReserva;
    int escolha;

    telaDeBoasVindas();

    srand(time(NULL));
    initFila(&filaDePecas);
    initPilha(&pilhaDeReserva);

    printf("Iniciando o jogo com 5 pecas...\n");
    for (int i = 0; i < 5; i++)
    {
        inserirPeca(&filaDePecas, gerarPeca());
    }

    // Loop principal do jogo com a LÓGICA CORRIGIDA
    while (1)
    {
        exibirEstadoAtual(&filaDePecas, &pilhaDeReserva);
        menu();
        scanf("%d", &escolha);
        limpar_buffer();

        switch (escolha)
        {
        case 1:
        { // Jogar Peça
            jogarPeca(&filaDePecas);
            inserirPeca(&filaDePecas, gerarPeca()); // Reabastece a fila
            break;
        }
        case 2:
        { // Reservar Peça
            if (!pilhaCheia(&pilhaDeReserva))
            {
                Peca pecaMovida = jogarPeca(&filaDePecas);
                if (pecaMovida.id != -1)
                {
                    push(&pilhaDeReserva, pecaMovida);
                    printf(">> Peca [%c %d] movida para a reserva.\n", pecaMovida.nome, pecaMovida.id);
                    inserirPeca(&filaDePecas, gerarPeca()); // Reabastece a fila
                }
            }
            else
            {
                printf(">> A pilha de reserva esta cheia! Nao e possivel reservar.\n");
            }
            break;
        }
        case 3:
        { // Usar Peça Reservada
            Peca pecaUsada = pop(&pilhaDeReserva);
            if (pecaUsada.id != -1)
            {
                printf(">> Peca reservada [%c %d] foi usada.\n", pecaUsada.nome, pecaUsada.id);
                inserirPeca(&filaDePecas, gerarPeca()); // Reabastece a fila
            }
            break;
        }
        case 0:
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