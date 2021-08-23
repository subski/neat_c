#pragma once
#ifndef POOL_H
#define POOL_H

#include <stddef.h>

#include "clist.h"

typedef clist pool;

/*
Returns a memory block of the given size from the pool.
Allocates a new block if none are avaiable.
*/
void* request(pool** _pool, size_t type);

/*
Save unused memory blocks into the pool for futur uses.
If a type is given, all bits int the block will be set to 0.
*/
void pfree(pool** _pool, void** data, size_t type);

#endif