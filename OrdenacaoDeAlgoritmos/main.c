#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Necessário para a função clock()

// Estrutura para um componente da torre
typedef struct Component {
    char name[30];
    char type[20];
    int priority;
} Component;

// Variáveis globais para o armazém de componentes
Component tower[20];
int componentCount = 0;

// Ponteiros para contadores de desempenho
long long comparison_count = 0;
double execution_time = 0.0;


// --- FUNÇÕES BÁSICAS ---

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void displayComponents() {
    printf("\n---------- COMPONENTES DA TORRE ----------\n");
    if (componentCount == 0) {
        printf("Nenhum componente cadastrado.\n");
        return;
    }
    // Mostra os componentes com nome, tipo e prioridade
    for (int i = 0; i < componentCount; i++) {
        printf("%d. %-20s (Tipo: %-15s, Prioridade: %d)\n", i + 1, tower[i].name, tower[i].type, tower[i].priority);
    }
    printf("----------------------------------------\n");
}

void addComponent(Component component) {
    if (componentCount < 20) {
        tower[componentCount] = component;
        componentCount++;
        printf(">> Componente '%s' adicionado com sucesso!\n", component.name);
    } else {
        printf(">> Armazem cheio! Nao foi possivel adicionar o componente.\n");
    }
}


// --- ALGORITMOS DE ORDENAÇÃO ---

// Bubble Sort para ordenar por NOME
void bubbleSortByName() {
    Component temp;
    comparison_count = 0;
    for (int i = 0; i < componentCount - 1; i++) {
        for (int j = 0; j < componentCount - i - 1; j++) {
            comparison_count++; // Conta cada comparação
            if (strcmp(tower[j].name, tower[j + 1].name) > 0) {
                temp = tower[j];
                tower[j] = tower[j + 1];
                tower[j + 1] = temp;
            }
        }
    }
    printf(">> Componentes ordenados por NOME com %lld comparacoes.\n", comparison_count);
}

// Insertion Sort para ordenar por TIPO
void insertionSortByType() {
    int i, j;
    Component key;
    comparison_count = 0;
    for (i = 1; i < componentCount; i++) {
        key = tower[i];
        j = i - 1;
        while (j >= 0) {
            comparison_count++; // Conta cada comparação
            if (strcmp(tower[j].type, key.type) > 0) {
                tower[j + 1] = tower[j];
                j = j - 1;
            } else {
                break; // Para o while se o elemento já está na posição correta
            }
        }
        tower[j + 1] = key;
    }
    printf(">> Componentes ordenados por TIPO com %lld comparacoes.\n", comparison_count);
}

// Selection Sort para ordenar por PRIORIDADE
void selectionSortByPriority() {
    int i, j, min_idx;
    Component temp;
    comparison_count = 0;
    for (i = 0; i < componentCount - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < componentCount; j++) {
            comparison_count++; // Conta cada comparação
            if (tower[j].priority < tower[min_idx].priority) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            temp = tower[min_idx];
            tower[min_idx] = tower[i];
            tower[i] = temp;
        }
    }
    printf(">> Componentes ordenados por PRIORIDADE com %lld comparacoes.\n", comparison_count);
}


// --- ALGORITMOS DE BUSCA ---

// Busca Sequencial por NOME
Component* linearSearchByName(char name[], int* comparisons) {
    *comparisons = 0;
    for (int i = 0; i < componentCount; i++) {
        (*comparisons)++;
        if (strcmp(tower[i].name, name) == 0) {
            return &tower[i];
        }
    }
    return NULL;
}

// Busca Binária por NOME (requer vetor ordenado por nome)
Component* binarySearchByName(char name[], int* comparisons) {
    int start = 0;
    int end = componentCount - 1;
    *comparisons = 0;
    while (start <= end) {
        int middle = start + (end - start) / 2;
        (*comparisons)++;
        int res = strcmp(name, tower[middle].name);
        if (res == 0) return &tower[middle];
        if (res > 0) start = middle + 1;
        else end = middle - 1;
    }
    return NULL;
}


// --- INTERFACE DO USUÁRIO ---

void displayMenu(int currentCount) {
    printf("\n+-----------------------------------------------+\n");
    printf("|          MONTAGEM DA TORRE DE FUGA          |\n");
    printf("+-----------------------------------------------+\n");
    printf("| Componentes no armazem: %-2d/20              |\n", currentCount);
    printf("+--- OPERACOES ---------------------------------+\n");
    printf("| 1. Adicionar Componente                       |\n");
    printf("| 2. Listar Componentes                         |\n");
    printf("+--- ORDENACAO (mostra comparacoes) ------------+\n");
    printf("| 3. Ordenar por Nome (Bubble Sort)             |\n");
    printf("| 4. Ordenar por Tipo (Insertion Sort)          |\n");
    printf("| 5. Ordenar por Prioridade (Selection Sort)    |\n");
    printf("+--- BUSCAS (mostra comparacoes) ---------------+\n");
    printf("| 6. Buscar por Nome (Sequencial)               |\n");
    printf("| 7. Buscar por Nome (Binaria - requer ordem.) |\n");
    printf("+-----------------------------------------------+\n");
    printf("| 0. Sair                                       |\n");
    printf("+-----------------------------------------------+\n");
    printf(" Escolha uma opcao: ");
}


// --- FUNÇÃO PRINCIPAL ---
int main() {
    int choice;
    Component tempComponent;
    char searchName[50];
    int comparisons = 0;
    clock_t start_t, end_t;

    while (1) {
        system("cls || clear");
        displayMenu(componentCount);
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: // Adicionar
                printf("Nome do componente: ");
                fgets(tempComponent.name, 30, stdin);
                tempComponent.name[strcspn(tempComponent.name, "\n")] = 0;
                printf("Tipo (controle, suporte, propulsao): ");
                fgets(tempComponent.type, 20, stdin);
                tempComponent.type[strcspn(tempComponent.type, "\n")] = 0;
                printf("Prioridade (1-10): ");
                scanf("%d", &tempComponent.priority);
                clearInputBuffer();
                addComponent(tempComponent);
                break;
            case 2: // Listar
                displayComponents();
                break;
            case 3: // Ordenar por Nome
                start_t = clock();
                bubbleSortByName();
                end_t = clock();
                execution_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;
                printf("   Tempo de execucao: %f segundos\n", execution_time);
                displayComponents();
                break;
            case 4: // Ordenar por Tipo
                start_t = clock();
                insertionSortByType();
                end_t = clock();
                execution_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;
                printf("   Tempo de execucao: %f segundos\n", execution_time);
                displayComponents();
                break;
            case 5: // Ordenar por Prioridade
                start_t = clock();
                selectionSortByPriority();
                end_t = clock();
                execution_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;
                printf("   Tempo de execucao: %f segundos\n", execution_time);
                displayComponents();
                break;
            case 6: // Busca Sequencial
                printf("Nome do componente (Busca Sequencial): ");
                fgets(searchName, 50, stdin);
                searchName[strcspn(searchName, "\n")] = 0;
                Component* foundCompSeq = linearSearchByName(searchName, &comparisons);
                printf(">> Busca Sequencial realizada com %d comparacoes.\n", comparisons);
                if (foundCompSeq) printf("   Encontrado: %s (Tipo: %s, Prioridade: %d)\n", foundCompSeq->name, foundCompSeq->type, foundCompSeq->priority);
                else printf(">> Componente '%s' nao encontrado.\n", searchName);
                break;
            case 7: // Busca Binária
                printf("AVISO: A busca binaria so funciona com componentes ordenados por nome!\n");
                printf("Nome do componente (Busca Binaria): ");
                fgets(searchName, 50, stdin);
                searchName[strcspn(searchName, "\n")] = 0;
                Component* foundCompBin = binarySearchByName(searchName, &comparisons);
                printf(">> Busca Binaria realizada com %d comparacoes.\n", comparisons);
                if (foundCompBin) printf("   Encontrado: %s (Tipo: %s, Prioridade: %d)\n", foundCompBin->name, foundCompBin->type, foundCompBin->priority);
                else printf(">> Componente '%s' nao encontrado.\n", searchName);
                break;
            case 0: // Sair
                printf("Saindo do programa...\n");
                return 0;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
        printf("\nPressione Enter para continuar...");
        getchar();
    }
    return 0;
}