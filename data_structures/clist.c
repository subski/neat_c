#include "data_structures/clist.h"

#include "tools/pcg_basic.h"
#include "tools/utils.h"

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

	uint32_t i = 0;
	ITER(node, iter,
		i++;
	);
	return i;
}

void* random(clist* node)
{
	if (node == NULL)
		return NULL;

	uint32_t index = pcg32_boundedrand(len(node));

	uint32_t i = 0;
	ITER(node, iter,
		if (index == i)
			return iter->data;
	i++;
	);

	return NULL;
}

void* random_max(clist* node, int max)
{
	if (node == NULL)
		return NULL;

	uint32_t index = pcg32_boundedrand(max);

	uint32_t i = 0;
	ITER(node, iter,
		if (index == i)
			return iter->data;
	i++;
	);

	return NULL;
}

bool find(clist* node, void* data)
{
	if (node == NULL)
		return false;

	ITER(node, iter,
		if (iter->data == data)
			return true;
	);

	return false;
}