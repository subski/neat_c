#ifndef CLIST_H
#define CLIST_H

#include <stdarg.h>
#include <stdbool.h>

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
	do											\
	{											\
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

void* random_max(clist* node, int max);

bool find(clist* node, void* data);

#endif
