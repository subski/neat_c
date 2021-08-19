#ifndef LLIST_H
#define LLIST_H

typedef struct llist llist;

struct llist
{
    llist* next;
    void* data;
};

void push(llist* list, void* data_ptr);

void append(llist* list, void* data_ptr);

#endif