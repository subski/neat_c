#ifndef KMEANS_H
#define KMEANS_H

#include "neurolution/agent.h"

#include "data_structures/clist.h"

void kmeans_init(clist* datalist, clist** specieslist, int species_count);

void kmeans_run(clist* datalist, clist* specieslist);

int speciate( clist* pointlist, clist* species );

void CalculateCentroidAgent(clist* agentList, Agent* centroid_agent);

#endif // !KMEANS_H