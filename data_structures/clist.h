#ifndef CLIST_H
#define CLIST_H

#include <stdarg.h>

#define next(node) node = node->next

#define ITER(node, iter, code)					\
    do {										\
        clist* iter = node;						\
        do										\
        {										\
            code								\
            next(iter);							\
        } while (iter != node);					\
    } while (0);

#define ITER_V(node, iter, _data, type, code)	\
    do {										\
            if (node == NULL)					\
                break;							\
        clist* iter = node;						\
        type _data;								\
        do										\
        {										\
            _data = (type) iter->data;			\
            code								\
            next(iter);							\
        } while (iter != node);					\
    } while (0);
#define INSERT_DATA(node, type, data)			\
    do {										\
        type* _ptr = malloc(sizeof(type));		\
        if (_ptr != NULL)						\
        {										\
            *_ptr = data;						\
            insert(node, _ptr);					\
        }										\
    } while (0);

/*
    Cyclic List.
    usage: insert, clear, clean, pclean, len, random, random_max, find.

    Used to store pointers to some data when no particular order is required.
    A cyclic list should be represented as a 'clist*' and the address of the pointer is used by the functions that modify it.
    An empty list should be initialized to NULL.
    We can access an entire list by using any given node.

    example:
        clist* myList = NULL;
        insert(&mylist, data);
        int size = len(mylist);
        clean(&mylist);
*/
typedef struct clist clist;

struct clist
{
    void* data;
    clist* next;
};

// TODO: Add pooling for Cycled List structs
// static clist* P_CLIST = 0;

/*
    Insert a node at the top of the list.
    If the list was empty, the node.next will point to itself.
*/
bool insert     (clist** node, void* data);

/*
    Free the memory used by all the nodes in a given list while preserving the data referenced by the list.
    Set the list empty (NULL).
*/
void clear      (clist** node);

/*
    Free all the memory used by the nodes in the list as well as their associated data.
    Set the list empty (NULL).
*/
void clean      (clist** node);

/*
    Free the memory used by the nodes in a list and transfer the references of the data to a second list (here pool).
    Used mainly for dynamic memory pool.
    Set the list (node) empty (NULL).
*/
void pclean     (clist** node, clist** pool);

/*
    Return the number of nodes in a list.
*/
uint32_t len	(clist* node);

/*
    Return a data pointer from a node selected at random. 
*/
void* random    (clist* node);

/*
    Return a data pointer from a node selected at random with a range of selection. (can be higher than the length of the list)
    Useful if the length of the list is already known.
*/
void* random_max(clist* node, uint32_t max);

/*
    Return true if the given data is in the list.
*/
bool find       (clist* node, void* data);

#endif
