#include <stdlib.h>
#include <stdio.h>
#include <Windows.h> // used for Sleep(), use <unistd.h> for linux(?)

typedef struct LinkedList
{
    void* data;
    struct LinkedList* next;
} LinkedList;

void push(LinkedList** head, void* data_ptr)
{
    LinkedList* new = malloc(sizeof(LinkedList));
    new->next = *head;
    new->data = data_ptr;

    *head = new;
}

void append(LinkedList* node, void* data_ptr)
{
    while (node->next != NULL)
        node = node->next;

    LinkedList* new = malloc(sizeof(LinkedList));
    new->data = data_ptr;
    new->next = NULL;

    node->next = new;
}

void destroy_list(LinkedList** head)
{
    LinkedList* current_node = *head;
    LinkedList* next_node;

    //int i = 0;

    while (current_node != NULL)
    {
        next_node = current_node->next;
        free(current_node);
        current_node = next_node;
        //i++;
    }
    ///printf("%d\n", i);
    *head = NULL;
}

int _main(void)
{
    // create initial head for the list
    LinkedList* list;

    for (int j = 0; j < 10; j++)
    {
        list = malloc(sizeof(LinkedList));
        list->next = NULL;
        list->data = NULL;

        Sleep(1000); // added delay to better see the memory usage

        // trying to fill the list with both methods and compare the result of memory usage
        for (int i = 0; i < 20000; i++)
        {
            append(list, NULL);

            push(&list, NULL);
        }

        Sleep(1000);

        // trying to free all struct LinkedList
        destroy_list(&list);

        Sleep(3000);

    }
    system("Pause");
    return EXIT_SUCCESS;
}