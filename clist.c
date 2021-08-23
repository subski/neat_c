#include "clist.h"

#include <stdlib.h>
#include <stdio.h>

bool insert(clist** node, void* data)
{
	clist* new_node = malloc(sizeof(clist));
	if (new_node == NULL)
	{
		perror("malloc error in clist => insert()");
		return false;
	}

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

uint16_t len(clist* node)
{
	if (node == NULL)
		return 0;

	uint16_t i = 0;
	CLIST_ITER(node, iter,
		i++;
	)
		return i;
}