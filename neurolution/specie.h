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
	int proportion;
} Specie;

void specie_sortByFitness(Specie* specie, int order);

void specie_computeFitness(Specie* specie);

void specie_mutate(Specie* specie);

Specie* specie_copy(Specie* specie);

void free_specie(Specie* specie);

#endif // !SPECIE_H