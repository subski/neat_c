#include "pool.h"

void* request(pool** _pool, size_t type)
{
	void* result;
	if (*_pool == NULL)
	{
		result = malloc(type);
		if (result == NULL)
		{
			perror("malloc error");
			return NULL;
		}
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

void pfree(pool** _pool, void** data, size_t type)
{
	insert(_pool, *data);
	if (type != 0)
		memset(*data, 0, type);

	*data = NULL;
}