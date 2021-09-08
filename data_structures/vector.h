#ifndef VECTOR_H
#define VECTOR_H

// TODO: Buffer before the start of the data array

typedef struct vector vector;

struct vector
{
	size_t type_size;

	uint32_t count;
	uint32_t max;

	byte_t* start;
	byte_t* finish;
	byte_t* end_storage;
};

vector vec_init (size_t type_size, uint32_t elementCount);

void vec_push   (vector* vec, void* val);

void vec_pop    (vector* vec, void** dest);

void vec_insert (vector* vec, void* val, uint32_t index);

void vec_resize (vector* vec, uint32_t elementCount);

void vec_mresize(vector* vec, uint32_t elementCount);

void vec_free   (vector* vec);

#endif // !VECTOR_H
