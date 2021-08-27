#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct vector vector;

struct vector
{
	size_t type_size;

	unsigned int count;
	unsigned int max;

	void* start;
	void* finish;
	void* end_storage;
};

vector vec_init(size_t type_size, unsigned int elementCount);

void vec_push(vector* vec, void* val);

void vec_pop(vector* vec, void** dest);

void vec_insert(vector* vec, void* val, unsigned int index);

void vec_resize(vector* vec, unsigned int elementCount);

void vec_mresize(vector* vec, unsigned int elementCount);

void vec_free(vector* vec);

#endif // !VECTOR_H
