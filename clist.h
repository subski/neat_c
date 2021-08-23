#pragma once
#ifndef CLIST_H
#define CLIST_H

#include "utils.h"

#define bool _Bool

#define next(node) node = node->next
#define CLIST_ITER(node, iter, code)	\
	clist* iter = node;					\
	do									\
	{									\
		code							\
		next(iter);						\
	} while (iter != node);

typedef struct clist clist;

struct clist
{
	void* data;
	clist* next;
};

bool insert(clist** node, void* data);

void clear(clist** node);

void clean(clist** node);

uint16_t len(clist* node);

#endif
