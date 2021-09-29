#ifndef UI_H
#define UI_H

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

void ui_init();

int ui_run();

void ui_exit();

int speciate( clist* pointlist, clist* species );

double distance_point( Point *p1, Point *p2 );

#endif // !UI_H