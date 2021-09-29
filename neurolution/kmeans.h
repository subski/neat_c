#ifndef KMEANS_H
#define KMEANS_H

#include "neurolution/agent.h"

#include "data_structures/clist.h"

typedef struct Point
{
    double x;
    double y;
    int specie;
} Point;

typedef struct Specie
{
	int id;
    Agent* centroid;
	clist* specimens;
} Specie;

void kmeans_init();

void kmeans_run();

void kmeans_end();

int speciate( clist* pointlist, clist* species );

double distance_point( Point *p1, Point *p2 );


#endif // !KMEANS_H