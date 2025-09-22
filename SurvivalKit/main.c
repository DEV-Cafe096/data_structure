#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura de Item
typedef struct Item
{
    char name[50];
    char type[20];
    int quantity;
} Item;

// Vetor de struct Item
Item inventory[10];
int numItems = 0;

// Funcao para inserir item
void insertItem(Item item)
{
    for (int i = 0; i < numItems; i++)
    {
        if (strcmp(inventory[i].name, item.name) == 0)
        {
            inventory[i].quantity += item.quantity;
            // MENSAGEM CORRIGIDA
            printf(">> Quantidade do item '%s' atualizada!\n", item.name);
            return;
        }
    }
    if (numItems < 10)
    {
        inventory[numItems] = item;
        numItems++;
        // MENSAGEM CORRIGIDA
        printf(">> Item '%s' inserido com sucesso!\n", item.name);
    }
    else
    {
        printf(">> Mochila cheia! Nao foi possivel inserir o item.\n");
    }
}

// Funcao para remover item
void removeItem(char name[])
{
    int index = -1;
    for (int i = 0; i < numItems; i++)
    {
        if (strcmp(inventory[i].name, name) == 0)
        {
            index = i;
            break;
        }
    }
    if (index != -1)
    {
        for (int i = index; i < numItems - 1; i++)
        {
            inventory[i] = inventory[i + 1];
        }
        numItems--;
        printf(">> Item '%s' removido com sucesso!\n", name);
    }
    else
    {
        printf(">> Item '%s' nao encontrado!\n", name);
    }
}

// Funcao para exibir inventario
void showInventory()
{
    printf("\n-----Inventario Atual-----\n");
    if (numItems == 0)
    {
        printf("Mochila vazia!\n");
        return;
    }
    for (int i = 0; i < numItems; i++)
    {
        printf("%d. %s (Tipo: %s, Quantidade: %d)\n", i + 1, inventory[i].name, inventory[i].type, inventory[i].quantity);
    }
    printf("---------------------------\n");
}

// Funcao para buscar item
Item *searchItem(char name[])
{
    for (int i = 0; i < numItems; i++)
    {
        if (strcmp(inventory[i].name, name) == 0)
        {
            return &inventory[i];
        }
    }
    return NULL;
}

// Funcao para exibir o menu principal
void showMenu()
{
    printf("+==========================================+\n");
    printf("|            SURVIVAL KIT MENU             |\n");
    printf("+==========================================+\n");
    printf("| 1. Adicionar Item                        |\n");
    printf("| 2. Remover Item                          |\n");
    printf("| 3. Listar Itens                          |\n");
    printf("| 4. Buscar Item                           |\n");
    printf("| 0. Sair                                  |\n");
    printf("+==========================================+\n");
    printf(" Escolha uma opcao: ");
}

// Funcao Principal
int main()
{
    int choice;
    Item tempItem;
    char searchName[50];

    while (1)
    {
        // Limpa a tela
        system("cls || clear");

        showMenu();

        scanf("%d", &choice);

        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ; // Limpa o buffer

        switch (choice)
        {
        case 1:
            printf("Digite o nome do item: ");
            fgets(tempItem.name, 50, stdin);
            tempItem.name[strcspn(tempItem.name, "\n")] = 0;

            printf("Digite o tipo do item: ");
            fgets(tempItem.type, 20, stdin);
            tempItem.type[strcspn(tempItem.type, "\n")] = 0;

            printf("Digite a quantidade: ");
            scanf("%d", &tempItem.quantity);
            while ((c = getchar()) != '\n' && c != EOF)
                ;

            insertItem(tempItem);
            // Pausa para o usuário ver a mensagem antes do loop recomeçar
            printf("\nPressione Enter para continuar...");
            getchar();
            break;
        case 2:
            printf("Digite o nome do item a ser removido: ");
            fgets(searchName, 50, stdin);
            searchName[strcspn(searchName, "\n")] = 0;
            removeItem(searchName);

            printf("\nPressione Enter para continuar...");
            getchar();
            break;
        case 3:
            showInventory();
            printf("\nPressione Enter para continuar...");
            getchar();
            break;
        case 4:
            printf("Digite o nome do item a ser buscado: ");
            fgets(searchName, 50, stdin);
            searchName[strcspn(searchName, "\n")] = 0;

            Item *foundItem = searchItem(searchName);
            if (foundItem != NULL)
            {
                printf("\n>> Item Encontrado! <<\n");
                printf("Nome: %s\nTipo: %s\nQuantidade: %d\n",
                        foundItem->name, foundItem->type, foundItem->quantity);
            }
            else
            {
                printf(">> Item '%s' nao encontrado.\n", searchName);
            }

            printf("\nPressione Enter para continuar...");
            getchar();
            break;
        case 0:
            printf("Saindo do programa...\n");
            return 0;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            printf("\nPressione Enter para continuar...");
            getchar();
        }
    }
    return 0;
}