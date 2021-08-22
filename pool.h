#ifndef POOL_H
#define POOL_H

#include "clist.h"

typedef clist pool;

void* request(pool** _pool, size_t type);

void pfree(pool** _pool, void** data, size_t type);

#endif