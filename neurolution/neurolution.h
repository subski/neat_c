#ifndef NEUROLUTION_H
#define NEUROLUTION_H

#include "data_structures/vector.h"
#include "data_structures/pool.h"

#include "neurolution/agent.h"

// Web links for speciation methods:
/*
 * K-means algo in plain C :
 *	https://github.com/TheAlgorithms/C/blob/master/machine_learning/k_means_clustering.c
 * K-means applied in SharpNeat :
 *	https://github.com/colgreen/sharpneat-refactor/tree/5a7fee09cf13cb953ebf02d8059fd75ae0e4fc7b/src/SharpNeat/Neat/Speciation/GeneticKMeans
 * Distance calculation in SharpNeat :
 *	https://github.com/colgreen/sharpneat-refactor/tree/5a7fee09cf13cb953ebf02d8059fd75ae0e4fc7b/src/SharpNeat/Neat/DistanceMetrics/Double
 *
 * Affinity Propagation C++ :
 *  https://github.com/nojima/affinity-propagation-sparse
 *  https://github.com/jincheng9/AffinityPropagation
 *  https://github.com/Bismarrck/FastAffinityPropagation
 *
 * Adaptive Species Discovery (ASD) :
 *  https://www.researchgate.net/publication/220741366_Speciation_in_evolutionary_algorithms_Adaptive_Species_Discovery
 */

typedef struct NeuronHistory_s NeuronHistory_s;

extern pool* P_AGENT;
extern pool* P_NEURON;
extern pool* P_LINK;

extern uint32_t NeuronCount;

extern NeuronHistory_s NeuronHistory;

extern clist* Population;
extern clist* Species;

typedef struct Specie
{
	int id;
    Agent* centroid;
	clist* specimens;
} Specie;

struct NeuronHistory_s
{
	uint32_t neuronId;
	int linkId;

	NeuronHistory_s* next;
};

void evolve(void);

void createInitialPopulation(clist** population, uint32_t count);

void free_neurolution();

#endif // !NEUROLUTION_H
