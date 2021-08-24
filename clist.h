#ifndef CLIST_H
#define CLIST_H

#include <stdarg.h>
#include <stdbool.h>


#define next(node) node = node->next
#define CLIST_ITER(node, iter, code)		\
	{										\
		clist* iter = node;					\
		do									\
		{									\
			code							\
			next(iter);						\
		} while (iter != node);				\
	}

#define INSERT_DATA(node, type, data)		\
	{										\
		type* _ptr = malloc(sizeof(type));	\
		if (_ptr != NULL)					\
		{									\
			*_ptr = data;					\
			insert(node, _ptr);				\
		}									\
	}

typedef struct clist clist;

struct clist
{
	void* data;
	clist* next;
};

// TODO: Add pooling for Cycled List structs
// static clist* P_CLIST = 0;

bool insert(clist** node, void* data);

bool insert_data(clist** node, size_t size, ...);

void clear(clist** node);

void clean(clist** node);

void pclean(clist** node, clist** pool);

unsigned int len(clist* node);

void* random(clist* node);

#endif
