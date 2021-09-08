#include "data_structures/pool.h"

#include "data_structures/clist.h"
#include "tools/utils.h"

void* request(pool** _pool, size_t size)
{
	void* result;
	if (*_pool == NULL) // empty pool
	{
		result = malloc(size);
		MCHECK(result, NULL);

		return result;
	}

	if ((*_pool)->next->data == NULL)
	{
		perror("pool data error.");
		return NULL;
	}

	pool* pool_node = (*_pool)->next;

	result = pool_node->data;

	if (*_pool == (*_pool)->next)
	{
		*_pool = NULL;
	}
	else
	{
		(*_pool)->next = pool_node->next;
	}

	free(pool_node);

	return result;
}

void pfree(pool** _pool, void** data)
{
	insert(_pool, *data);

	*data = NULL;
}