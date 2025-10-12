#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Incluído para a função srand()

// Pilha de Historico

#define MAX_CAMINHO 10

typedef struct Sala Sala; // Declara a estrutura Sala

// Estrutura para armazenar o caminho percorrido
typedef struct
{
    Sala *salas[MAX_CAMINHO];
    int topo;
} PilhaSalas;

// Inicializa a Pilha de Salas
void initPilhaSalas(PilhaSalas *pilha)
{
    pilha->topo = -1;
}

// Verifica se a Pilha de Salas esta vazia
int pilhaSalasVazia(PilhaSalas *pilha)
{
    return pilha->topo == -1;
}

// Adiciona um ponteiro para uma sala na Pilha de Salas
void pushPilhaSalas(PilhaSalas *pilha, Sala *sala)
{ // Verifica se a Pilha de Salas esta cheia
    if (pilha->topo < MAX_CAMINHO - 1)
    {
        pilha->topo++;
        pilha->salas[pilha->topo] = sala;
    }
}

// Remove e retorna um ponteiro para uma sala da Pilha de Salas
Sala *popPilhaSalas(PilhaSalas *pilha)
{
    if (!pilhaSalasVazia(pilha))
    {
        Sala *salaRetornada = pilha->salas[pilha->topo];
        pilha->topo--;
        return salaRetornada;
    }
    return NULL; // Retorna NULL se a Pilha de Salas estiver vazia
}

// --- ESTRUTURAS DE DADOS ---

// Estrutura para uma sala da mansão (nó da árvore do mapa).
typedef struct Sala
{
    char value[50];
    const char *pista;
    struct Sala *left;
    struct Sala *right;
} Sala;

// Estrutura para os nós da Árvore de Pistas (BST).
typedef struct NoPista
{
    char pista[100];
    struct NoPista *left;
    struct NoPista *right;
} NoPista;

#define TAMANHO_TABELA 50

// Estrutura para os nós da Tabela Hash (lista encadeada).
typedef struct NoHash
{
    char chave[100]; // A pista
    char valor[50];  // O suspeito
    struct NoHash *proximo;
} NoHash;

// Estrutura principal da Tabela Hash.
typedef struct
{
    int capacidade;
    NoHash **tabela;
} TabelaHash;

// --- FUNÇÕES DA TABELA HASH ---

// Converte a string da pista em um índice para o vetor.  // unsigned int para evitar problemas com valores negativos
unsigned int funcaoHash(const char *chave, int capacidade)
{
    unsigned long int hash = 5381;
    int c;
    while ((c = *chave++))
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % capacidade;
}

// Cria e inicializa uma Tabela Hash vazia.
TabelaHash *criarTabelaHash(int capacidade)
{
    TabelaHash *tabela = malloc(sizeof(TabelaHash));
    tabela->capacidade = capacidade;
    tabela->tabela = calloc(capacidade, sizeof(NoHash *));
    return tabela;
}

// Insere uma associação pista/suspeito na tabela hash.
void inserirNaHash(TabelaHash *tabela, const char *chave, const char *valor)
{
    int indice = funcaoHash(chave, tabela->capacidade);
    NoHash *novoNo = malloc(sizeof(NoHash));
    strcpy(novoNo->chave, chave);
    strcpy(novoNo->valor, valor);
    novoNo->proximo = tabela->tabela[indice];
    tabela->tabela[indice] = novoNo;
}

// Busca um suspeito na tabela hash usando a pista como chave.
const char *encontrarSuspeito(TabelaHash *tabela, const char *chave)
{
    int indice = funcaoHash(chave, tabela->capacidade);
    NoHash *noAtual = tabela->tabela[indice];
    while (noAtual != NULL)
    {
        if (strcmp(noAtual->chave, chave) == 0)
        {
            return noAtual->valor;
        }
        noAtual = noAtual->proximo;
    }
    return NULL;
}

// Libera toda a memória alocada pela tabela hash.
void freeTabelaHash(TabelaHash *tabela)
{
    for (int i = 0; i < tabela->capacidade; i++)
    {
        NoHash *noAtual = tabela->tabela[i];
        while (noAtual != NULL)
        {
            NoHash *proximo = noAtual->proximo;
            free(noAtual);
            noAtual = proximo;
        }
    }
    free(tabela->tabela);
    free(tabela);
}

// --- LÓGICA DE JULGAMENTO ---

// Função auxiliar recursiva que percorre a BST de pistas e conta evidências.
void contarPistas(NoPista *raizPistas, TabelaHash *tabela, const char *acusado, int *contador)
{
    if (raizPistas == NULL)
        return;

    contarPistas(raizPistas->left, tabela, acusado, contador);
    const char *suspeito = encontrarSuspeito(tabela, raizPistas->pista);
    if (suspeito != NULL && strcmp(suspeito, acusado) == 0)
    {
        (*contador)++;
    }
    contarPistas(raizPistas->right, tabela, acusado, contador);
}

// Função principal da fase de julgamento que interage com o jogador.
void verificarSuspeitoFinal(NoPista *arvoreDePistas, TabelaHash *tabela)
{
    char acusado[50];
    int contadorDeEvidencias = 0;

    printf("\n--- FASE DE JULGAMENTO ---\n\n");
    printf("Os suspeitos sao: Dr. Blackwood, Sra. White, Prof. Plum\n");
    printf("Com base nas pistas, quem voce acusa? ");
    fgets(acusado, 50, stdin);
    acusado[strcspn(acusado, "\n")] = 0;

    contarPistas(arvoreDePistas, tabela, acusado, &contadorDeEvidencias);

    printf("\nAnalisando as evidencias...\n");
    printf("Foram encontradas %d pista(s) apontando para %s.\n", contadorDeEvidencias, acusado);

    // O requisito pede para verificar se "pelo menos duas" pistas apontam para o suspeito.
    if (contadorDeEvidencias >= 2)
    {
        printf("\nPARABENS! Voce solucionou o caso! O culpado era %s!\n", acusado);
    }
    else
    {
        printf("\nNAO FOI DESTA VEZ! O verdadeiro culpado escapou...\n");
    }
}

// --- FUNÇÕES DA ÁRVORE DO MAPA ---

Sala *createRoom(const char *nome, const char *pista)
{
    Sala *newSala = malloc(sizeof(Sala));
    strcpy(newSala->value, nome);
    newSala->pista = pista;
    newSala->left = NULL;
    newSala->right = NULL;
    return newSala;
}

void freeTreeSalas(Sala *root)
{
    if (root != NULL)
    {
        freeTreeSalas(root->left);
        freeTreeSalas(root->right);
        free(root);
    }
}

// --- FUNÇÕES DA ÁRVORE DE PISTAS (BST) ---

NoPista *inserirPista(NoPista *raizPistas, const char *pista)
{
    if (raizPistas == NULL)
    {
        NoPista *novoNo = malloc(sizeof(NoPista));
        strcpy(novoNo->pista, pista);
        novoNo->left = novoNo->right = NULL;
        return novoNo;
    }
    int cmp = strcmp(pista, raizPistas->pista);
    if (cmp < 0)
    {
        raizPistas->left = inserirPista(raizPistas->left, pista);
    }
    else if (cmp > 0)
    {
        raizPistas->right = inserirPista(raizPistas->right, pista);
    }
    return raizPistas;
}

void exibirPistas(NoPista *raizPistas)
{
    if (raizPistas != NULL)
    {
        exibirPistas(raizPistas->left);
        printf("- %s\n", raizPistas->pista);
        exibirPistas(raizPistas->right);
    }
}

void freeTreePistas(NoPista *root)
{
    if (root != NULL)
    {
        freeTreePistas(root->left);
        freeTreePistas(root->right);
        free(root);
    }
}

// --- FUNÇÕES DE INTERFACE ---

void exibirMenuPrincipal()
{
    printf("+------------------------------------+\n");
    printf("|          DETECTIVE QUEST           |\n");
    printf("+------------------------------------+\n");
    printf("| 1. Iniciar Exploracao              |\n");
    printf("| 0. Sair do Jogo                    |\n");
    printf("+------------------------------------+\n");
    printf(" Escolha uma opcao: ");
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
    int escolha;
    srand(time(NULL)); // Inicializa o gerador de números aleatórios.

    // Loop do menu principal.
    while (1)
    {
        system("cls || clear");
        exibirMenuPrincipal();
        scanf("%d", &escolha);
        limpar_buffer();

        switch (escolha)
        {
        case 1:
        { // Inicia o jogo
            // 1. Monta o mapa da mansão com as pistas.
            Sala *raiz = createRoom("Hall de Entrada", NULL);
            raiz->left = createRoom("Biblioteca", "Um livro antigo menciona uma passagem secreta.");
            raiz->left->left = createRoom("Estudio", "Apenas o Dr. Blackwood tinha a chave desta sala.");
            raiz->left->left->right = createRoom("Passagem Secreta", "Encontrei um cofre! A senha parece ser o nome do dono da mansao.");
            raiz->right = createRoom("Sala de Jantar", "Um quadro na parede tem a inscricao 'Dr. Blackwood'.");
            raiz->right->left = createRoom("Cozinha", "Ha restos de um veneno raro na lata de lixo.");

            // Cria a árvore de pistas (vazia) e a tabela de suspeitos.
            NoPista *arvoreDePistas = NULL;
            TabelaHash *tabelaDeSuspeitos = criarTabelaHash(TAMANHO_TABELA);

            // Popula a tabela hash com as associações Pista -> Suspeito.
            inserirNaHash(tabelaDeSuspeitos, "Um quadro na parede tem a inscricao 'Dr. Blackwood'.", "Dr. Blackwood");
            inserirNaHash(tabelaDeSuspeitos, "Apenas o Dr. Blackwood tinha a chave desta sala.", "Dr. Blackwood");
            inserirNaHash(tabelaDeSuspeitos, "Ha restos de um veneno raro na lata de lixo.", "Sra. White");

            // 2. Prepara as variáveis para o início da exploração.
            Sala *salaAtual = raiz;
            char escolhaCaminho = ' ';
            PilhaSalas historico;
            initPilhaSalas(&historico);
            printf("\nBem-vindo ao Detective Quest!\n");

            // Loop de exploração: continua até o jogador digitar 's'.
            while (escolhaCaminho != 's' && escolhaCaminho != 'S')
            {
                system("cls || clear");
                printf("Voce esta na sala: %s\n\n", salaAtual->value);

                // Verifica se a sala atual tem uma pista e a coleta.
                if (salaAtual->pista != NULL)
                {
                    printf("Voce encontrou uma pista: \"%s\"\n", salaAtual->pista);
                    arvoreDePistas = inserirPista(arvoreDePistas, salaAtual->pista);
                    salaAtual->pista = NULL;
                }

                printf("\nCaminhos disponiveis:\n");
                if (salaAtual->left)
                    printf(" (e)squerda -> %s\n", salaAtual->left->value);
                if (salaAtual->right)
                    printf(" (d)ireita -> %s\n", salaAtual->right->value);
                if (!pilhaSalasVazia(&historico))
                    printf(" (v)oltar -> %s\n", historico.salas[historico.topo]->value);
                printf(" (s)air da exploracao para o julgamento\n");
                printf("Para onde voce quer ir? ");

                scanf(" %c", &escolhaCaminho);
                limpar_buffer();

                if (escolhaCaminho == 'e' || escolhaCaminho == 'E')
                {
                    if (salaAtual->left)
                    {
                        pushPilhaSalas(&historico, salaAtual);
                        salaAtual = salaAtual->left;
                    }
                    else
                    {
                        printf("\n>> Nao ha um caminho nessa direcao!\n");
                    }
                }
                else if (escolhaCaminho == 'd' || escolhaCaminho == 'D')
                {
                    if (salaAtual->right)
                    {
                        pushPilhaSalas(&historico, salaAtual);
                        salaAtual = salaAtual->right;
                    }
                    else
                    {
                        printf("\n>> Nao ha um caminho nessa direcao!\n");
                    }
                }
                else if (escolhaCaminho == 'v' || escolhaCaminho == 'V')
                {
                    if (!pilhaSalasVazia(&historico))
                    {
                        salaAtual = popPilhaSalas(&historico);
                    }
                    else
                    {
                        printf("\n>> Voce ja esta no Hall de Entrada! Nao e possivel voltar mais.\n");
                    }
                }
                else if (escolhaCaminho != 's' && escolhaCaminho != 'S')
                {
                    printf("\n>> Comando invalido! Tente novamente.\n");
                }

                if (escolhaCaminho != 's' && escolhaCaminho != 'S')
                {
                    printf("\nPressione Enter para continuar...");
                    getchar();
                }
            }

            // 3. Fim da exploração: exibe as pistas e inicia o julgamento.
            printf("\n--- PISTAS COLETADAS ---\n");
            if (arvoreDePistas == NULL)
            {
                printf("Nenhuma pista foi coletada.\n");
            }
            else
            {
                exibirPistas(arvoreDePistas);
            }

            verificarSuspeitoFinal(arvoreDePistas, tabelaDeSuspeitos);

            // 4. Limpeza da Memória: libera todas as três estruturas de dados.
            freeTreeSalas(raiz);
            freeTreePistas(arvoreDePistas);
            freeTabelaHash(tabelaDeSuspeitos);

            printf("\nPressione Enter para voltar ao menu principal...");
            getchar();
            break;
        }
        case 0:
        { // Sair do jogo
            printf("\nObrigado por jogar Detective Quest!\n");
            return 0;
        }
        default:
        { // Opção inválida
            printf("\nOpcao invalida! Pressione Enter para tentar novamente.");
            getchar();
            break;
        }
        }
    }
    return 0;
}