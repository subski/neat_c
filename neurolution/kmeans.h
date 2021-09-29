#ifndef KMEANS_H
#define KMEANS_H

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
	int color[3];
    Point centroid;
	clist* points;
} Specie;

void kmeans_init();

int kmeans_run();

void kmeans_end();

int speciate( clist* pointlist, clist* species );

double distance_point( Point *p1, Point *p2 );


#endif // !KMEANS_H