#include "clist.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "pcg_basic.h"
#include "utils.h"

bool insert(clist** node, void* data)
{
	clist* new_node = malloc(sizeof(clist));
	MCHECK(new_node, false);

	if (*node == NULL) // empty list
	{
		*node = new_node;
		(*node)->data = data;
		(*node)->next = *node;
	}
	else
	{
		new_node->data = data;
		new_node->next = (*node)->next;

		(*node)->next = new_node;
	}

	return true; // SUCCESS
}

void clear(clist** node)
{
	if (*node == NULL)
		return;

	clist* current = *node;
	clist* next;
	do
	{
		next = current->next;
		free(current);
		current = next;
	} while (next != *node);

	*node = NULL;
}

void clean(clist** node)
{
	if (*node == NULL)
		return;

	clist* current = *node;
	clist* next;
	do
	{
		next = current->next;
		free(current->data);
		free(current);
		current = next;
	} while (next != *node);

	*node = NULL;
}

void pclean(clist** node, clist** pool)
{
	if (*node == NULL)
		return;

	clist* current = *node;
	clist* next;
	do
	{
		next = current->next;
		insert(pool, current->data);
		free(current);
		current = next;
	} while (next != *node);

	*node = NULL;
}

unsigned int len(clist* node)
{
	if (node == NULL)
		return 0;

	unsigned int i = 0;
	CLIST_ITER(node, iter,
		i++;
	)
		return i;
}

void* random(clist* node)
{
	if (node == NULL)
		return NULL;

	unsigned int index = pcg32_boundedrand(len(node));
	
	unsigned int i = 0;
	CLIST_ITER(node, iter,
		if (index == i)
			return iter->data;
		i++;
	)

	return NULL;
}