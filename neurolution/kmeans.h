#ifndef KMEANS_H
#define KMEANS_H

#include "data_structures/clist.h"

void kmeans_init();

void kmeans_run();

void kmeans_end();

int speciate( clist* pointlist, clist* species );

#endif // !KMEANS_H