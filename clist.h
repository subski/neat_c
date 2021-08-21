#ifndef CLIST_H
#define CLIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define next(node) node = node->next

#define CLIST_ITER(node, iter, code)	\
	clist* iter = node;					\
	do									\
	{									\
		code							\
		next(iter);						\
	} while (iter != node);

typedef struct clist
{
	void* data;
	struct clist* next;
} clist;

bool insert(clist** node, void* data);

void clear(clist** node);

#endif // !CLIST_H
