#include "data_structures/vector.h"

#include <string.h>

#include "tools/utils.h"
#include "tools/malloc_dbg.h"

vector vec_init(size_t type_size, unsigned int elementCount)
{
	vector new_vector;

	
	new_vector.type_size = type_size;
	new_vector.count = elementCount;
	new_vector.max = elementCount;

	new_vector.start = malloc(elementCount * type_size);
	new_vector.finish = (char*)new_vector.start + type_size * elementCount;

	new_vector.end_storage = new_vector.finish;
	
	return new_vector;
}

void vec_push(vector* vec, void* val)
{
	vec_mresize(vec, vec->count+1);
	memcpy(vec->finish, val, vec->type_size);
	vec->finish = (char*)vec->finish + vec->type_size;
	vec->count++;
}

void vec_pop(vector* vec, void** dest)
{
	if (vec->start == vec->finish) // empty vector
	{
		*dest = NULL;
		return;
	}

	*dest = malloc(vec->type_size);
	MCHECK(*dest, );

	vec->finish = (char *)vec->finish - vec->type_size;
	memcpy(*dest, vec->finish, vec->type_size);
	vec->count--;
}

void vec_insert(vector* vec, void* val, unsigned int index);

void vec_resize(vector* vec, unsigned int elementCount);

void vec_mresize(vector* vec, unsigned int maxCount)
{
	if (vec->max == maxCount)
		return;

	vec->max = maxCount;
	if (vec->count > maxCount)
	{
		vec->count = maxCount;
	}

	void* new_mem = realloc(vec->start, vec->type_size * maxCount);
	MCHECK(new_mem, );

	vec->start = new_mem;
	vec->finish = (char*)new_mem + vec->count * vec->type_size;
	vec->end_storage = (char*)new_mem + maxCount * vec->type_size;
}

void vec_free(vector* vec)
{
	if (vec == NULL)
		return;

	free(vec->start);
	free(vec);
}