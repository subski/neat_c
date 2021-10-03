#ifndef NEUROLUTION_H
#define NEUROLUTION_H

#include "data_structures/pool.h"
#include "data_structures/clist.h"

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

typedef struct Generation Generation;
typedef struct NeuronHistory_s NeuronHistory_s;

// Memory pools that are used to store data to be reused instead of freeing it.
extern pool* P_AGENT;
extern pool* P_NEURON;
extern pool* P_LINK;

// Total amount of neurons discovered by mutation.
extern uint32_t NeuronCount;

// Keeps track of links which split into a new neuron.
extern NeuronHistory_s NeuronHistory;

// Store the agents and species of a generation step.
extern Generation CurrentGeneration;
extern Generation NextGeneration;

struct Generation
{
	clist* Population;
	clist* Species;
};

struct NeuronHistory_s
{
	uint32_t neuronId;
	int linkId;

	NeuronHistory_s* next;
};

/*
	Main evolution loop.
*/
void evolve(void);

/*
	Fill a cyclic list with a number of Basic Agents.
*/
void createInitialPopulation(clist** population, uint32_t count);

/*
	Compute the fitness of a list of agents.
*/
void population_eval(clist* agents);

/*
	Compute the fitness of each species in a list.
*/
void fitnessSharing(clist* species);

/*
	Perform a selection of the best agents and fill the next generation with offsprings.
*/
void produceNextGeneration(Generation* _gen, Generation* _nxtGen);

/*
	Free the current generation and put the NextGeneration as the current one.
*/
void _advanceGeneration();

/*
	Free resources used by the evolution algorithm.
*/
void free_neurolution();

#endif // !NEUROLUTION_H
