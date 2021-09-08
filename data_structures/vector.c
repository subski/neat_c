#include "data_structures/vector.h"

#include "tools/utils.h"

vector vec_init(size_t type_size, uint32_t elementCount)
{
	vector new_vector;

	new_vector.type_size = type_size;
	new_vector.count = elementCount;
	new_vector.max = elementCount;

	new_vector.start = malloc(elementCount * type_size);
	new_vector.finish = new_vector.start + type_size * elementCount;

	new_vector.end_storage = new_vector.finish;

	return new_vector;
}

void vec_push(vector* vec, void* val)
{
	vec_mresize(vec, vec->count + 1);
	memcpy(vec->finish, val, vec->type_size);
	vec->finish = vec->finish + vec->type_size;
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

	vec->finish = vec->finish - vec->type_size;
	memcpy(*dest, vec->finish, vec->type_size);
	vec->count--;
}

void vec_insert(vector* vec, void* val, uint32_t index);

void vec_resize(vector* vec, uint32_t elementCount);

void vec_mresize(vector* vec, uint32_t maxCount)
{
	if (vec->max == maxCount)
		return;

	vec->max = maxCount;
	if (vec->count > maxCount)
	{
		vec->count = maxCount;
	}

	byte_t* new_mem = realloc(vec->start, vec->type_size * maxCount);
	MCHECK(new_mem, );

	vec->start = new_mem;
	vec->finish = new_mem + vec->count * vec->type_size;
	vec->end_storage = new_mem + maxCount * vec->type_size;
}

void vec_free(vector* vec)
{
	if (vec == NULL)
		return;

	free(vec->start);
	free(vec);
}