#include "llist.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void push(llist* list, void* data_ptr)
{
    assert(list != NULL || data_ptr != NULL);

    if (list == NULL) {
        fprintf(stderr, "Cannot push data to linked list, the list pointer is NULL.");
        return;
    }
    
    llist* buffer = malloc(sizeof(llist));
    
    if (buffer == NULL) {
        fprintf(stderr, "Allocation error.");
        return;
    }

    *buffer = (llist){ list->next, list->data };
    *list = (llist){ buffer, data_ptr };
}

void append(llist* list, void* data_ptr)
{
    assert(list != NULL || data_ptr != NULL);

    if (list == NULL) {
        fprintf(stderr, "Cannot push data to linked list, the list pointer is NULL.");
        return;
    }

    llist* item = list;
    while (item->next != NULL)
    {
        item = item->next;
    }

    llist* buffer = malloc(sizeof(llist));

    if (buffer == NULL) {
        fprintf(stderr, "Allocation error.");
        return;
    }

    *buffer = (llist) { NULL, data_ptr };
    item->next = buffer;
}