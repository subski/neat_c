#ifndef CLIST_H
#define CLIST_H

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

// TODO: Add pooling for Cycled List structs
// static clist* P_CLIST = 0;

bool insert(clist** node, void* data);

void clear(clist** node);

void clean(clist** node);

void pclean(clist** node, clist** pool);

unsigned short len(clist* node);

#endif
