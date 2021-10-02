#ifndef SPECIE_H
#define SPECIE_H

#include "neurolution/agent.h"

#include "data_structures/clist.h"

typedef struct Specie
{
	int id;
    Agent* centroid;
	clist* specimens;
	double fitness;
	double proportion;
} Specie;

void specie_sort(Specie* specie, int order);

void free_specie(Specie* specie);

#endif // !SPECIE_H